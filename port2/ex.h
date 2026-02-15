/* Copyright (c) 1981 Regents of the University of California */
/* sccs id:	"@(#)e.h	1.1"		(9.3	3/17/83) */

/*
 * Ex version 3 (see exact version in ex_cmds.c, search for /Version/)
 *
 * Mark Horton, U.C. Berkeley
 * Bill Joy, U.C. Berkeley
 * November 1979
 */

#define _DEFAULT_SOURCE
#define getline __posix_getline
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/stat.h>

#include <stdint.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#define bcopy(from, to, n) memmove((to), (from), (n))
#define VFORK fork

/*
 * Terminal capability interface.
 */
#include "vi_term.h"


typedef	int	line;
#include <stdbool.h>

#include "ex_tune.h"
#include "ex_vars.h"

/*
 * Options in the editor are referred to usually by "value(name)" where
 * name is all uppercase, i.e. "value(PROMPT)".
 */
struct	option {
	char	*oname;
	char	*oabbrev;
	int	otype;		/* Types -- see below */
	int	odefault;	/* Default value */
	int	ovalue;		/* Current value */
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
 */
#include <stdio.h>
#undef putchar
#undef getchar
#undef NULL
#define	NULL	0
#undef	EOF
#define	EOF	-1

#undef getline

/*
 * Character constants and bits
 *
 * The editor uses the QUOTE bit as a flag to pass on with characters.
 * The editor never uses a simple char variable.
 */
#define	QUOTE	0200
#define	TRIM	0177
#undef	CTRL
#define	CTRL(c)	((c) & 037)
#define	NL	CTRL('j')
#define	CR	CTRL('m')
#define	DELETE	0177		/* See also ATTN, QUIT in ex_tune.h */
#define	ESCAPE	033

/*
 * Miscellaneous random variables used in more than one place
 */
extern	bool	aiflag;		/* Append/change/insert with autoindent */
extern	bool	anymarks;	/* We have used '[a-z] */
extern	int	chng;		/* Warn "No write" */
extern	char	*Command;
extern	int	defwind;	/* -w# change default window size */
extern	int	dirtcnt;	/* When >= MAXDIRT, should sync temporary */
extern	bool	dosusp;		/* Do SIGTSTP in visual when ^Z typed */
extern	bool	edited;		/* Current file is [Edited] */
extern	line	*endcore;	/* Last available core location */
extern	 bool	endline;	/* Last cmd mode command ended with \n */
/* var short erfile; */
extern	line	*fendcore;	/* First address in line pointer space */
extern	char	file[FNSIZE];	/* Working file name */
extern	char	genbuf[LBSIZE];	/* Working buffer when manipulating linebuf */
extern	bool	hush;		/* Command line option - was given, hush up! */
extern	char	*globp;		/* (Untyped) input string to command mode */
extern	bool	holdcm;		/* Don't cursor address */
extern	bool	inappend;	/* in ex command append mode */
extern	bool	inglobal;	/* Inside g//... or v//... */
extern	char	*initev;	/* Initial : escape for visual */
extern	bool	inopen;		/* Inside open or visual */
extern	char	*input;		/* Current position in cmd line input buffer */
extern	bool	intty;		/* Input is a tty */
extern	int	io;		/* General i/o unit (auto-closed on error!) */
extern	 int	lastc;		/* Last character ret'd from cmd input */
extern	bool	laste;		/* Last command was an "e" (or "rec") */
extern	char	lastmac;	/* Last macro called for ** */
extern	char	lasttag[TAGSIZE];	/* Last argument to a tag command */
extern	char	*linebp;	/* Used in substituting in \n */
extern	char	linebuf[LBSIZE];	/* The primary line buffer */
extern	bool	listf;		/* Command should run in list mode */
extern	char	*loc1;		/* Where re began to match (in linebuf) */
extern	char	*loc2;		/* First char after re match (") */
extern	line	names['z'-'a'+2];	/* Mark registers a-z,' */
extern	int	notecnt;	/* Count for notify (to visual from cmd) */
extern	bool	numberf;	/* Command should run in number mode */
extern	char	obuf[BUFSIZ];	/* Buffer for tty output */
extern	int	oprompt;	/* Saved during source */
extern	int	otchng;		/* Backup tchng to find changes in macros */
extern	int	peekc;		/* Peek ahead character (cmd mode input) */
extern	char	*pkill[2];	/* Trim for put with ragged (LISP) delete */
extern	bool	pfast;		/* Have stty -nl'ed to go faster */
extern	int	pid;		/* Process id of child */
extern	int	ppid;		/* Process id of parent (e.g. main ex proc) */
extern	sigjmp_buf	resetlab;	/* For error throws to top level (cmd mode) */
extern	int	rpid;		/* Pid returned from wait() */
extern	bool	ruptible;	/* Interruptible is normal state */
extern	bool	seenprompt;	/* 1 if have gotten user input */
extern	bool	shudclob;	/* Have a prompt to clobber (e.g. on ^D) */
extern	int	status;		/* Status returned from wait() */
extern	int	tchng;		/* If nonzero, then [Modified] */
extern	int	tfile;		/* Temporary file unit */
extern	bool	vcatch;		/* Want to catch an error (open/visual) */
extern	sigjmp_buf	vreslab;	/* For error throws to a visual catch */
extern	bool	writing;	/* 1 if in middle of a file write */
extern	int	xchng;		/* Suppresses multiple "No writes" in !cmd */

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
#define	getexit(a)	copy(a, resetlab, sizeof (sigjmp_buf))
#define	lastchar()	lastc
#define	outchar(c)	(*Outchar)(c)
#define	pastwh()	(ignore(skipwh()))
#define	pline(no)	(*Pline)(no)
#define	reset()		siglongjmp(resetlab,1)
#define	resexit(a)	copy(resetlab, a, sizeof (sigjmp_buf))
#define	setexit()	sigsetjmp(resetlab, 1)
#define	setlastchar(c)	lastc = c
#define	ungetchar(c)	peekc = c

#define	CATCH		vcatch = 1; if (sigsetjmp(vreslab, 1) == 0) {
#define	ONERR		} else { vcatch = 0;
#define	ENDCATCH	} vcatch = 0;

/*
 * Environment like memory
 */
extern	char	altfile[FNSIZE];	/* Alternate file name */
extern	char	direct[ONMSZ];		/* Temp file goes here */
extern	char	shell[ONMSZ];		/* Copied to be settable */
extern	char	uxb[UXBSIZE + 2];	/* Last !command for !! */

/*
 * The editor data structure for accessing the current file consists
 * of an incore array of pointers into the temporary file tfile.
 */
extern	line	*addr1;			/* First addressed line in a command */
extern	line	*addr2;			/* Second addressed line */
extern	line	*dol;			/* Last line in buffer */
extern	line	*dot;			/* Current line */
extern	line	*one;			/* First line */
extern	line	*truedol;		/* End of all lines, including saves */
extern	line	*unddol;		/* End of undo saved lines */
extern	line	*zero;			/* Points to empty slot before one */

/*
 * Undo information
 */
extern	int	undkind;

extern	line	*unddel;	/* Saved deleted lines go after here */
extern	line	*undap1;	/* Beginning of new lines */
extern	line	*undap2;	/* New lines end before undap2 */
extern	line	*undadot;	/* If we saved all lines, dot reverts here */

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

extern	int	(*Outchar)(int);
extern	int	(*Pline)(int);
extern	int	(*Putchar)(int);
extern	void	(*oldhup)(int);
int	(*setlist(bool t))(int);
int	(*setnorm())(int);
int	(*setnumb(bool t))(int);
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
char	*vgetline(int cnt, char *gcursor, bool *aescaped, char commch);
char	*vinit();
char	*vpastwh();
char	*vskipwh();
void	put(int unused);
void	putreg(int c);
void	YANKreg(int c);
void	delete(int hush);
void	filter(int mode);
int	getfile();
int	getsub();
int	gettty();
void	join(int c);
int	listchar(short c);
int	normchar(short c);
int	normline();
int	numbline();
extern	void	(*oldquit)(int);
void	onhup(int);
void	onintr(int);
void	onsusp(int);
void	vi_signal(int sig, void (*handler)(int));
int	putch(int);
void	shift();
int	termchar(int);
void	vfilter(int c);
void	vintr(int);
int	vputch(int);
void	vshftop(int c);
void	yank(int unused);

#	define	ignore(a)	a
#	define	ignorf(a)	a

#include "vi_mem.h"
