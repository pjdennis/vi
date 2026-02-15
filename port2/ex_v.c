/* Copyright (c) 1981 Regents of the University of California */
#include "ex.h"
#include "ex_re.h"
#include "ex_tty.h"
#include "ex_vis.h"

void ovbeg(void);
void ovend(ttymode f);
void setwind(void);
void vok(char *atube);

/*
 * Entry points to open and visual from command mode processor.
 * The open/visual code breaks down roughly as follows:
 *
 * ex_v.c	entry points, checking of terminal characteristics
 *
 * ex_vadj.c	logical screen control, use of intelligent operations
 *		insert/delete line and coordination with screen image;
 *		updating of screen after changes.
 *
 * ex_vget.c	input of single keys and reading of input lines
 *		from the echo area, handling of \ escapes on input for
 *		uppercase only terminals, handling of memory for repeated
 *		commands and small saved texts from inserts and partline
 *		deletes, notification of multi line changes in the echo
 *		area.
 *
 * ex_vmain.c	main command decoding, some command processing.
 *
 * ex_voperate.c   decoding of operator/operand sequences and
 *		contextual scans, implementation of word motions.
 *
 * ex_vops.c	major operator interfaces, undos, motions, deletes,
 *		changes, opening new lines, shifts, replacements and yanks
 *		coordinating logical and physical changes.
 *
 * ex_vops2.c	subroutines for operator interfaces in ex_vops.c,
 *		insert mode, read input line processing at lowest level.
 *
 * ex_vops3.c	structured motion definitions of ( ) { } and [ ] operators,
 *		indent for lisp routines, () and {} balancing.
 *
 * ex_vput.c	output routines, clearing, physical mapping of logical cursor
 *		positioning, cursor motions, handling of insert character
 *		and delete character functions of intelligent and unintelligent
 *		terminals, visual mode tracing routines (for debugging),
 *		control of screen image and its updating.
 *
 * ex_vwind.c	window level control of display, forward and backward rolls,
 *		absolute motions, contextual displays, line depth determination
 */

/*
 * Enter open mode
 */
void
oop()
{
	char *ic;
	char atube[TUBESIZE + LBSIZE];
	ttymode f;	/* mjm: was */

	ovbeg();
	if (peekchar() == '/') {
		ignore(compile(getchar(), 1));
		savere(scanre);
		if (execute(0, dot) == 0)
			error("Fail|Pattern not found on addressed line");
		ic = loc1;
		if (ic > linebuf && *ic == 0)
			ic--;
	} else {
		getDOT();
		ic = vskipwh(linebuf);
	}
	donewline();

	/*
	 * If overstrike then have to HARDOPEN
	 * else if can move cursor up off current line can use CRTOPEN (~~vi1)
	 * otherwise (ugh) have to use ONEOPEN (like adm3)
	 */
	if (cursor_address || cursor_up)
		bastate = CRTOPEN;
	else
		bastate = ONEOPEN;
	setwind();

	/*
	 * To avoid bombing on glass-crt's when the line is too long
	 * pretend that such terminals are 160 columns wide.
	 * If a line is too wide for display, we will dynamically
	 * switch to hardcopy open mode.
	 */
	if (state != CRTOPEN)
		WCOLS = TUBECOLS;
	if (!inglobal)
		savevis();
	vok(atube);
	if (state != CRTOPEN)
		columns = WCOLS;
	Outchar = vputchar;
	f = ostart();
	if (state == CRTOPEN) {
		if (outcol == UKCOL)
			outcol = 0;
		vmoveitup(1, 1);
	} else
		outline = destline = WBOT;
	vshow(dot, NOLINE);
	vnline(ic);
	vmain();
	if (state != CRTOPEN)
		vclean();
	Command = "open";
	ovend(f);
}

void
ovbeg()
{

	if (inopen)
		error("Recursive open/visual not allowed");
	Vlines = lineDOL();
	fixzero();
	setdot();
	pastwh();
	dot = addr2;
}

void
ovend(ttymode f)
{

	splitw = 1;
	vgoto(WECHO, 0);
	vclreol();
	vgoto(WECHO, 0);
	holdcm = 0;
	splitw = 0;
	ostop(f);
	setoutt();
	undvis();
	columns = OCOLUMNS;
	inopen = 0;
	flusho();
	netchHAD(Vlines);
}

/*
 * Enter visual mode
 */
