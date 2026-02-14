/* Copyright (c) 1981 Regents of the University of California */
/* sccs id:	"@(#)e.h	1.1"		(9.3	3/17/83) */

/*
 * Ex version 3 (see exact version in ex_cmds.c, search for /Version/)
 *
 * Mark Horton, U.C. Berkeley
 * Bill Joy, U.C. Berkeley
 * November 1979
 *
 * PORT_NOTE: Ported to modern Linux/WSL, February 2026.
 */

/* PORT_NOTE: Hide POSIX getline() which clashes with vi's own getline(tl) */
#define getline __posix_getline
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/stat.h>

/*
 * PORT_NOTE: Compatibility layer: termios, string.h, stdlib.h, unistd.h, etc.
 */
#include "vi_compat.h"

/*
 * Terminal capability interface.
 * ORIG: #include "term.h"
 * Replaced with our ANSI-based terminal wrapper.
 */
#include "vi_term.h"

#ifndef var
#define var	extern
#endif

/*
 * ORIG_BEGIN: USG CBREAK simulation
 * The original defined CBREAK to a garbage value under USG to simulate it.
 * With termios, we use raw/cbreak mode directly. Define CBREAK for
 * conditional compilation blocks that test it.
 */
/* ORIG: #define CBREAK xxxxx */
#define CBREAK 1
/* ORIG_END */

/*
 * ORIG: extern int errno;
 * Modern C: errno is provided by <errno.h> (included above).
 */

/*
 * PORT_NOTE: Line type: 32-bit for VMUNIX (allows large files).
 */
/* ORIG_BEGIN: VMUNIX conditional for line type */
/* PORT_NOTE: typedef short line;  -- non-VMUNIX */
/* ORIG_END */
typedef	int	line;
typedef	short	bool;

#include "ex_tune.h"
#include "ex_vars.h"

/*
 * Options in the editor are referred to usually by "value(name)" where
 * name is all uppercase, i.e. "value(PROMPT)".
 */
struct	option {
	char	*oname;
	char	*oabbrev;
	short	otype;		/* Types -- see below */
	short	odefault;	/* Default value */
	short	ovalue;		/* Current value */
	char	*osvalue;
};

#define	ONOFF	0
#define	NUMERIC	1
#define	STRING	2		/* SHELL or DIRECTORY */
#define	OTERM	3

#define	value(a)	options[a].ovalue
#define	svalue(a)	options[a].osvalue

extern	 struct	option options[NOPTS + 1];


/*
 * The editor does not normally use the standard i/o library.
 * PORT_NOTE: We define BUFSIZ, NULL, EOF ourselves when not in TRACE mode.
 * PORT_NOTE: Our printf.c provides vi_printf (renamed to avoid libc collision).
 */
#ifdef TRACE
#	include <stdio.h>
	var	FILE	*trace;
	var	bool	trubble;
	var	bool	techoin;
	var	char	tracbuf[BUFSIZ];
#	undef	putchar
#	undef	getchar
#else
/*
 * ORIG_BEGIN: manual BUFSIZ definition
 * Warning: do not change BUFSIZ without also changing LBSIZE in ex_tune.h
 */
/* ORIG: #define BUFSIZ 1024 */
/* PORT_NOTE: On modern systems, stdio.h always provides BUFSIZ. Include it
 * just for the BUFSIZ definition, then undef the macros we override. */
#include <stdio.h>
#undef putchar
#undef getchar
#undef NULL
#define	NULL	0
#undef	EOF
#define	EOF	-1
/* ORIG_END */
#endif

#undef getline  /* PORT_NOTE: restore vi's own getline after stdio.h is done */

/*
 * Character constants and bits
 *
 * The editor uses the QUOTE bit as a flag to pass on with characters.
 * The editor never uses a simple char variable.
 */
#define	QUOTE	0200
#define	TRIM	0177
/*
 * ORIG: #define CTRL(c) ('c' & 037)
 * The original macro stringified its argument: CTRL(j) -> 'j' & 037.
 * Modern C doesn't do this. Fix: CTRL('j') with proper parenthesized macro.
 */
