/* Copyright (c) 1981 Regents of the University of California */
/* sccs id:	"@(#)e.h	1.1"		(9.3	3/17/83) */

/*
 * Ex version 3 (see exact version in ex_cmds.c, search for /Version/)
 *
 * Mark Horton, U.C. Berkeley
 * Bill Joy, U.C. Berkeley
 * November 1979
 */

#define _DEFAULT_SOURCE
#define getline __posix_getline
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/stat.h>

#include <stdint.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#define bcopy(from, to, n) memmove((to), (from), (n))
#define VFORK fork

/*
 * Terminal capability interface.
 */
#include "vi_term.h"


typedef	int	line;
#include <stdbool.h>

#include "ex_tune.h"
#include "ex_vars.h"
#include "ex_tty.h"

/*
 * Options in the editor are referred to usually by "value(name)" where
 * name is all uppercase, i.e. "value(PROMPT)".
 */
struct	option {
	char	*oname;
	char	*oabbrev;
	int	otype;		/* Types -- see below */
	int	odefault;	/* Default value */
	int	ovalue;		/* Current value */
	char	*osvalue;
};

#define	ONOFF	0
#define	NUMERIC	1
#define	STRING	2		/* SHELL or DIRECTORY */
#define	OTERM	3

#define	value(a)	options[a].ovalue
#define	svalue(a)	options[a].osvalue

extern	 struct	option options[NOPTS + 1];


/*
 * The editor does not normally use the standard i/o library.
 */
#include <stdio.h>
#undef putchar
#undef getchar
#undef NULL
#define	NULL	0
#undef	EOF
#define	EOF	-1

#undef getline

/*
 * Character constants and bits
 *
 * The editor uses the QUOTE bit as a flag to pass on with characters.
 * The editor never uses a simple char variable.
 */
#define	QUOTE	0200
#define	TRIM	0177
#undef	CTRL
#define	CTRL(c)	((c) & 037)
#define	NL	CTRL('j')
#define	CR	CTRL('m')
#define	DELETE	0177		/* See also ATTN, QUIT in ex_tune.h */
#define	ESCAPE	033

/*
 * Miscellaneous random variables used in more than one place
 */
extern	bool	aiflag;		/* Append/change/insert with autoindent */
extern	bool	anymarks;	/* We have used '[a-z] */
extern	int	chng;		/* Warn "No write" */
extern	char	*Command;
extern	int	defwind;	/* -w# change default window size */
extern	int	dirtcnt;	/* When >= MAXDIRT, should sync temporary */
extern	bool	dosusp;		/* Do SIGTSTP in visual when ^Z typed */
extern	bool	edited;		/* Current file is [Edited] */
extern	line	*endcore;	/* Last available core location */
extern	 bool	endline;	/* Last cmd mode command ended with \n */
/* var short erfile; */
extern	line	*fendcore;	/* First address in line pointer space */
extern	char	file[FNSIZE];	/* Working file name */
extern	char	genbuf[LBSIZE];	/* Working buffer when manipulating linebuf */
extern	bool	hush;		/* Command line option - was given, hush up! */
extern	char	*globp;		/* (Untyped) input string to command mode */
extern	bool	holdcm;		/* Don't cursor address */
extern	bool	inappend;	/* in ex command append mode */
extern	int	inglobal;	/* Inside g//... or v//... */
extern	char	*initev;	/* Initial : escape for visual */
extern	int	inopen;		/* Inside open or visual */
extern	char	*input;		/* Current position in cmd line input buffer */
extern	bool	intty;		/* Input is a tty */
extern	int	io;		/* General i/o unit (auto-closed on error!) */
extern	 int	lastc;		/* Last character ret'd from cmd input */
extern	bool	laste;		/* Last command was an "e" (or "rec") */
extern	char	lastmac;	/* Last macro called for ** */
extern	char	lasttag[TAGSIZE];	/* Last argument to a tag command */
extern	char	*linebp;	/* Used in substituting in \n */
extern	char	linebuf[LBSIZE];	/* The primary line buffer */
extern	bool	listf;		/* Command should run in list mode */
extern	char	*loc1;		/* Where re began to match (in linebuf) */
extern	char	*loc2;		/* First char after re match (") */
extern	line	names['z'-'a'+2];	/* Mark registers a-z,' */
extern	int	notecnt;	/* Count for notify (to visual from cmd) */
extern	bool	numberf;	/* Command should run in number mode */
extern	char	obuf[BUFSIZ];	/* Buffer for tty output */
extern	int	oprompt;	/* Saved during source */
extern	int	otchng;		/* Backup tchng to find changes in macros */
extern	int	peekc;		/* Peek ahead character (cmd mode input) */
extern	char	*pkill[2];	/* Trim for put with ragged (LISP) delete */
extern	int	pfast;		/* Have stty -nl'ed to go faster */
extern	int	pid;		/* Process id of child */
extern	int	ppid;		/* Process id of parent (e.g. main ex proc) */
extern	sigjmp_buf	resetlab;	/* For error throws to top level (cmd mode) */
extern	int	rpid;		/* Pid returned from wait() */
extern	bool	ruptible;	/* Interruptible is normal state */
extern	bool	seenprompt;	/* 1 if have gotten user input */
extern	bool	shudclob;	/* Have a prompt to clobber (e.g. on ^D) */
extern	int	status;		/* Status returned from wait() */
extern	int	tchng;		/* If nonzero, then [Modified] */
extern	int	tfile;		/* Temporary file unit */
extern	bool	vcatch;		/* Want to catch an error (open/visual) */
extern	sigjmp_buf	vreslab;	/* For error throws to a visual catch */
extern	bool	writing;	/* 1 if in middle of a file write */
extern	int	xchng;		/* Suppresses multiple "No writes" in !cmd */

