# port2 Porting Plan — Clean C11 Transition

Status legend: [ ] pending, [x] done

## High Priority — Mechanical, safe bulk changes

- [x] 1. Remove all `register` keywords (~312 instances, 26 files)
- [x] 2. Remove dead Makefile `-D` flags (VMUNIX, USG, SIGTSTP)
- [x] 3. Remove dead code with `__attribute__((unused))` (vrigid, vcookit)
- [x] 4. Remove `index`/`rindex` compatibility shim in ex_io.c, use strchr/strrchr directly
- [x] 5. Remove stale local prototype `char *vgetpass()` in ex_cmds.c

## Medium Priority — Structural cleanup

- [x] 6. Move extern declarations from .c files into headers
- [x] 7. Fix old-style function pointer typedef `iofcn_t` (empty parameter list)
- [x] 8. Replace `short` with `int` where appropriate (~20 instances)
- [x] 9. Replace `(char *) 0` and `(line *) 0` with NULL/NOSTR/NOLINE
- [x] 10. Add include guard to ex.h
- [x] 11. Resolve printf.c GCC pragma / naming conflict

## Lower Priority — Deeper old-style patterns

- [x] 12. Replace `Copy()` function and `copy()` macro with memcpy
- [x] 13. Clean up `forbid()` macro hacks with embedded goto
- [x] 14. Simplify `CP()` macro to direct strcpy
- [x] 15. Convert `#define` constant groups to `enum` (ex_vars.h, ex_vis.h)
- [x] 16. Remove stale platform comments (USG, VMUNIX, 4.2BSD references)
