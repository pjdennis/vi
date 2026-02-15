/*
 * vi_mem.c - Memory management for line pointer array
 *
 * Replaces sbrk()/brk() with malloc()/realloc().
 * The original vi grew the data segment to hold line pointers.
 * We allocate a dynamic array and realloc as needed.
 */
/* Need ex.h for line type and global variables, but ex.h includes us.
 * We break the circularity by including the minimum needed. */
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* Forward-declare what we need from ex.h */
typedef int line;
extern line *zero, *one, *dot, *dol, *truedol, *unddol;
extern line *addr1, *addr2;
extern line *undap1, *undap2, *unddel, *undadot;
extern line *fendcore, *endcore;
extern line *wdot, *vUNDdot;

/* Initial size: 8K lines */
#define INIT_LINES	8192
/* Growth increment: 1024 lines */
#define GROW_LINES	1024

static line *line_array = NULL;
static int line_array_size = 0;

/*
 * Adjust a line pointer after realloc moves the array.
 */
#define ADJUST(ptr, delta) \
	do { if (ptr) ptr = (line *)((char *)(ptr) + (delta)); } while(0)

void
vi_mem_init(void)
{
	line_array = (line *)malloc(INIT_LINES * sizeof(line));
	if (line_array == NULL) {
		write(2, "vi: out of memory\n", 18);
		_exit(1);
	}
	memset(line_array, 0, INIT_LINES * sizeof(line));
	line_array_size = INIT_LINES;

	fendcore = line_array;
	endcore = line_array + line_array_size - 2;
}

int
vi_morelines(void)
{
	line *old_array = line_array;
	int new_size = line_array_size + GROW_LINES;
	line *new_array;
	ptrdiff_t delta;

	new_array = (line *)realloc(line_array, new_size * sizeof(line));
	if (new_array == NULL)
		return -1;

	/* Clear newly allocated space */
	memset(new_array + line_array_size, 0, GROW_LINES * sizeof(line));

	delta = (char *)new_array - (char *)old_array;
	line_array = new_array;
	line_array_size = new_size;

	/* If the block moved, adjust all global line* pointers */
	if (delta != 0) {
		ADJUST(zero, delta);
		ADJUST(one, delta);
		ADJUST(dot, delta);
		ADJUST(dol, delta);
		ADJUST(truedol, delta);
		ADJUST(unddol, delta);
		ADJUST(addr1, delta);
		ADJUST(addr2, delta);
		ADJUST(undap1, delta);
		ADJUST(undap2, delta);
		ADJUST(unddel, delta);
		ADJUST(undadot, delta);
		ADJUST(wdot, delta);
		ADJUST(vUNDdot, delta);
		ADJUST(fendcore, delta);
	}

	endcore = line_array + line_array_size - 2;
	return 0;
}