extern	char	termtype[];	/* Terminal type from strstrstrstrstrstring capability */
extern	bool	pflag, nflag;	/* Print/number flags for commands */
extern	int	poffset;	/* Print line number offset */
extern	int	isalt;		/* Current file is alternate */
extern	int	slevel;		/* Source nesting level */
extern	int	ttyindes;	/* TTY input descriptor */
extern	int	verbose;	/* Verbose option (ex -V) */
extern	void	oncore(int);	/* Core dump signal handler */

/*
 * Macros
 */
#define	CP(a, b)	(ignore(strcpy(a, b)))
			/*
			 * FIXUNDO: do we want to mung undo vars?
			 * Usually yes unless in a macro or global.
			 */
#define FIXUNDO		(inopen >= 0 && (inopen || !inglobal))
#define ckaw()		{if (chng && value(AUTOWRITE) && !value(READONLY)) wop(0);}
#define	copy(a,b,c)	Copy((char *) (a), (char *) (b), (c))
#define	eq(a, b)	((a) && (b) && strcmp(a, b) == 0)
#define	getexit(a)	copy(a, resetlab, sizeof (sigjmp_buf))
#define	lastchar()	lastc
#define	outchar(c)	(*Outchar)(c)
#define	pastwh()	(ignore(skipwh()))
#define	pline(no)	(*Pline)(no)
#define	reset()		siglongjmp(resetlab,1)
#define	resexit(a)	copy(resetlab, a, sizeof (sigjmp_buf))
#define	setexit()	sigsetjmp(resetlab, 1)
#define	setlastchar(c)	lastc = c
#define	ungetchar(c)	peekc = c

#define	CATCH		vcatch = 1; if (sigsetjmp(vreslab, 1) == 0) {
#define	ONERR		} else { vcatch = 0;
#define	ENDCATCH	} vcatch = 0;

/*
 * Environment like memory
 */
extern	char	altfile[FNSIZE];	/* Alternate file name */
extern	char	direct[ONMSZ];		/* Temp file goes here */
extern	char	shell[ONMSZ];		/* Copied to be settable */
extern	char	uxb[UXBSIZE + 2];	/* Last !command for !! */

/*
 * The editor data structure for accessing the current file consists
 * of an incore array of pointers into the temporary file tfile.
 */
extern	line	*addr1;			/* First addressed line in a command */
extern	line	*addr2;			/* Second addressed line */
extern	line	*dol;			/* Last line in buffer */
extern	line	*dot;			/* Current line */
extern	line	*one;			/* First line */
extern	line	*truedol;		/* End of all lines, including saves */
extern	line	*unddol;		/* End of undo saved lines */
extern	line	*zero;			/* Points to empty slot before one */

/*
 * Undo information
 */
extern	int	undkind;

extern	line	*unddel;	/* Saved deleted lines go after here */
extern	line	*undap1;	/* Beginning of new lines */
extern	line	*undap2;	/* New lines end before undap2 */
extern	line	*undadot;	/* If we saved all lines, dot reverts here */

