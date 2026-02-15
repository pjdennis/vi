/*
 * vi_term.c - ANSI terminal capability implementation
 *
 * Provides hardcoded ANSI/xterm escape sequences and the
 * vi_setupterm/vi_tparm/vi_tputs functions that replace
 * the original terminfo/termcap interface.
 */

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "vi_term.h"

/* From ex_subr.c - wrapper around sigaction() */
extern void vi_signal(int sig, void (*handler)(int));

/*
 * Terminal dimensions.
 */
int lines = 24;
int columns = 80;
int cur_term = 0;

/*
 * String capabilities - ANSI escape sequences.
 */

/* Cursor movement */
char *cursor_address = "\033[%d;%dH";     /* parameterized */
char *cursor_up      = "\033[A";
char *cursor_down    = "\033[B";          /* also \n works */
char *cursor_left    = "\b";
char *cursor_right   = "\033[C";
char *cursor_home    = "\033[H";
char *cursor_to_ll   = NULL;              /* go to lower-left; we synthesize */
char *parm_down_cursor  = "\033[%dB";     /* parameterized */
char *parm_up_cursor    = "\033[%dA";     /* parameterized */
char *parm_left_cursor  = "\033[%dD";     /* parameterized */
char *parm_right_cursor = "\033[%dC";     /* parameterized */

/* Line/screen manipulation */
char *insert_line    = "\033[L";
char *delete_line    = "\033[M";
char *parm_insert_line  = "\033[%dL";     /* parameterized */
char *parm_delete_line  = "\033[%dM";     /* parameterized */
char *scroll_forward    = "\n";
char *scroll_reverse    = "\033M";
char *change_scroll_region = "\033[%d;%dr";  /* parameterized */
char *save_cursor    = "\0337";
char *restore_cursor = "\0338";

/* Clear/erase */
char *clear_screen   = "\033[H\033[2J";
char *clr_eol        = "\033[K";
char *clr_eos        = "\033[J";

/* Insert/delete mode */
char *enter_insert_mode = "\033[4h";
char *exit_insert_mode  = "\033[4l";
char *insert_character  = NULL;           /* not needed with insert mode */
char *delete_character  = "\033[P";
char *enter_delete_mode = NULL;
char *exit_delete_mode  = NULL;
char *insert_padding    = NULL;

/* Standout/highlighting */
char *enter_standout_mode = "\033[7m";    /* reverse video */
char *exit_standout_mode  = "\033[m";

/* Alternate screen */
char *enter_ca_mode  = "\033[?1049h";
char *exit_ca_mode   = "\033[?1049l";

/* Cursor visibility */
char *cursor_visible = "\033[?12;25h";    /* blinking + visible */
char *cursor_normal  = "\033[?12l\033[?25h";

/* Keypad */
char *keypad_xmit    = "\033[?1h\033=";
char *keypad_local   = "\033[?1l\033>";

/* Audio/visual feedback */
char *flash_screen   = "\033[?5h\033[?5l";  /* brief reverse video flash */
char *bell           = "\007";

/* Tab */
char *tab            = "\t";
char *back_tab       = "\033[Z";

/* Newline/return */
char *carriage_return = "\r";
char *newline         = "\r\n";

/* Boolean capabilities */
int auto_right_margin   = 1;    /* xterm wraps at right margin */
int eat_newline_glitch  = 1;    /* xterm eats newline after wrap */
int memory_above        = 0;
int memory_below        = 0;
int generic_type         = 0;
int move_insert_mode     = 1;  /* modern terminals support cursor movement in insert mode */
int ceol_standout_glitch = 0;

/* Function keys (xterm sequences) */
char *key_f0  = "\033[10~";
char *key_f1  = "\033OP";
char *key_f2  = "\033OQ";
char *key_f3  = "\033OR";
char *key_f4  = "\033OS";
char *key_f5  = "\033[15~";
char *key_f6  = "\033[17~";
char *key_f7  = "\033[18~";
char *key_f8  = "\033[19~";
char *key_f9  = "\033[20~";

/* Special keys (xterm application mode sequences) */
char *key_up    = "\033OA";
char *key_down  = "\033OB";
char *key_left  = "\033OD";
char *key_right = "\033OC";
char *key_home  = "\033[1~";
char *key_ic    = "\033[2~";      /* Insert */
char *key_eic   = NULL;
char *key_il    = NULL;           /* no standard key for insert line */
char *key_dl    = NULL;           /* no standard key for delete line */
char *key_clear = NULL;
char *key_eol   = NULL;
char *key_sf    = NULL;
char *key_dc    = "\033[3~";      /* Delete */
char *key_npage = "\033[6~";      /* Page Down */
char *key_ppage = "\033[5~";      /* Page Up */
char *key_sr    = NULL;
char *key_eos   = NULL;

