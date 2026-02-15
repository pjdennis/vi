/*
 * vi_term.h - ANSI terminal capability definitions
 *
 * Replaces the original term.h / terminfo interface with hardcoded
 * ANSI/xterm escape sequences. All terminfo capability names used
 * by vi are defined here as static const char* strings.
 *
 * The original vi used setupterm()/tparm()/tputs() from libcurses.
 * We provide vi_setupterm()/vi_tparm()/vi_tputs() as replacements.
 */

#ifndef VI_TERM_H
#define VI_TERM_H

/*
 * Terminal dimensions - set by vi_setupterm() via TIOCGWINSZ.
 * The original code referenced these as global ints from terminfo.
 */
extern int lines;
extern int columns;

/*
 * cur_term: in the original, this was a pointer to the terminal description.
 * We use it as a simple flag: non-zero means initialized.
 */
extern int cur_term;

/*
 * String capabilities - ANSI escape sequences.
 * These are non-const char* because the original code sometimes assigns
 * them to "" to indicate "present but empty" (e.g., insert_line when
 * change_scroll_region is available).
 */

/* Cursor movement */
extern char *cursor_address;      /* cm - parameterized: \033[%d;%dH */
extern char *cursor_up;           /* up */
extern char *cursor_down;         /* do */
extern char *cursor_left;         /* le */
extern char *cursor_right;        /* nd */
extern char *cursor_home;         /* ho */
extern char *cursor_to_ll;        /* ll */
extern char *parm_down_cursor;    /* DO - parameterized */
extern char *parm_up_cursor;      /* UP - parameterized */
extern char *parm_left_cursor;    /* LE - parameterized */
extern char *parm_right_cursor;   /* RI - parameterized */

/* Line/screen manipulation */
extern char *insert_line;         /* al */
extern char *delete_line;         /* dl */
extern char *parm_insert_line;    /* AL - parameterized */
extern char *parm_delete_line;    /* DL - parameterized */
extern char *scroll_forward;      /* sf */
extern char *scroll_reverse;      /* sr */
extern char *change_scroll_region; /* cs - parameterized */
extern char *save_cursor;         /* sc */
extern char *restore_cursor;      /* rc */

/* Clear/erase */
extern char *clear_screen;        /* cl */
extern char *clr_eol;            /* ce */
extern char *clr_eos;            /* cd */

/* Insert/delete mode */
extern char *enter_insert_mode;   /* im */
extern char *exit_insert_mode;    /* ei */
extern char *insert_character;    /* ic */
extern char *delete_character;    /* dc */
extern char *enter_delete_mode;   /* dm */
extern char *exit_delete_mode;    /* ed */
extern char *insert_padding;      /* ip */

/* Standout/highlighting */
extern char *enter_standout_mode; /* so */
extern char *exit_standout_mode;  /* se */

/* Alternate screen */
extern char *enter_ca_mode;       /* ti */
extern char *exit_ca_mode;        /* te */

/* Cursor visibility */
extern char *cursor_visible;      /* vs */
extern char *cursor_normal;       /* ve */

/* Keypad */
extern char *keypad_xmit;        /* ks */
extern char *keypad_local;        /* ke */

/* Audio/visual feedback */
extern char *flash_screen;        /* vb */
extern char *bell;               /* bl */

/* Tab */
extern char *tab;                /* ta - hardware tab */
extern char *back_tab;           /* bt */

/* Newline/return */
extern char *carriage_return;     /* cr */
extern char *newline;            /* nw - newline (behaves like CRLF) */

/* Boolean capabilities (0 or 1) */
extern int auto_right_margin;     /* am */
extern int eat_newline_glitch;    /* xn */
extern int memory_above;         /* da */
extern int memory_below;         /* db */
extern int generic_type;         /* gn */
extern int move_insert_mode;     /* mir */
extern int ceol_standout_glitch; /* xhp */

/* Function keys */
extern char *key_f0, *key_f1, *key_f2, *key_f3, *key_f4;
extern char *key_f5, *key_f6, *key_f7, *key_f8, *key_f9;

/* Special keys */
extern char *key_up;
extern char *key_down;
extern char *key_left;
extern char *key_right;
extern char *key_home;
extern char *key_ic;              /* Insert char key */
extern char *key_eic;            /* Exit insert char key */
extern char *key_il;             /* Insert line key */
extern char *key_dl;             /* Delete line key */
extern char *key_clear;
extern char *key_eol;            /* Clear to end of line key */
extern char *key_sf;             /* Scroll forward key */
extern char *key_dc;             /* Delete char key */
extern char *key_npage;          /* Next page key */
extern char *key_ppage;          /* Prev page key */
extern char *key_sr;             /* Scroll reverse key */
extern char *key_eos;            /* Clear to end of screen key */

/*
 * Function prototypes for terminal operations.
 */

/*
 * vi_setupterm: Initialize terminal. Reads window size via TIOCGWINSZ,
 * installs SIGWINCH handler. Sets lines/columns.
 * type and fd are accepted for compatibility but ignored (always ANSI).
 * errret receives 1 on success.
 */
void vi_setupterm(char *type, int fd, int *errret);

/*
 * vi_tparm: Format a parameterized capability string.
 * Handles cursor_address (row,col) and change_scroll_region (top,bot)
 * with 0-based to 1-based conversion for ANSI.
 * Returns pointer to static buffer.
 */
char *vi_tparm(char *cap, int p1, int p2);

/*
 * vi_tputs: Output a capability string through a function.
 * No padding delays needed on modern terminals.
 */
void vi_tputs(char *cp, int affcnt, int (*outc)(int));

/*
 * cost: Return the cost (in characters) of a capability string.
 */
int vi_cost(char *str);

/*
 * Map the original terminfo function names to our vi_ versions.
 */
#define setupterm(type, fd, errret) vi_setupterm(type, fd, errret)
/* tparm is called with 2 or 3 args; always pass 3 to vi_tparm */
#define VI_TPARM_3(a,b,c,...) vi_tparm(a,b,c)
#define tparm(...) VI_TPARM_3(__VA_ARGS__, 0, 0)
#define tputs(cp, cnt, outc) vi_tputs(cp, cnt, outc)
#define resetterm() /* no-op for ANSI */
#define saveterm()  /* no-op for ANSI */
#define fixterm()   /* no-op for ANSI */

#endif /* VI_TERM_H */
