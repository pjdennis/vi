/*
 * vi_mem.h - Memory management for line pointer array
 *
 * Replaces the original sbrk()/brk() memory management with
 * malloc()/realloc(). The line pointer array is dynamically
 * allocated and grown as needed.
 */

#ifndef VI_MEM_H
#define VI_MEM_H

/*
 * vi_mem_init: Allocate initial line pointer array.
 * Sets fendcore/endcore to bracket the available space.
 * Must be called before fileinit().
 */
void vi_mem_init(void);

/*
 * vi_morelines: Grow the line pointer array by 1024 entries.
 * Adjusts all global line* pointers by the delta if realloc
 * moves the block. Returns 0 on success, -1 on failure.
 */
int vi_morelines(void);

#endif /* VI_MEM_H */