void
vop()
{
	int c;
	char atube[TUBESIZE + LBSIZE];
	ttymode f;	/* mjm: was */

	if (!cursor_address && !cursor_up) {
		if (initev) {
toopen:
			if (generic_type)
				merror("I don't know what kind of terminal you are on - all I have is '%s'.", termtype);
			putNFL();
			merror("[Using open mode]");
			putNFL();
			oop();
			return;
		}
		error("Visual needs addressible cursor or upline capability");
	}
	if (!clear_screen) {
		if (initev)
			goto toopen;
		error("Visual requires clear screen capability");
	}
	if (!scroll_forward) {
		if (initev)
			goto toopen;
		error("Visual requires scrolling");
	}
	ovbeg();
	bastate = VISUAL;
	c = 0;
	if (any(peekchar(), "+-^."))
		c = getchar();
	pastwh();
	vsetsiz(isdigit(peekchar()) ? getnum() : value(WINDOW));
	setwind();
	donewline();
	vok(atube);
	if (!inglobal)
		savevis();
	Outchar = vputchar;
	vmoving = 0;
	f = ostart();
	if (initev == 0) {
		vcontext(dot, c);
		vnline(NOSTR);
	}
	vmain();
	Command = "visual";
	ovend(f);
}

/*
 * Hack to allow entry to visual with
 * empty buffer since routines internally
 * demand at least one line.
 */
void
fixzero()
{

	if (dol == zero) {
		bool ochng = chng;

		vdoappend("");
		if (!ochng)
			vi_sync();
		addr1 = addr2 = one;
	} else if (addr2 == zero)
		addr2 = one;
}

/*
 * Save lines before visual between unddol and truedol.
 * Accomplish this by throwing away current [unddol,truedol]
 * and then saving all the lines in the buffer and moving
 * unddol back to dol.  Don't do this if in a global.
 *
 * If you do
 *	g/xxx/vi.
 * and then do a
 *	:e xxxx
 * at some point, and then quit from the visual and undo
 * you get the old file back.  Somewhat weird.
 */
void
savevis()
{

	if (inglobal)
		return;
	truedol = unddol;
	saveall();
	unddol = dol;
	undkind = UNDNONE;
}

/*
 * Restore a sensible state after a visual/open, moving the saved
 * stuff back to [unddol,dol], and killing the partial line kill indicators.
 */
void
undvis()
{

	if (ruptible)
		vi_signal(SIGINT, onintr);
	squish();
	pkill[0] = pkill[1] = 0;
	unddol = truedol;
	unddel = zero;
	undap1 = one;
	undap2 = dol + 1;
	undkind = UNDALL;
	if (undadot <= zero || undadot > dol)
		undadot = zero+1;
}

/*
 * Set the window parameters based on the base state bastate
 * and the available buffer space.
 */
void
setwind()
{

	WCOLS = columns;
	switch (bastate) {

	case ONEOPEN:
		if (auto_right_margin)
			WCOLS--;
		/* FALLTHROUGH */

	case HARDOPEN:
		basWTOP = WTOP = WBOT = WECHO = 0;
		ZERO = 0;
		holdcm = 1;
		break;

	case CRTOPEN:
		basWTOP = lines - 2;
		/* FALLTHROUGH */

	case VISUAL:
		ZERO = lines - TUBESIZE / WCOLS;
		if (ZERO < 0)
			ZERO = 0;
		if (ZERO > basWTOP)
			error("Screen too large for internal buffer");
		WTOP = basWTOP; WBOT = lines - 2; WECHO = lines - 1;
		break;
	}
	state = bastate;
	basWLINES = WLINES = WBOT - WTOP + 1;
}

/*
 * Can we hack an open/visual on this terminal?
 * If so, then divide the screen buffer up into lines,
 * and initialize a bunch of state variables before we start.
 */
void
vok(char *atube)
{
	int i;

	if (WCOLS == 1000)
		serror("Don't know enough about your terminal to use %s", Command);
	if (WCOLS > TUBECOLS)
		error("Terminal too wide");
	if (WLINES >= TUBELINES || WCOLS * (WECHO - ZERO + 1) > TUBESIZE)
		error("Screen too large");

	vtube0 = atube;
	vclrbyte(atube, WCOLS * (WECHO - ZERO + 1));
	for (i = 0; i < ZERO; i++)
		vtube[i] = NULL;
	for (; i <= WECHO; i++)
		vtube[i] = atube, atube += WCOLS;
	for (; i < TUBELINES; i++)
		vtube[i] = NULL;
	vutmp = atube;
	vundkind = VNONE;
	vUNDdot = 0;
	OCOLUMNS = columns;
	inopen = 1;
	vi_signal(SIGINT, vintr);
	vmoving = 0;
	splitw = 0;
	doomed = 0;
	holdupd = 0;
	Peekkey = 0;
	vcnt = vcline = 0;
	if (vSCROLL == 0)
		vSCROLL = value(SCROLL);
}

void
vintr(int sig)
{
	(void)sig;
	vi_signal(SIGINT, vintr);
	if (vcatch)
		onintr(0);
	ungetkey(ATTN);
	draino();
}

/*
 * Set the size of the screen to size lines, to take effect the
 * next time the screen is redrawn.
 */
void
vsetsiz(int size)
{
	int b;

	if (bastate != VISUAL)
		return;
	b = lines - 1 - size;
	if (b >= lines - 1)
		b = lines - 2;
	if (b < 0)
		b = 0;
	basWTOP = b;
	basWLINES = WBOT - b + 1;
}
