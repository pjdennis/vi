/* Copyright (c) 1981 Regents of the University of California */
/* sccs id:	"@(#)ex_argv.h	1.3"	(7.2	7/26/81) */
#ifndef EX_ARGV_H
#define EX_ARGV_H
/*
 * The current implementation of the argument list is poor,
 * using an argv even for internally done "next" commands.
 */
extern char	**argv;
extern char	**argv0;
extern char	*args;
extern char	*args0;
extern int	argc;
extern int	argc0;
extern int	morargc;		/* Used with "More files to edit..." */

extern int	firstln;		/* From +lineno */
extern char	*firstpat;		/* From +/pat	*/

/* Yech... */
struct	glob {
	int	argc;			/* Index of current file in argv */
	int	argc0;			/* Number of arguments in argv */
	char	*argv[NARGS + 1];	/* WHAT A WASTE! */
	char	argspac[NCARGS + sizeof (int)];
};
extern struct	glob frob;

#endif /* EX_ARGV_H */
