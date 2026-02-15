/* Copyright (c) 1981 Regents of the University of California */
/* sccs id:	"@(#)ex_tty.h	1.3"	(9.1	2/9/83) */
#ifndef EX_TTY_H
#define EX_TTY_H
/*
 * Capabilities from termcap/terminfo
 */

/*
 * From the tty modes...
 */
extern	bool	NONL;		/* Terminal can't hack linefeeds doing a CR */
extern	bool	UPPERCASE;	/* Ick! */
extern	int	OCOLUMNS;	/* Save columns for a hack in open mode */

extern	int	outcol;		/* Where the cursor is */
extern	int	outline;

extern	int	destcol;	/* Where the cursor should be */
extern	int	destline;

/*
 * TTY mode handling.
 */
extern	struct	termios tty;	/* Use this one structure to change modes */
typedef	struct termios ttymode;	/* Mode to contain tty flags */

/* var struct tchars ottyc, nttyc;   */
/* var struct ltchars olttyc, nlttyc; */

extern	ttymode	normf;		/* Restore tty flags to this (someday) */
extern	bool	normtty;	/* Have to restore normal mode from normf */

/* Function prototypes for ostart, setty, unixex are in ex.h */

extern	int	costCM;	/* # chars to output a typical cursor_address */
extern	int	costSR;	/* likewise for scroll reverse */
extern	int	costAL;	/* likewise for insert line */
extern	int	costDP;	/* likewise for parm_down_cursor */
extern	int	costLP;	/* likewise for parm_left_cursor */
extern	int	costRP;	/* likewise for parm_right_cursor */
extern	int	costCE;	/* likewise for clear to end of line */
extern	int	costCD;	/* likewise for clear to end of display */

#define MAXNOMACS	128	/* max number of macros of each kind */
#define MAXCHARMACS	2048	/* max # of chars total in macros */

struct maps {
	char *cap;	/* pressing button that sends this.. */
	char *mapto;	/* .. maps to this string */
	char *descr;	/* legible description of key */
};
extern	struct maps arrows[MAXNOMACS];	/* macro defs - 1st 5 built in */
extern	struct maps immacs[MAXNOMACS];	/* for while in insert mode */
extern	struct maps abbrevs[MAXNOMACS];	/* for word abbreviations */
extern	int	ldisc;			/* line discipline for ucb tty driver */
extern	char	mapspace[MAXCHARMACS];
extern	char	*msnext;	/* next free location in mapspace */
extern	int	maphopcnt;	/* check for infinite mapping loops */
extern	bool	anyabbrs;	/* true if abbr or unabbr has been done */
extern	char	ttynbuf[20];	/* result of ttyname() */
extern	int	ttymesg;	/* original mode of users tty */

#endif /* EX_TTY_H */
