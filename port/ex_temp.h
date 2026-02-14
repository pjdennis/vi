/* Copyright (c) 1981 Regents of the University of California */
/* sccs id:	"@(#)ex_temp.h	1.3"	(9.1	2/9/83) */
/*
 * The editor uses a temporary file for files being edited.
 * PORT_NOTE: Resolved to VMUNIX values (32-bit line pointers, large buffers).
 */

/* ORIG_BEGIN: VMUNIX conditional for temp file parameters */
/* Non-VMUNIX values removed; only VMUNIX values kept */
/* ORIG_END */
#define	BLKMSK	077777
#define	BNDRY	2
#define	INCRMT	02000
#define	LBTMSK	01776
#define	NMBLKS	077770
#define	OFFBTS	10
#define	OFFMSK	01777
#define	SHFT	0

var bool	hitin2;		/* Last read hit was ibuff2 not ibuff */
var bool	ichang2;	/* Have actually changed ibuff2 */
var bool	ichanged;	/* Have actually changed ibuff */
var short	iblock;		/* Temp file block number of ibuff (or -1) */
var short	iblock2;	/* Temp file block number of ibuff2 (or -1) */
var short	ninbuf;		/* Number useful chars left in input buffer */
var short	nleft;		/* Number usable chars left in output buffer */
var short	oblock;		/* Temp file block number of obuff (or -1) */
/* ORIG: VMUNIX uses int tline */
var int	tline;

var char	ibuff[BUFSIZ];
var char	ibuff2[BUFSIZ];
var char	obuff[BUFSIZ];

/*
 * Structure of the descriptor block which resides
 * in the first block of the temporary file.
 */

/* This definition also appears in expreserve.c... beware */
struct 	header {
	time_t	Time;			/* Time temp file last updated */
	int	Uid;
	/* ORIG: VMUNIX uses int Flines */
	int	Flines;			/* Number of lines in file */
	char	Savedfile[FNSIZE];	/* The current file name */
	short	Blocks[LBLKS];		/* Blocks where line pointers stashed */
};
var struct 	header H;

#define	uid		H.Uid
#define	flines		H.Flines
#define	savedfile	H.Savedfile
#define	blocks		H.Blocks
