/* Copyright (c) 1981 Regents of the University of California */
/* sccs id:	"@(#)ex_temp.h	1.3"	(9.1	2/9/83) */
/*
 * The editor uses a temporary file for files being edited.
 */

#define	BLKMSK	077777
#define	BNDRY	2
#define	INCRMT	02000
#define	LBTMSK	01776
#define	NMBLKS	077770
#define	OFFBTS	10
#define	OFFMSK	01777
#define	SHFT	0

extern bool	hitin2;		/* Last read hit was ibuff2 not ibuff */
extern bool	ichang2;	/* Have actually changed ibuff2 */
extern bool	ichanged;	/* Have actually changed ibuff */
extern int	iblock;		/* Temp file block number of ibuff (or -1) */
extern int	iblock2;	/* Temp file block number of ibuff2 (or -1) */
extern int	ninbuf;		/* Number useful chars left in input buffer */
extern int	nleft;		/* Number usable chars left in output buffer */
extern int	oblock;		/* Temp file block number of obuff (or -1) */
extern int	tline;

extern char	ibuff[BUFSIZ];
extern char	ibuff2[BUFSIZ];
extern char	obuff[BUFSIZ];

/*
 * Structure of the descriptor block which resides
 * in the first block of the temporary file.
 */

/* This definition also appears in expreserve.c... beware */
struct 	header {
	time_t	Time;			/* Time temp file last updated */
	int	Uid;
	int	Flines;			/* Number of lines in file */
	char	Savedfile[FNSIZE];	/* The current file name */
	int	Blocks[LBLKS];		/* Blocks where line pointers stashed */
};
extern struct 	header H;

#define	uid		H.Uid
#define	flines		H.Flines
#define	savedfile	H.Savedfile
#define	blocks		H.Blocks