#define	UNDCHANGE	0
#define	UNDMOVE		1
#define	UNDALL		2
#define	UNDNONE		3
#define	UNDPUT		4

/*
 * Function type definitions
 */
#define	NOSTR	(char *) 0
#define	NOLINE	(line *) 0

extern	int	(*Outchar)(int);
extern	int	(*Pline)(int);
extern	int	(*Putchar)(int);
extern	void	(*oldhup)(int);
extern	void	(*oldquit)(int);

/*
 * Function prototypes - ex.c
 */
void	init(void);
char	*tailpath(char *p);

/*
 * Function prototypes - ex_addr.c
 */
void	setdot(void);
void	setdot1(void);
void	setcount(void);
int	getnum(void);
void	setall(void);
void	setnoaddr(void);
line	*address(char *inputline);
void	setCNL(void);
void	setNAEOL(void);

/*
 * Function prototypes - ex_cmds.c
 */
void	commands(bool noprompt, bool exitoneof);

/*
 * Function prototypes - ex_cmds2.c
 */
int	cmdreg(void);
int	endcmd(int ch);
void	eol(void);
void	error(char *str, ...);
void	erewind(void);
void	error0(void);
void	error1(char *str);
void	fixol(void);
int	exclam(void);
void	makargs(void);
void	next(void);
void	donewline(void);
void	nomore(void);
int	quickly(void);
void	resetflav(void);
void	serror(char *str, char *cp);
void	setflav(void);
int	skipend(void);
void	tailspec(int c);
void	tail(char *comm);
void	tail2of(char *comm);
int	tailprim(char *comm, int i, bool notinvis);
void	vcontin(bool ask);
void	vnfl(void);

/*
 * Function prototypes - ex_cmdsub.c
 */
int	append(int (*f)(void), line *a);
void	appendnone(void);
void	pargs(void);
void	delete(int hush);
void	deletenone(void);
void	squish(void);
void	join(int c);
void	move(void);
void	move1(int cflag, line *addrt);
void	put(int unused);
void	pragged(bool kill);
void	shift(int c, int cnt);
void	tagfind(bool quick);
void	yank(int unused);
void	zop(int hadpr);
void	zop2(int nlines, int op);
void	plines(line *adr1, line *adr2, bool movedot);
void	pofix(void);
void	undo(bool c);
void	somechange(void);
void	mapcmd(int un, int ab);
void	addmac(char *src, char *dest, char *dname, struct maps *mp);
void	cmdmac(char c);
char	*vgetpass(char *prompt);

/*
 * Function prototypes - ex_get.c
 */
void	ignchar(void);
int	getchar(void);
int	getcd(void);
int	peekchar(void);
int	peekcd(void);
int	getach(void);
int	gettty(void);
int	smunch(int col, char *ocp);
line	*setin(line *addr);

/*
 * Function prototypes - ex_io.c
 */
struct	glob;	/* Forward declaration (defined in ex_argv.h) */
void	filename(int comm);
int	getargs(void);
void	glob(struct glob *gp);
void	getone(void);
void	rop(int c);
void	rop2(void);
void	rop3(int c);
int	wop(bool dofname);
int	getfile(void);
void	putfile(int isfilter);
void	source(char *fil, bool okfail);
int	iostats(void);
void	clrstats(void);

/*
 * Function prototypes - ex_put.c
 */
int	(*setlist(bool t))(int);
int	(*setnorm(void))(int);
int	(*setnumb(bool t))(int);
int	listchar(int c);
int	normchar(int c);
int	numbline(int i);
int	normline(int);
void	slobber(int c);
int	putchar(int c);
int	termchar(int c);
void	flush(void);
void	flush1(void);
void	flush2(void);
void	fgoto(void);
void	gotab(int col);
int	plod(int cnt);
void	noteinp(void);
void	termreset(void);
void	draino(void);
void	flusho(void);
void	putnl(void);
void	putS(char *cp);
int	putch(int c);
void	putpad(char *cp);
void	setoutt(void);
void	lprintf(char *cp, char *dp);
void	putNFL(void);
void	pstart(void);
void	pstop(void);
ttymode	ostart(void);
void	tostart(void);
void	ttcharoff(void);
void	ostop(ttymode f);
void	tostop(void);
void	normal(ttymode f);
ttymode	setty(ttymode f);
void	gTTY(int i);
void	sTTY(int i);
void	noonl(void);

/*
 * Function prototypes - ex_re.c
 */
