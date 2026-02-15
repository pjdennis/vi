/*
********************************************************************************
*                         Copyright (c) 1985 AT&T                              *
*                           All Rights Reserved                                *
*                                                                              *
*                                                                              *
*          THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T                 *
*        The copyright notice above does not evidence any actual               *
*        or intended publication of such source code.                          *
********************************************************************************
*/
/* Copyright (c) 1981 Regents of the University of California */
#include "ex.h"
#include "ex_tty.h"

int checkjunk(char c);

/*
 * Input routines for command mode.
 * Since we translate the end of reads into the implied ^D's
 * we have different flavors of routines which do/don't return such.
 */
static	bool junkbs;
int	lastc = '\n';

void
ignchar()
{
	ignore(getchar());
}

int
getchar()
{
	register int c;

	do
		c = getcd();
	while (!globp && c == CTRL('d'));
	return (c);
}

int
getcd()
{
	register int c;
	extern short slevel;

again:
	c = getach();
	if (c == EOF)
		return (c);
	c &= TRIM;
	if (!inopen && slevel==0)
		if (!globp && c == CTRL('d'))
			setlastchar('\n');
		else if (junk(c)) {
			checkjunk(c);
			goto again;
		}
	return (c);
}

int
peekchar()
{

	if (peekc == 0)
		peekc = getchar();
	return (peekc);
}

int
peekcd()
{
	if (peekc == 0)
		peekc = getcd();
	return (peekc);
}

int verbose;
int
getach()
{
	register int c;
	static char inputline[128];
	struct stat statb;

	c = peekc;
	if (c != 0) {
		peekc = 0;
		return (c);
	}
	if (globp) {
		if (*globp)
			return (*globp++);
		globp = 0;
		return (lastc = EOF);
	}
top:
	if (input) {
		if (c = *input++) {
			if (c &= TRIM)
				return (lastc = c);
			goto top;
		}
		input = 0;
	}
	flush();
	if (intty) {
		c = read(0, inputline, sizeof inputline - 4);
		if (c < 0)
			return (lastc = EOF);
		if (c == 0 || inputline[c-1] != '\n')
			inputline[c++] = CTRL('d');
		if (inputline[c-1] == '\n')
			noteinp();
		inputline[c] = 0;
		for (c--; c >= 0; c--)
			if (inputline[c] == 0)
				inputline[c] = QUOTE;
		input = inputline;
		goto top;
	}
	if (read(0, inputline, 1) != 1)
		lastc = EOF;
	else {
		lastc = inputline[0];
		if (verbose)
			write(2, inputline, 1);
	}
	return (lastc);
}

/*
 * Input routine for insert/append/change in command mode.
 * Most work here is in handling autoindent.
 */
static	short	lastin;

int
gettty()
{
	register int c = 0;
	register char *cp = genbuf;
	char hadup = 0;
	int numbline();
	extern int (*Pline)(int);
	int offset = Pline == numbline ? 8 : 0;
	int ch;

	if (intty && !inglobal) {
		if (offset) {
			holdcm = 1;
			printf("  %4d  ", lineDOT() + 1);
			flush();
			holdcm = 0;
		}
		if (value(AUTOINDENT) ^ aiflag) {
			holdcm = 1;
			if (value(LISP))
				lastin = lindent(dot + 1);
			gotab(lastin + offset);
			while ((c = getcd()) == CTRL('d')) {
				if (lastin == 0 && isatty(0) == -1) {
					holdcm = 0;
					return (EOF);
				}
				lastin = backtab(lastin);
				gotab(lastin + offset);
			}
			switch (c) {

			case '^':
			case '0':
				ch = getcd();
				if (ch == CTRL('d')) {
					if (c == '0')
						lastin = 0;
					if (!over_strike) {
						putchar('\b' | QUOTE);
						putchar(' ' | QUOTE);
						putchar('\b' | QUOTE);
					}
					gotab(offset);
					hadup = 1;
					c = getchar();
				} else
					ungetchar(ch);
				break;

			case '.':
				if (peekchar() == '\n') {
					ignchar();
					noteinp();
					holdcm = 0;
					return (EOF);
				}
				break;

			case '\n':
				hadup = 1;
				break;
			}
		}
		flush();
		holdcm = 0;
	}
	if (c == 0)
		c = getchar();
	while (c != EOF && c != '\n') {
		if (cp > &genbuf[LBSIZE - 2])
			error("Input line too long");
		*cp++ = c;
		c = getchar();
	}
	if (c == EOF) {
		if (inglobal)
			ungetchar(EOF);
		return (EOF);
	}
	*cp = 0;
	cp = linebuf;
	if ((value(AUTOINDENT) ^ aiflag) && hadup == 0 && intty && !inglobal) {
		lastin = c = smunch(lastin, genbuf);
		for (c = lastin; c >= value(TABSTOP); c -= value(TABSTOP))
			*cp++ = '\t';
		for (; c > 0; c--)
			*cp++ = ' ';
	}
	CP(cp, genbuf);
	if (linebuf[0] == '.' && linebuf[1] == 0)
		return (EOF);
	return (0);
}

/*
 * Crunch the indent.
 * Hard thing here is that in command mode some of the indent
 * is only implicit, so we must seed the column counter.
 * This should really be done differently so as to use the whitecnt routine
 * and also to hack indenting for LISP.
 */
int
smunch(int col, char *ocp)
{
	register char *cp;

	cp = ocp;
	for (;;)
		switch (*cp++) {

		case ' ':
			col++;
			continue;

		case '\t':
			col += value(TABSTOP) - (col % value(TABSTOP));
			continue;

		default:
			cp--;
			CP(ocp, cp);
			return (col);
		}
}

char	*cntrlhm =	"^H discarded\n";

int
checkjunk(char c)
{

	if (junkbs == 0 && c == '\b') {
		write(2, cntrlhm, 13);
		junkbs = 1;
	}
}

line *
setin(line *addr)
{

	if (addr == zero)
		lastin = 0;
	else
		getline(*addr), lastin = smunch(0, linebuf);
}
