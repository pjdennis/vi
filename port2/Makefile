# Makefile for vi editor — clean C11 port
# Target: POSIX/Linux, VT100/ANSI terminal, ASCII

CC = gcc
CFLAGS = -std=c11 \
	-DVMUNIX -DUSG -DTABS=8 -DSIGTSTP \
	-fsigned-char \
	-Wall -Wextra -Wpedantic -Wshadow -Werror \
	-g

SRCS = ex.c ex_addr.c ex_cmds.c ex_cmds2.c ex_cmdsub.c \
	ex_data.c ex_extern.c ex_get.c ex_io.c ex_put.c \
	ex_re.c ex_set.c ex_subr.c ex_temp.c ex_tty.c ex_unix.c \
	ex_v.c ex_vadj.c ex_vget.c ex_vmain.c ex_voper.c \
	ex_vops.c ex_vops2.c ex_vops3.c ex_vput.c ex_vwind.c \
	printf.c vi_term.c vi_mem.c

OBJS = $(SRCS:.c=.o)

all: vi

vi: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Header dependencies
$(OBJS): ex.h ex_tune.h ex_vars.h vi_term.h

ex_extern.o: ex_argv.h ex_re.h ex_temp.h ex_tty.h ex_vis.h
ex_data.o: ex_tty.h
ex.o: ex_argv.h ex_temp.h ex_tty.h
ex_tty.o: ex_tty.h
ex_subr.o: ex_re.h ex_tty.h ex_vis.h
ex_put.o: ex_tty.h ex_vis.h
ex_temp.o: ex_temp.h ex_vis.h ex_tty.h
ex_get.o: ex_tty.h
ex_io.o: ex_argv.h ex_temp.h ex_tty.h ex_vis.h
ex_cmds.o: ex_argv.h ex_temp.h ex_tty.h ex_vis.h
ex_cmds2.o: ex_argv.h ex_temp.h ex_tty.h ex_vis.h
ex_cmdsub.o: ex_argv.h ex_temp.h ex_tty.h ex_vis.h
ex_addr.o: ex_re.h
ex_re.o: ex_re.h ex_tty.h ex_vis.h
ex_set.o: ex_temp.h ex_tty.h
ex_unix.o: ex_temp.h ex_tty.h ex_vis.h
ex_v.o: ex_tty.h ex_vis.h
ex_vadj.o: ex_tty.h ex_vis.h
ex_vget.o: ex_tty.h ex_vis.h
ex_vmain.o: ex_tty.h ex_vis.h
ex_voper.o: ex_tty.h ex_vis.h
ex_vops.o: ex_tty.h ex_vis.h
ex_vops2.o: ex_tty.h ex_vis.h
ex_vops3.o: ex_tty.h ex_vis.h
ex_vput.o: ex_tty.h ex_vis.h
ex_vwind.o: ex_tty.h ex_vis.h

clean:
	rm -f vi $(OBJS)

.PHONY: all clean