void	global(bool k);
int	substitute(int c);
int	compsub(int ch);
int	getsub(void);
int	compile(int eof, int oknl);
int	execute(int gf, ...);
int	advance(char *lp, char *ep);
char	*place(char *sp, char *l1, char *l2);

/*
 * Function prototypes - ex_set.c
 */
void	set(void);

/*
 * Function prototypes - ex_subr.c
 */
int	any(int c, char *s);
int	backtab(int i);
void	change(void);
int	column(char *cp);
void	comment(void);
void	Copy(char *to, char *from, int size);
void	copyw(line *to, line *from, int size);
void	copywR(line *to, line *from, int size);
int	ctlof(int c);
void	dingdong(void);
int	fixindent(int indent);
void	filioerr(char *cp);
char	*genindent(int indent);
void	getDOT(void);
line	*getmark(int c);
int	getn(char *cp);
void	ignnEOF(void);
int	iswhite(int c);
int	junk(int c);
void	killed(void);
void	killcnt(int cnt);
int	lineno(line *a);
int	lineDOL(void);
int	lineDOT(void);
void	markDOT(void);
void	markpr(line *which);
int	markreg(int c);
char	*mesg(char *str);
void	merror(char *seekpt, ...);
void	merror1(char *seekpt);
int	morelines(void);
void	nonzero(void);
int	notable(int i);
void	notempty(void);
void	netchHAD(int cnt);
void	netchange(int i);
void	putmark(line *addr);
void	putmk1(line *addr, int n);
char	*plural(long i);
int	qcolumn(char *lim, char *gp);
int	qcount(int c);
void	reverse(line *a1, line *a2);
void	save(line *a1, line *a2);
void	save12(void);
void	saveall(void);
int	span(void);
void	vi_sync(void);
int	skipwh(void);
void	smerror(char *seekpt, char *cp);
char	*strend(char *cp);
void	strcLIN(char *dp);
void	syserror(int danger);
int	tabcol(int col, int ts);
char	*vfindcol(int i);
char	*vskipwh(char *cp);
char	*vpastwh(char *cp);
int	whitecnt(char *cp);
void	markit(line *addr);
void	onhup(int);
void	onintr(int);
void	setrupt(void);
int	preserve(void);
void	onsusp(int);
void	vi_signal(int sig, void (*handler)(int));

/*
 * Function prototypes - ex_temp.c
 */
void	fileinit(void);
int	cleanup(bool all);
void	getline(line tl);
int	putline(void);
char	*getblock(line atl, int iof);
void	blkio(short b, char *buf, ssize_t (*iofcn)(int, void *, size_t));
void	tlaste(void);
void	tflush(void);
void	synctmp(void);
void	TSYNC(void);
void	putreg(int c);
int	partreg(char c);
void	notpart(int c);
void	YANKreg(int c);
void	kshift(void);
void	YANKline(void);
void	rbflush(void);
void	regbuf(char c, char *buf, int buflen);

/*
 * Function prototypes - ex_tty.c
 */
void	gettmode(void);
void	setterm(char *type);
char	*fkey(int i);
int	cost(char *str);

/*
 * Function prototypes - ex_unix.c
 */
void	unix0(bool warn);
ttymode	unixex(char *opt, char *up, int newstdin, int mode);
void	unixwt(bool c, ttymode f);
void	filter(int mode);
void	recover(void);
void	waitfor(void);
void	revocer(void);

/*
 * Function prototypes - ex_v.c
 */
void	oop(void);
void	ovbeg(void);
void	ovend(ttymode f);
void	vop(void);
void	fixzero(void);
void	savevis(void);
void	undvis(void);
void	setwind(void);
void	vok(char *atube);
void	vintr(int);
void	vsetsiz(int size);

/*
 * Function prototypes - ex_vadj.c
 */
void	vopen(line *tp, int p);
int	vreopen(int p, int lineno, int l);
int	vglitchup(int l, int o);
void	vinslin(int p, int cnt, int l);
void	vopenup(int cnt, bool could, int l);
void	vadjAL(int p, int cnt);
void	vrollup(int dl);
void	vup1(void);
void	vmoveitup(int cnt, bool doclr);
void	vscroll(int cnt);
void	vscrap(void);
void	vrepaint(char *curs);
void	vredraw(int p);
void	vdellin(int p, int cnt, int l);
void	vadjDL(int p, int cnt);
void	vsyncCL(void);
void	vsync(int p);
void	vsync1(int p);
void	vcloseup(int l, int cnt);
void	vreplace(int l, int cnt, int newcnt);
void	sethard(void);
void	vdirty(int base, int i);