#undef	CTRL
#define	CTRL(c)	((c) & 037)
#define	NL	CTRL('j')
#define	CR	CTRL('m')
#define	DELETE	0177		/* See also ATTN, QUIT in ex_tune.h */
#define	ESCAPE	033

/*
 * Miscellaneous random variables used in more than one place
 */
var	bool	aiflag;		/* Append/change/insert with autoindent */
var	bool	anymarks;	/* We have used '[a-z] */
var	int	chng;		/* Warn "No write" */
var	char	*Command;
var	short	defwind;	/* -w# change default window size */
var	int	dirtcnt;	/* When >= MAXDIRT, should sync temporary */
var	bool	dosusp;		/* Do SIGTSTP in visual when ^Z typed */
var	bool	edited;		/* Current file is [Edited] */
var	line	*endcore;	/* Last available core location */
extern	 bool	endline;	/* Last cmd mode command ended with \n */
/* ORIG_REMOVED: erfile - not needed for VMUNIX */
/* var short erfile; */
/* :ORIG_REMOVED */
var	line	*fendcore;	/* First address in line pointer space */
var	char	file[FNSIZE];	/* Working file name */
var	char	genbuf[LBSIZE];	/* Working buffer when manipulating linebuf */
var	bool	hush;		/* Command line option - was given, hush up! */
var	char	*globp;		/* (Untyped) input string to command mode */
var	bool	holdcm;		/* Don't cursor address */
var	bool	inappend;	/* in ex command append mode */
var	bool	inglobal;	/* Inside g//... or v//... */
var	char	*initev;	/* Initial : escape for visual */
var	bool	inopen;		/* Inside open or visual */
var	char	*input;		/* Current position in cmd line input buffer */
var	bool	intty;		/* Input is a tty */
var	short	io;		/* General i/o unit (auto-closed on error!) */
extern	 short	lastc;		/* Last character ret'd from cmd input */
var	bool	laste;		/* Last command was an "e" (or "rec") */
var	char	lastmac;	/* Last macro called for ** */
var	char	lasttag[TAGSIZE];	/* Last argument to a tag command */
var	char	*linebp;	/* Used in substituting in \n */
var	char	linebuf[LBSIZE];	/* The primary line buffer */
var	bool	listf;		/* Command should run in list mode */
var	char	*loc1;		/* Where re began to match (in linebuf) */
var	char	*loc2;		/* First char after re match (") */
var	line	names['z'-'a'+2];	/* Mark registers a-z,' */
var	int	notecnt;	/* Count for notify (to visual from cmd) */
var	bool	numberf;	/* Command should run in number mode */
var	char	obuf[BUFSIZ];	/* Buffer for tty output */
var	short	oprompt;	/* Saved during source */
var	short	ospeed;		/* Output speed (from gtty) */
var	int	otchng;		/* Backup tchng to find changes in macros */
var	short	peekc;		/* Peek ahead character (cmd mode input) */
var	char	*pkill[2];	/* Trim for put with ragged (LISP) delete */
var	bool	pfast;		/* Have stty -nl'ed to go faster */
var	int	pid;		/* Process id of child */
var	int	ppid;		/* Process id of parent (e.g. main ex proc) */
var	jmp_buf	resetlab;	/* For error throws to top level (cmd mode) */
var	int	rpid;		/* Pid returned from wait() */
var	bool	ruptible;	/* Interruptible is normal state */
var	bool	seenprompt;	/* 1 if have gotten user input */
var	bool	shudclob;	/* Have a prompt to clobber (e.g. on ^D) */
var	int	status;		/* Status returned from wait() */
var	int	tchng;		/* If nonzero, then [Modified] */
extern	short	tfile;		/* Temporary file unit */
var	bool	vcatch;		/* Want to catch an error (open/visual) */
var	jmp_buf	vreslab;	/* For error throws to a visual catch */
var	bool	writing;	/* 1 if in middle of a file write */
var	int	xchng;		/* Suppresses multiple "No writes" in !cmd */

/*
 * Macros
 */
