/* Copyright (c) 1981 Regents of the University of California */
/* sccs id:	"@(#)ex_tty.h	1.3"	(9.1	2/9/83) */
/*
 * Capabilities from termcap/terminfo
 */

/*
 * From the tty modes...
 */
var	bool	NONL;		/* Terminal can't hack linefeeds doing a CR */
var	bool	UPPERCASE;	/* Ick! */
var	short	OCOLUMNS;	/* Save columns for a hack in open mode */

var	short	outcol;		/* Where the cursor is */
var	short	outline;

var	short	destcol;	/* Where the cursor should be */
var	short	destline;

/*
 * TTY mode handling.
 */
var	struct	termios tty;	/* Use this one structure to change modes */
typedef	struct termios ttymode;	/* Mode to contain tty flags */

/* var struct tchars ottyc, nttyc;   */
/* var struct ltchars olttyc, nlttyc; */

var	ttymode	normf;		/* Restore tty flags to this (someday) */
var	bool	normtty;	/* Have to restore normal mode from normf */

ttymode ostart(), setty(), unixex();

var	short	costCM;	/* # chars to output a typical cursor_address */
var	short	costSR;	/* likewise for scroll reverse */
var	short	costAL;	/* likewise for insert line */
var	short	costDP;	/* likewise for parm_down_cursor */
var	short	costLP;	/* likewise for parm_left_cursor */
var	short	costRP;	/* likewise for parm_right_cursor */
var	short	costCE;	/* likewise for clear to end of line */
var	short	costCD;	/* likewise for clear to end of display */

#define MAXNOMACS	128	/* max number of macros of each kind */
#define MAXCHARMACS	2048	/* max # of chars total in macros */

struct maps {
	char *cap;	/* pressing button that sends this.. */
	char *mapto;	/* .. maps to this string */
	char *descr;	/* legible description of key */
};
var	struct maps arrows[MAXNOMACS];	/* macro defs - 1st 5 built in */
var	struct maps immacs[MAXNOMACS];	/* for while in insert mode */
var	struct maps abbrevs[MAXNOMACS];	/* for word abbreviations */
var	int	ldisc;			/* line discipline for ucb tty driver */
var	char	mapspace[MAXCHARMACS];
var	char	*msnext;	/* next free location in mapspace */
var	int	maphopcnt;	/* check for infinite mapping loops */
var	bool	anyabbrs;	/* true if abbr or unabbr has been done */
var	char	ttynbuf[20];	/* result of ttyname() */
var	int	ttymesg;	/* original mode of users tty */
