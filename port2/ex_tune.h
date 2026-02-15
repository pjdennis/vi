/* Copyright (c) 1981 Regents of the University of California */
/* sccs id:	"@(#)ex_tune.h	1.4"	(9.2	3/17/83) */
#ifndef EX_TUNE_H
#define EX_TUNE_H
/*
 * Definitions of editor parameters and limits
 */

/*
 * Pathnames.
 */
#define	EXRECOVER	"/usr/libexec/ex/recover"
#define	EXPRESERVE	"/usr/libexec/ex/preserve"

/*
 * If your system believes that tabs expand to a width other than
 * 8 then your makefile should cc with -DTABS=whatever, otherwise we use 8.
 */
#ifndef TABS
#define	TABS	8
#endif

/*
 * Maximums
 *
 * The definitions of LBSIZE and CRSIZE should be the same as BUFSIZ
 * Most other definitions are quite generous.
 */

#define LBSIZE		BUFSIZ		/* Line buffer size */
#define CRSIZE		BUFSIZ		/* Crypt block size */

#define ESIZE		512
/* FNSIZE is also defined in expreserve.c */
#define	FNSIZE		128		/* Max file name size */
#define	RHSSIZE		256		/* Size of rhs of substitute */
#define	NBRA		9		/* Number of re \( \) pairs */
#define	TAGSIZE		32		/* Tag length */
#define	ONMSZ		128		/* Option name size */
#define	GBSIZE		256		/* Buffer size */
#define	UXBSIZE		128		/* Unix command buffer size */
#define	VBSIZE		128		/* Partial line max size in visual */
/* LBLKS is also defined in expreserve.c */
#define	LBLKS		900
#define	HBLKS		2
#define	MAXDIRT		12		/* Max dirtcnt before sync tfile */
#define TCBUFSIZE	1024		/* Max entry size in termcap */

#define	NCARGS	5120
#define	NARGS	(NCARGS/6)

#define	TUBELINES	100
#define	TUBECOLS	160
#define	TUBESIZE	10000	/* 100 * 100 */

/*
 * Output column (and line) are set to this value on cursor addressible
 * terminals when we lose track of the cursor to force cursor
 * addressing to occur.
 */
#define	UKCOL		-20	/* Prototype unknown column */

/*
 * Attention is the interrupt character (normally 0177 -- delete).
 * Quit is the quit signal (normally fs -- control-\) and quits open/visual.
 */
#define	ATTN	(-2)
#define	QUIT	('\\' & 037)

#endif /* EX_TUNE_H */
