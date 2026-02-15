/* Copyright (c) 1981 Regents of the University of California */
/* sccs id:	"@(#)ex_re.h	1.3"	(9.1	2/9/83) */
#ifndef EX_RE_H
#define EX_RE_H
/*
 * Regular expression definitions.
 */
struct	regexp {
	char	Expbuf[ESIZE + 2];
	bool	Circfl;
	int	Nbra;
};

extern struct	regexp re;		/* Last re */
extern struct	regexp scanre;		/* Last scanning re */
extern struct	regexp subre;		/* Last substitute re */

#define	expbuf	re.Expbuf
#define	circfl	re.Circfl
#define	nbra	re.Nbra

#define	savere(a)	copy(&a, &re, sizeof (struct regexp))
#define	resre(a)	copy(&re, &a, sizeof (struct regexp))

/*
 * Definitions for substitute
 */
extern char	*braslist[NBRA];	/* Starts of \(\)'ed text in lhs */
extern char	*braelist[NBRA];	/* Ends... */
extern char	rhsbuf[RHSSIZE];	/* Rhs of last substitute */

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

#endif /* EX_RE_H */
