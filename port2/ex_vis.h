/* Copyright (c) 1981 Regents of the University of California */
/* sccs id:	"@(#)ex_vis.h	1.3"	(9.1	2/9/83) */
/*
 * Open and visual mode definitions.
 */

extern	int	bastate;
extern	int	state;

#define	VISUAL		0
#define	CRTOPEN		1
#define	ONEOPEN		2
#define	HARDOPEN	3

extern	int	basWTOP;
extern	int	basWLINES;
extern	int	WTOP;
extern	int	WBOT;
extern	int	WLINES;
extern	int	WCOLS;
extern	int	WECHO;

extern	bool	splitw;

struct vlinfo {
	int	vliny;		/* Y coordinate */
	int	vdepth;		/* Depth of displayed line */
	int	vflags;		/* Is line potentially dirty ? */
};
extern	struct vlinfo  vlinfo[TUBELINES + 2];

#define	DEPTH(c)	(vlinfo[c].vdepth)
#define	LINE(c)		(vlinfo[c].vliny)
#define	FLAGS(c)	(vlinfo[c].vflags)

#define	VDIRT	1

#define	vlcopy(i, j)	i = j;

extern	int	vcline;
extern	int	vcnt;

extern	char	*vtube[TUBELINES];
extern	char	*vtube0;

extern	char	*cursor;
extern	char	*wcursor;
extern	line	*wdot;

#define	VNONE	0
#define	VCHNG	1
#define	VMANY	2
#define	VCAPU	3
#define	VMCHNG	4
#define	VMANYINS 5

extern	int	vundkind;	/* Which kind of undo - from above */
extern	char	*vutmp;		/* Prev line image when "VCHNG" */

#define VC_NOTINMAC	0	/* Not in a macro */
#define VC_NOCHANGE	1	/* In a macro, no changes so far */
#define VC_ONECHANGE	2	/* In a macro, one change so far */
#define VC_MANYCHANGE	3	/* In a macro, at least 2 changes so far */

extern	int	vch_mac;	/* Change state - one of the above */

extern	char	*vUNDcurs;	/* Cursor just before 'U' */
extern	line	*vUNDdot;	/* The line address of line saved in vUNDsav */
extern	line	vUNDsav;	/* Grabbed initial "*dot" */

#define	killU()		vUNDdot = NOLINE

extern	int	hold;
extern	int	holdupd;	/* Hold off update when echo line is too long */

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
extern	int	CDCNT;		/* Count of ^D's in insert on this line */
extern	char	DEL[VBSIZE];	/* Last deleted text */
extern	bool	HADUP;		/* This insert line started with ^ then ^D */
extern	bool	HADZERO;	/* This insert line started with 0 then ^D */
extern	char	INS[VBSIZE];	/* Last inserted text */
extern	int	Vlines;		/* Number of file lines "before" vi command */
extern	int	Xcnt;		/* External variable holding last cmd's count */
extern	bool	Xhadcnt;	/* Last command had explicit count? */
extern	int	ZERO;
extern	int	dir;		/* Direction for search (+1 or -1) */
extern	int	doomed;		/* Disply chars right of cursor to be killed */
extern	bool	gobblebl;	/* Wrapmargin space generated nl, eat a space */
extern	bool	hadcnt;		/* (Almost) internal to vmain() */
extern	bool	heldech;	/* We owe a clear of echo area */
extern	bool	insmode;	/* Are in character insert mode */
extern	char	lastcmd[5];	/* Chars in last command */
extern	int	lastcnt;	/* Count for last command */
extern	char	*lastcp;	/* Save current command here to repeat */
extern	bool	lasthad;	/* Last command had a count? */
extern	int	lastvgk;	/* Previous input key, if not from keyboard */
extern	int	lastreg;	/* Register with last command */
extern	char	*ncols['z'-'a'+2];	/* Cursor positions of marks */
extern	char	*notenam;	/* Name to be noted with change count */
extern	char	*notesgn;	/* Change count from last command */
extern	char	op;		/* Operation of current command */
extern	int	Peekkey;	/* Peek ahead key */
extern	bool	rubble;		/* Line is filthy (in hardcopy open), redraw! */
extern	int	vSCROLL;	/* Number lines to scroll on ^D/^U */
extern	char	*vglobp;	/* Untyped input (e.g. repeat insert text) */
extern	char	vmacbuf[VBSIZE];   /* Text of visual macro, hence nonnestable */
extern	char	*vmacp;		/* Like vglobp but for visual macros */
extern	char	*vmcurs;	/* Cursor for restore after undo d), e.g. */
extern	int	vmovcol;	/* Column to try to keep on arrow keys */
extern	bool	vmoving;	/* Are trying to keep vmovcol */
extern	int	vreg;		/* Reg for this command */
extern	int	wdkind;		/* Liberal/conservative words? */
extern	char	workcmd[5];	/* Temporary for lastcmd */


/*
 * Macros
 */
#define	INF		30000
#define	LASTLINE	LINE(vcnt)
#define	OVERBUF		QUOTE
#define	beep		obeep
#define	cindent()	((outline - vlinfo[vcline].vliny) * WCOLS + outcol)
#define	vputp(cp, cnt)	vi_tputs(cp, cnt, vputch)
#define	vputc(c)	putch(c)

/*
 * Function types
 */
void	beep();
int	qcount(int);
int	vchange(char c);
int	vdelete(char c);
int	vgrabit();
void	vinschar(int);
void	vmove();
int	vputchar(int);
void	vshift();
void	vyankit();