/*
 * SIGWINCH handler - update terminal dimensions.
 */
static void
sigwinch_handler(int sig)
{
	struct winsize ws;

	(void)sig;
	if (ioctl(2, TIOCGWINSZ, &ws) == 0) {
		if (ws.ws_row > 0)
			lines = ws.ws_row;
		if (ws.ws_col > 0)
			columns = ws.ws_col;
	}
}

/*
 * vi_setupterm: Initialize terminal. Read window size, install SIGWINCH.
 */
void
vi_setupterm(char *type, int fd, int *errret)
{
	struct winsize ws;

	(void)type;
	(void)fd;

	/* Get actual terminal dimensions */
	if (ioctl(2, TIOCGWINSZ, &ws) == 0) {
		if (ws.ws_row > 0)
			lines = ws.ws_row;
		if (ws.ws_col > 0)
			columns = ws.ws_col;
	}

	/* Synthesize cursor_to_ll (move to last line, column 0) */
	/* This is done dynamically since it depends on lines */
	{
		static char ll_buf[20];
		snprintf(ll_buf, sizeof(ll_buf), "\033[%d;1H", lines);
		cursor_to_ll = ll_buf;
	}

	/* Install SIGWINCH handler */
	vi_signal(SIGWINCH, sigwinch_handler);

	cur_term = 1;
	if (errret)
		*errret = 1;
}

/*
 * vi_tparm: Format parameterized capability strings.
 *
 * The original terminfo tparm() processed % sequences in capability strings.
 * For ANSI terminals, the parameterized capabilities are:
 *   cursor_address:      \033[row;colH     (0-based -> 1-based)
 *   change_scroll_region: \033[top;botH     (0-based -> 1-based)
 *   parm_{up,down,left,right}_cursor: \033[nX
 *   parm_{insert,delete}_line: \033[nX
 *
 * We detect the capability by its address and format accordingly.
 */
static char tparm_buf[64];

char *
vi_tparm(char *cap, int p1, int p2)
{
	if (cap == NULL) {
		tparm_buf[0] = 'O';  /* signals "OOPS" to caller */
		tparm_buf[1] = 0;
		return tparm_buf;
	}

	/*
	 * cursor_address and change_scroll_region take two parameters
	 * and need 0-to-1-based conversion.
	 */
	if (cap == cursor_address) {
		snprintf(tparm_buf, sizeof(tparm_buf),
			"\033[%d;%dH", p1 + 1, p2 + 1);
		return tparm_buf;
	}
	if (cap == change_scroll_region) {
		snprintf(tparm_buf, sizeof(tparm_buf),
			"\033[%d;%dr", p1 + 1, p2 + 1);
		return tparm_buf;
	}

	/*
	 * Single-parameter capabilities: parm_{up,down,left,right}_cursor,
	 * parm_{insert,delete}_line. These use p1 directly (no conversion).
	 */
	if (cap == parm_down_cursor) {
		snprintf(tparm_buf, sizeof(tparm_buf), "\033[%dB", p1);
		return tparm_buf;
	}
	if (cap == parm_up_cursor) {
		snprintf(tparm_buf, sizeof(tparm_buf), "\033[%dA", p1);
		return tparm_buf;
	}
	if (cap == parm_left_cursor) {
		snprintf(tparm_buf, sizeof(tparm_buf), "\033[%dD", p1);
		return tparm_buf;
	}
	if (cap == parm_right_cursor) {
		snprintf(tparm_buf, sizeof(tparm_buf), "\033[%dC", p1);
		return tparm_buf;
	}
	if (cap == parm_insert_line) {
		snprintf(tparm_buf, sizeof(tparm_buf), "\033[%dL", p1);
		return tparm_buf;
	}
	if (cap == parm_delete_line) {
		snprintf(tparm_buf, sizeof(tparm_buf), "\033[%dM", p1);
		return tparm_buf;
	}

	/* Unknown parameterized capability - return as-is */
	return cap;
}

/*
 * vi_tputs: Output a string through a function pointer.
 * No padding needed on modern terminals.
 */
void
vi_tputs(char *cp, int affcnt, int (*outc)(int))
{
	(void)affcnt;
	if (cp == NULL)
		return;
	while (*cp)
		(*outc)(*cp++);
}

/*
 * vi_cost: Return the number of characters in a capability string.
 */
int
vi_cost(char *str)
{
	if (str == NULL || *str == 'O')  /* OOPS */
		return 10000;  /* infinity */
	return strlen(str);
}