#define	CP(a, b)	(ignore(strcpy(a, b)))
			/*
			 * FIXUNDO: do we want to mung undo vars?
			 * Usually yes unless in a macro or global.
			 */
#define FIXUNDO		(inopen >= 0 && (inopen || !inglobal))
#define ckaw()		{if (chng && value(AUTOWRITE) && !value(READONLY)) wop(0);}
#define	copy(a,b,c)	Copy((char *) (a), (char *) (b), (c))
#define	eq(a, b)	((a) && (b) && strcmp(a, b) == 0)
#define	getexit(a)	copy(a, resetlab, sizeof (jmp_buf))
#define	lastchar()	lastc
#define	outchar(c)	(*Outchar)(c)
#define	pastwh()	(ignore(skipwh()))
#define	pline(no)	(*Pline)(no)
#define	reset()		longjmp(resetlab,1)
#define	resexit(a)	copy(resetlab, a, sizeof (jmp_buf))
#define	setexit()	setjmp(resetlab)
#define	setlastchar(c)	lastc = c
#define	ungetchar(c)	peekc = c

#define	CATCH		vcatch = 1; if (setjmp(vreslab) == 0) {
#define	ONERR		} else { vcatch = 0;
#define	ENDCATCH	} vcatch = 0;

/*
 * Environment like memory
 */
var	char	altfile[FNSIZE];	/* Alternate file name */
extern	char	direct[ONMSZ];		/* Temp file goes here */
extern	char	shell[ONMSZ];		/* Copied to be settable */
var	char	uxb[UXBSIZE + 2];	/* Last !command for !! */

/*
 * The editor data structure for accessing the current file consists
 * of an incore array of pointers into the temporary file tfile.
 */
var	line	*addr1;			/* First addressed line in a command */
var	line	*addr2;			/* Second addressed line */
var	line	*dol;			/* Last line in buffer */
var	line	*dot;			/* Current line */
var	line	*one;			/* First line */
var	line	*truedol;		/* End of all lines, including saves */
var	line	*unddol;		/* End of undo saved lines */
var	line	*zero;			/* Points to empty slot before one */

/*
 * Undo information
 */
var	short	undkind;

var	line	*unddel;	/* Saved deleted lines go after here */
var	line	*undap1;	/* Beginning of new lines */
var	line	*undap2;	/* New lines end before undap2 */
var	line	*undadot;	/* If we saved all lines, dot reverts here */

#define	UNDCHANGE	0
#define	UNDMOVE		1
#define	UNDALL		2
#define	UNDNONE		3
#define	UNDPUT		4

/*
 * Function type definitions
 */
#define	NOSTR	(char *) 0
#define	NOLINE	(line *) 0

extern	int	(*Outchar)();
extern	int	(*Pline)();
extern	int	(*Putchar)();
var	void	(*oldhup)();
int	(*setlist())();
int	(*setnorm())();
int	(*setnumb())();
line	*address();
char	*cgoto();
char	*genindent();
char	*getblock();
line	*getmark();
char	*mesg();
char	*place();
char	*plural();
line	*scanfor();
line	*setin();
char	*strend();
char	*tailpath();
line	*vback();
char	*vfindcol();
char	*vgetline();
char	*vinit();
char	*vpastwh();
char	*vskipwh();
int	put();
int	putreg();
int	YANKreg();
int	delete();
int	filter();
int	getfile();
int	getsub();
int	gettty();
int	join();
int	listchar();
int	normchar();
int	normline();
int	numbline();
var	void	(*oldquit)();
void	onhup();
void	onintr();
void	onsusp();
int	putch();
int	shift();
int	termchar();
int	vfilter();
#ifdef CBREAK
int	vintr();
#endif
int	vputch();
int	vshftop();
int	yank();

/*
 * C doesn't have a (void) cast, so we have to fake it for lint's sake.
 */
#ifdef lint
#	define	ignore(a)	Ignore((char *) (a))
#	define	ignorf(a)	Ignorf((int (*) ()) (a))
#else
#	define	ignore(a)	a
#	define	ignorf(a)	a
#endif

/*
 * PORT_NOTE: Memory management (replaces sbrk/brk)
 */
#include "vi_mem.h"
