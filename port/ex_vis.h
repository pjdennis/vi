/* Copyright (c) 1981 Regents of the University of California */
/* sccs id:	"@(#)ex_vis.h	1.3"	(9.1	2/9/83) */
/*
 * Open and visual mode definitions.
 */

var	short	bastate;
var	short	state;

#define	VISUAL		0
#define	CRTOPEN		1
#define	ONEOPEN		2
#define	HARDOPEN	3

var	short	basWTOP;
var	short	basWLINES;
var	short	WTOP;
var	short	WBOT;
var	short	WLINES;
var	short	WCOLS;
var	short	WECHO;

var	bool	splitw;

struct vlinfo {
	short	vliny;		/* Y coordinate */
	short	vdepth;		/* Depth of displayed line */
	short	vflags;		/* Is line potentially dirty ? */
};
var	struct vlinfo  vlinfo[TUBELINES + 2];

#define	DEPTH(c)	(vlinfo[c].vdepth)
#define	LINE(c)		(vlinfo[c].vliny)
#define	FLAGS(c)	(vlinfo[c].vflags)

#define	VDIRT	1

/* ORIG_REMOVED: V6 kludge for no structure assignment */
/* ORIG: #ifdef V6 ... struct { long longi; }; ... #endif */
/* :ORIG_REMOVED */
#define	vlcopy(i, j)	i = j;

var	short	vcline;
var	short	vcnt;

var	char	*vtube[TUBELINES];
var	char	*vtube0;

var	char	*cursor;
var	char	*wcursor;
var	line	*wdot;

#define	VNONE	0
#define	VCHNG	1
#define	VMANY	2
#define	VCAPU	3
#define	VMCHNG	4
#define	VMANYINS 5

var	short	vundkind;	/* Which kind of undo - from above */
var	char	*vutmp;		/* Prev line image when "VCHNG" */

#define VC_NOTINMAC	0	/* Not in a macro */
#define VC_NOCHANGE	1	/* In a macro, no changes so far */
#define VC_ONECHANGE	2	/* In a macro, one change so far */
#define VC_MANYCHANGE	3	/* In a macro, at least 2 changes so far */

var	short	vch_mac;	/* Change state - one of the above */

var	char	*vUNDcurs;	/* Cursor just before 'U' */
var	line	*vUNDdot;	/* The line address of line saved in vUNDsav */
var	line	vUNDsav;	/* Grabbed initial "*dot" */

#define	killU()		vUNDdot = NOLINE

var	short	hold;
var	short	holdupd;	/* Hold off update when echo line is too long */

#define	HOLDAT		1
#define	HOLDDOL		2
#define	HOLDROL		4
#define	HOLDQIK		8
#define	HOLDPUPD	16
#define	HOLDECH		32
#define HOLDWIG		64

/*
 * Miscellaneous variables
 */
var	short	CDCNT;		/* Count of ^D's in insert on this line */
var	char	DEL[VBSIZE];	/* Last deleted text */
var	bool	HADUP;		/* This insert line started with ^ then ^D */
var	bool	HADZERO;	/* This insert line started with 0 then ^D */
var	char	INS[VBSIZE];	/* Last inserted text */
var	int	Vlines;		/* Number of file lines "before" vi command */
var	int	Xcnt;		/* External variable holding last cmd's count */
var	bool	Xhadcnt;	/* Last command had explicit count? */
var	short	ZERO;
var	short	dir;		/* Direction for search (+1 or -1) */
var	short	doomed;		/* Disply chars right of cursor to be killed */
var	bool	gobblebl;	/* Wrapmargin space generated nl, eat a space */
var	bool	hadcnt;		/* (Almost) internal to vmain() */
var	bool	heldech;	/* We owe a clear of echo area */
var	bool	insmode;	/* Are in character insert mode */
var	char	lastcmd[5];	/* Chars in last command */
var	int	lastcnt;	/* Count for last command */
var	char	*lastcp;	/* Save current command here to repeat */
var	bool	lasthad;	/* Last command had a count? */
var	short	lastvgk;	/* Previous input key, if not from keyboard */
var	short	lastreg;	/* Register with last command */
var	char	*ncols['z'-'a'+2];	/* Cursor positions of marks */
var	char	*notenam;	/* Name to be noted with change count */
var	char	*notesgn;	/* Change count from last command */
var	char	op;		/* Operation of current command */
var	short	Peekkey;	/* Peek ahead key */
var	bool	rubble;		/* Line is filthy (in hardcopy open), redraw! */
var	int	vSCROLL;	/* Number lines to scroll on ^D/^U */
var	char	*vglobp;	/* Untyped input (e.g. repeat insert text) */
var	char	vmacbuf[VBSIZE];   /* Text of visual macro, hence nonnestable */
var	char	*vmacp;		/* Like vglobp but for visual macros */
var	char	*vmcurs;	/* Cursor for restore after undo d), e.g. */
var	short	vmovcol;	/* Column to try to keep on arrow keys */
var	bool	vmoving;	/* Are trying to keep vmovcol */
var	short	vreg;		/* Reg for this command */
var	short	wdkind;		/* Liberal/conservative words? */
var	char	workcmd[5];	/* Temporary for lastcmd */


/*
 * Macros
 */
#define	INF		30000
#define	LASTLINE	LINE(vcnt)
#define	OVERBUF		QUOTE
#define	beep		obeep
#define	cindent()	((outline - vlinfo[vcline].vliny) * WCOLS + outcol)
/*
 * ORIG: #define vputp(cp, cnt) tputs(cp, cnt, vputch)
 * Updated to use vi_tputs.
 */
#define	vputp(cp, cnt)	vi_tputs(cp, cnt, vputch)
#define	vputc(c)	putch(c)

/*
 * Function types
 */
int	beep();
int	qcount();
int	vchange();
int	vdelete();
int	vgrabit();
int	vinschar();
int	vmove();
int	vputchar();
int	vshift();
int	vyankit();
