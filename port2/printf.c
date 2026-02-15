#include <stdarg.h>

/*
 * This version of printf is compatible with the Version 7 C
 * printf. It prints through the editor's putchar, not stdout.
 * Renamed from printf to ex_printf to avoid shadowing the
 * standard library function.
 */
extern int putchar(int);


#define MAXOCT	11	/* Maximum octal digits in a long */
#define MAXINT	32767	/* largest normal length positive integer */
#define BIG	1000000000  /* largest power of 10 less than an unsigned long */
#define MAXDIGS	10	/* number of digits in BIG */

static int width, sign, fill;

static char *_p_dconv(long value, char *buffer);
static void _p_emit(char *s, char *send);

void
ex_printf(char *fmt, ...)
{
	va_list ap;
	char fcode;
	int prec;
	int length,mask1,nbits,n;
	long int mask2, num;
	char *bptr;
	char *ptr;
	char buf[134];

	va_start(ap, fmt);
	for (;;) {
		/* process format string first */
		while ((fcode = *fmt++)!='%') {
			/* ordinary (non-%) character */
			if (fcode=='\0') {
				va_end(ap);
				return;
			}
			putchar(fcode);
		}
		/* length modifier: -1 for h, 1 for l, 0 for none */
		length = 0;
		/* check for a leading - sign */
		sign = 0;
		if (*fmt == '-') {
			sign++;
			fmt++;
		}
		/* a '0' may follow the - sign */
		/* this is the requested fill character */
		fill = 1;
		if (*fmt == '0') {
			fill--;
			fmt++;
		}

		/* Now comes a digit string which may be a '*' */
		if (*fmt == '*') {
			width = va_arg(ap, int);
			if (width < 0) {
				width = -width;
				sign = !sign;
			}
			fmt++;
		}
		else {
			width = 0;
			while (*fmt>='0' && *fmt<='9')
				width = width * 10 + (*fmt++ - '0');
		}

		/* maybe a decimal point followed by more digits (or '*') */
		if (*fmt=='.') {
			if (*++fmt == '*') {
				prec = va_arg(ap, int);
				fmt++;
			}
			else {
				prec = 0;
				while (*fmt>='0' && *fmt<='9')
					prec = prec * 10 + (*fmt++ - '0');
			}
		}
		else
			prec = -1;

		switch (*fmt) {
			case 'L':
			case 'l':
				length = 2;
				/* FALLTHROUGH */
			case 'h':
			case 'H':
				length--;
				fmt++;
				break;
		}

		switch (fcode = *fmt++) {
			/* process characters and strings first */
			case 'c':
				buf[0] = va_arg(ap, int);
				ptr = bptr = &buf[0];
				if (buf[0] != '\0')
					ptr++;
				break;
			case 's':
				bptr = va_arg(ap,char *);
				if (bptr==0)
					bptr = "(null pointer)";
				if (prec < 0)
					prec = MAXINT;
				for (n=0; *bptr++ && n < prec; n++) ;
				ptr = --bptr;
				bptr -= n;
				break;
			case 'O':
				length = 1;
				fcode = 'o';
				/* FALLTHROUGH */
			case 'o':
			case 'X':
			case 'x':
				if (length > 0)
					num = va_arg(ap,long);
				else
					num = (unsigned)va_arg(ap,int);
				if (fcode=='o') {
					mask1 = 0x7;
					mask2 = 0x1fffffffL;
					nbits = 3;
				}
				else {
					mask1 = 0xf;
					mask2 = 0x0fffffffL;
					nbits = 4;
				}
				n = (num!=0);
				bptr = buf + MAXOCT + 3;
				/* shift and mask for speed */
				do
				    if (((int) num & mask1) < 10)
					*--bptr = ((int) num & mask1) + 060;
				    else
					*--bptr = ((int) num & mask1) + 0127;
				while ((num = (num >> nbits) & mask2));

				if (fcode=='o') {
					if (n)
						*--bptr = '0';
				}
				else
					if (!sign && fill <= 0) {
						putchar('0');
						putchar(fcode);
						width -= 2;
					}
					else {
						*--bptr = fcode;
						*--bptr = '0';
					}
				ptr = buf + MAXOCT + 3;
				break;
			case 'D':
			case 'U':
			case 'I':
				length = 1;
				fcode = fcode + 'a' - 'A';
				/* FALLTHROUGH */
			case 'd':
			case 'i':
			case 'u':
				if (length > 0)
					num = va_arg(ap,long);
				else {
					n = va_arg(ap,int);
					if (fcode=='u')
						num = (unsigned) n;
					else
						num = (long) n;
				}
				if ((n = (fcode != 'u' && num < 0)))
					num = -num;
				/* now convert to digits */
				bptr = _p_dconv(num, buf);
				if (n)
					*--bptr = '-';
				if (fill == 0)
					fill = -1;
				ptr = buf + MAXDIGS + 1;
				break;
			default:
				/* not a control character,
				 * print it.
				 */
				ptr = bptr = &fcode;
				ptr++;
				break;
			}
			if (fcode != '\0')
				_p_emit(bptr,ptr);
	}
	va_end(ap);
}

/* _p_dconv converts the unsigned long integer "value" to
 * printable decimal and places it in "buffer", right-justified.
 */
static char *
_p_dconv(long value, char *buffer)
{
	char *bp;
	int svalue;
	int n;
	long lval;

	bp = buffer;

	/* zero is a special case */
	if (value == 0) {
		bp += MAXDIGS;
		*bp = '0';
		return(bp);
	}

	/* develop the leading digit of the value in "n" */
	n = 0;
	while (value < 0) {
		value -= BIG;	/* will eventually underflow */
		n++;
	}
	while ((lval = value - BIG) >= 0) {
		value = lval;
		n++;
	}

	/* stash it in buffer[1] to allow for a sign */
	bp[1] = n + '0';
	bp += MAXDIGS + 1;
	while (value > MAXINT) {
		*--bp = (int)(value % 10) + '0';
		value /= 10;
	}

	/* cannot lose precision */
	svalue = value;
	while (svalue > 0) {
		*--bp = (svalue % 10) + '0';
		svalue /= 10;
	}

	/* fill in intermediate zeroes if needed */
	if (buffer[1] != '0') {
		while (bp > buffer + 2)
			*--bp = '0';
		--bp;
	}
	return(bp);
}

/*
 * This program sends string "s" to putchar.
 */
static void
_p_emit(char *s, char *send)
{
	char cfill;
	int alen;
	int npad;

	alen = send - s;
	if (alen > width)
		width = alen;
	cfill = fill>0? ' ': '0';

	/* we may want to print a leading '-' before anything */
	if (*s == '-' && fill < 0) {
		putchar(*s++);
		alen--;
		width--;
	}
	npad = width - alen;

	/* emit any leading pad characters */
	if (!sign)
		while (--npad >= 0)
			putchar(cfill);

	/* emit the string itself */
	while (--alen >= 0)
		putchar(*s++);

	/* emit trailing pad characters */
	if (sign)
		while (--npad >= 0)
			putchar(cfill);
}