/*
 * Function prototypes - ex_vget.c
 */
void	ungetkey(int c);
int	getkey(void);
int	peekbr(void);
int	getbr(void);
int	getesc(void);
int	peekkey(void);
int	readecho(char c);
void	setLAST(void);
void	addtext(char *cp);
void	setDEL(void);
void	setBUF(char *BUF);
void	addto(char *buf, char *str);
int	noteit(bool must);
void	beep(void);
int	map(int c, struct maps *maps);
void	macpush(char *st, int canundo);
int	vgetcnt(void);
int	fastpeekkey(void);
void	setalarm(void);
void	cancelalarm(void);

/*
 * Function prototypes - ex_vmain.c
 */
void	vmain(void);
void	grabtag(void);
void	prepapp(void);
void	vremote(int cnt, void (*f)(int), int arg);
void	vsave(void);
void	vzop(bool hadcnt, int cnt, int c);

/*
 * Function prototypes - ex_voper.c
 */
void	operate(int c, int cnt);
int	find(char c);
int	word(void (*op)(int), int cnt);
void	eend(void (*op)(int));
int	wordof(char which, char *wc);
int	wordch(char *wc);
int	edge(void);
int	margin(void);

/*
 * Function prototypes - ex_vops.c
 */
void	vUndo(void);
void	vundo(bool show);
void	vmacchng(bool fromvis);
void	vnoapp(void);
void	vmove(int c);
void	vdelete(int c);
void	vchange(int c);
void	voOpen(int c, int cnt);
void	vshftop(int c);
void	vfilter(int c);
int	xdw(void);
void	vshift(int unused);
void	vrep(int cnt);
void	vyankit(int c);
void	setpk(void);

/*
 * Function prototypes - ex_vops2.c
 */
void	bleep(int i, char *cp);
int	vdcMID(void);
void	takeout(char *BUF);
int	ateopr(void);
void	vappend(int ch, int cnt, int indent);
void	back1(void);
char	*vgetline(int cnt, char *gcursor, int *aescaped, char commch);
void	vdoappend(char *lp);

/*
 * Function prototypes - ex_vops3.c
 */
int	lfind(int pastatom, int cnt, void (*f)(int), line *limit);
int	endsent(bool pastatom);
int	endPS(void);
int	lindent(line *addr);
int	lmatchp(line *addr);
void	lsmatch(char *cp);
int	ltosolid(void);
int	ltosol1(char *parens);
int	lskipbal(char *parens);
int	lskipatom(void);
int	lskipa1(char *parens);
int	lnext(void);
int	lbrack(int c, void (*f)(int));
int	isa(char *cp);

/*
 * Function prototypes - ex_vput.c
 */
void	vclear(void);
void	vclrbyte(char *cp, int i);
void	vclrlin(int l, line *tp);
void	vclreol(void);
void	vclrech(bool didphys);
void	fixech(void);
void	vcursbef(char *cp);
void	vcursat(char *cp);
void	vcursaft(char *cp);
void	vfixcurs(void);
void	vsetcurs(char *nc);
void	vigoto(int y, int x);
void	vcsync(void);
void	vgotoCL(int x);
void	vshowmode(char *msg);
void	vgoto(int y, int x);
void	vprepins(void);
void	vmaktop(int p, char *cp);
int	vinschar(int c);
void	vneedpos(int cnt);
void	vnpins(int dosync);
void	vishft(void);
void	viin(int c);
void	godm(void);
void	enddm(void);
void	goim(void);
void	endim(void);
int	vputchar(int c);
void	physdc(int stcol, int endcol);
int	vputch(int);

/*
 * Function prototypes - ex_vwind.c
 */
void	vmoveto(line *addr, char *curs, char context);
void	vjumpto(line *addr, char *curs, char context);
void	vupdown(int cnt, char *curs);
void	vup(int cnt, int ind, bool scroll);
void	vdown(int cnt, int ind, bool scroll);
void	vcontext(line *addr, char where);
void	vclean(void);
void	vshow(line *addr, line *top);
void	vreset(bool inecho);
line	*vback(line *tp, int cnt);
int	vfit(line *tp, int cnt);
void	vroll(int cnt);
void	vrollR(int cnt);
int	vdepth(void);
void	vnline(char *curs);

#	define	ignore(a)	a
#	define	ignorf(a)	a

#include "vi_mem.h"
