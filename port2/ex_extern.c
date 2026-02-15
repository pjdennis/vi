/*
 * Provide definitions of all global variables.
 * Globals declared 'extern' in the headers are defined here,
 * except for those initialized in specific source files:
 *   Outchar, Pline, Putchar - defined in ex_put.c
 *   options - defined in ex_data.c
 *   endline, lastc, tfile - defined in ex_get.c / ex_temp.c
 *   direct, shell - defined in ex_set.c
 */

#include "ex.h"
#include "ex_argv.h"
#include "ex_re.h"
#include "ex_temp.h"
#include "ex_tty.h"
#include "ex_vis.h"

/* Actual variable definitions (correspond to extern declarations in headers) */

/* ex.h globals */
bool	aiflag;
bool	anymarks;
int	chng;
char	*Command;
int	defwind;
int	dirtcnt;
bool	dosusp;
bool	edited;
line	*endcore;
line	*fendcore;
char	file[FNSIZE];
char	genbuf[LBSIZE];
bool	hush;
char	*globp;
bool	holdcm;
bool	inappend;
bool	inglobal;
char	*initev;
bool	inopen;
char	*input;
bool	intty;
int	io;
bool	laste;
char	lastmac;
char	lasttag[TAGSIZE];
char	*linebp;
char	linebuf[LBSIZE];
bool	listf;
char	*loc1;
char	*loc2;
line	names['z'-'a'+2];
int	notecnt;
bool	numberf;
char	obuf[BUFSIZ];
int	oprompt;
int	ospeed;
int	otchng;
int	peekc;
char	*pkill[2];
bool	pfast;
int	pid;
int	ppid;
jmp_buf	resetlab;
int	rpid;
bool	ruptible;
bool	seenprompt;
bool	shudclob;
int	status;
int	tchng;
bool	vcatch;
jmp_buf	vreslab;
bool	writing;
int	xchng;
char	altfile[FNSIZE];
char	uxb[UXBSIZE + 2];
line	*addr1;
line	*addr2;
line	*dol;
line	*dot;
line	*one;
line	*truedol;
line	*unddol;
line	*zero;
int	undkind;
line	*unddel;
line	*undap1;
line	*undap2;
line	*undadot;
void	(*oldhup)();
void	(*oldquit)();

/* ex_tty.h globals */
bool	NONL;
bool	UPPERCASE;
int	OCOLUMNS;
int	outcol;
int	outline;
int	destcol;
int	destline;
struct	termios tty;
ttymode	normf;
bool	normtty;
int	costCM;
int	costSR;
int	costAL;
int	costDP;
int	costLP;
int	costRP;
int	costCE;
int	costCD;
struct maps arrows[MAXNOMACS];
struct maps immacs[MAXNOMACS];
struct maps abbrevs[MAXNOMACS];
int	ldisc;
char	mapspace[MAXCHARMACS];
char	*msnext;
int	maphopcnt;
bool	anyabbrs;
char	ttynbuf[20];
int	ttymesg;

/* ex_vis.h globals */
int	bastate;
int	state;
int	basWTOP;
int	basWLINES;
int	WTOP;
int	WBOT;
int	WLINES;
int	WCOLS;
int	WECHO;
bool	splitw;
struct vlinfo  vlinfo[TUBELINES + 2];
int	vcline;
int	vcnt;
char	*vtube[TUBELINES];
char	*vtube0;
char	*cursor;
char	*wcursor;
line	*wdot;
int	vundkind;
char	*vutmp;
int	vch_mac;
char	*vUNDcurs;
line	*vUNDdot;
line	vUNDsav;
int	hold;
int	holdupd;
int	CDCNT;
char	DEL[VBSIZE];
bool	HADUP;
bool	HADZERO;
char	INS[VBSIZE];
int	Vlines;
int	Xcnt;
bool	Xhadcnt;
int	ZERO;
int	dir;
int	doomed;
bool	gobblebl;
bool	hadcnt;
bool	heldech;
bool	insmode;
char	lastcmd[5];
int	lastcnt;
char	*lastcp;
bool	lasthad;
int	lastvgk;
int	lastreg;
char	*ncols['z'-'a'+2];
char	*notenam;
char	*notesgn;
char	op;
int	Peekkey;
bool	rubble;
int	vSCROLL;
char	*vglobp;
char	vmacbuf[VBSIZE];
char	*vmacp;
char	*vmcurs;
int	vmovcol;
bool	vmoving;
int	vreg;
int	wdkind;
char	workcmd[5];

/* ex_re.h globals */
struct	regexp re;
struct	regexp scanre;
struct	regexp subre;
char	*braslist[NBRA];
char	*braelist[NBRA];
char	rhsbuf[RHSSIZE];

/* ex_temp.h globals */
bool	hitin2;
bool	ichang2;
bool	ichanged;
int	iblock;
int	iblock2;
int	ninbuf;
int	nleft;
int	oblock;
int	tline;
char	ibuff[BUFSIZ];
char	ibuff2[BUFSIZ];
char	obuff[BUFSIZ];
struct 	header H;

/* ex_argv.h globals */
char	**argv;
char	**argv0;
char	*args;
char	*args0;
int	argc;
int	argc0;
int	morargc;
int	firstln;
char	*firstpat;
struct	glob frob;
