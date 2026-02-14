/* Copyright (c) 1981 Regents of the University of California */
/* sccs id:	"@(#)ex_re.h	1.3"	(9.1	2/9/83) */
/*
 * Regular expression definitions.
 */
struct	regexp {
	char	Expbuf[ESIZE + 2];
	bool	Circfl;
	short	Nbra;
};

var struct	regexp re;		/* Last re */
var struct	regexp scanre;		/* Last scanning re */
var struct	regexp subre;		/* Last substitute re */

#define	expbuf	re.Expbuf
#define	circfl	re.Circfl
#define	nbra	re.Nbra

#define	savere(a)	copy(&a, &re, sizeof (struct regexp))
#define	resre(a)	copy(&re, &a, sizeof (struct regexp))

/*
 * Definitions for substitute
 */
var char	*braslist[NBRA];	/* Starts of \(\)'ed text in lhs */
var char	*braelist[NBRA];	/* Ends... */
var char	rhsbuf[RHSSIZE];	/* Rhs of last substitute */

/*
 * Definitions of codes for the compiled re's.
 */
#define	STAR	1

#define	CBRA	1
#define	CDOT	4
#define	CCL	8
#define	NCCL	12
#define	CDOL	16
#define	CEOFC	17
#define	CKET	18
#define	CCHR	20
#define	CBRC	24
#define	CLET	25
