# port2 Porting Plan — Clean C11 Transition

Status legend: [ ] pending, [x] done

## High Priority — Mechanical, safe bulk changes

- [x] 1. Remove all `register` keywords (~312 instances, 26 files)
- [ ] 2. Remove dead Makefile `-D` flags (VMUNIX, USG, SIGTSTP)
- [ ] 3. Remove dead code with `__attribute__((unused))` (vrigid, vcookit)
- [ ] 4. Remove `index`/`rindex` compatibility shim in ex_io.c, use strchr/strrchr directly
- [ ] 5. Remove stale local prototype `char *vgetpass()` in ex_cmds.c

## Medium Priority — Structural cleanup

- [ ] 6. Move extern declarations from .c files into headers
- [ ] 7. Fix old-style function pointer typedef `iofcn_t` (empty parameter list)
- [ ] 8. Replace `short` with `int` where appropriate (~20 instances)
- [ ] 9. Replace `(char *) 0` with NULL (~15 instances)
- [ ] 10. Add include guard to ex.h
- [ ] 11. Resolve printf.c GCC pragma / naming conflict

## Lower Priority — Deeper old-style patterns

- [ ] 12. Replace `Copy()` function and `copy()` macro with memcpy
- [ ] 13. Clean up `forbid()` macro hacks with embedded goto
- [ ] 14. Replace `CP()` macro with direct strcpy (or safer alternative)
- [ ] 15. Convert `#define` constant groups to `enum` (ex_vars.h, ex_vis.h, ex_re.h)
- [ ] 16. Remove stale platform comments (USG, VMUNIX, 4.2BSD references)
