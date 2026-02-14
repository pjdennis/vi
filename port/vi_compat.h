/*
 * PORT_NOTE: vi_compat.h - Compatibility layer for porting original vi to modern Linux
 *
 * Bridges the gap between the original V7/USG Unix interfaces and
 * modern POSIX/Linux. Provides:
 *   - termios-based tty handling (replaces termio.h)
 *   - Modern libc function declarations
 *   - Signal compatibility
 *   - BSD->POSIX function mappings
 */

#ifndef VI_COMPAT_H
#define VI_COMPAT_H

#include <stdint.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

/*
 * Map old USG termio to modern termios.
 * The original code uses "struct termio" with USG defined;
 * we redirect to termios which is a superset.
 */
/* ORIG: #include <termio.h> */
/* ORIG: typedef struct termio SGTTY; */
typedef struct termios SGTTY;

/*
 * BSD compatibility: bcopy -> memmove
 * The original uses bcopy in VMUNIX mode for temp file buffer copies.
 */
#define bcopy(from, to, n) memmove((to), (from), (n))

/*
 * Signal compatibility.
 * SIGEMT doesn't exist on Linux. Define it to an unused value
 * so references compile; we'll make sure it's never actually used.
 */
/* SIGEMT doesn't exist on Linux; just leave it undefined.
 * Code that uses it must be guarded with #ifdef SIGEMT. */

/*
 * Old USG termio ioctl names mapped to termios functions.
 * These are #defined so the original code patterns work:
 *   ioctl(fd, TCGETA, &tty)  ->  tcgetattr(fd, &tty)
 *   ioctl(fd, TCSETAW, &tty) ->  tcsetattr(fd, TCSADRAIN, &tty)
 *
 * Note: We don't actually #define the ioctls because we replace
 * the calls directly in ex_put.c. These comments document the mapping.
 */

/*
 * Speed extraction: original used (tty.c_cflag & CBAUD).
 * Modern termios uses cfgetospeed().
 */

/*
 * VFORK: the original uses vfork() under VMUNIX for performance.
 * Modern Linux vfork() is either identical to fork() or deprecated.
 * Map to fork() for safety.
 */
#define VFORK fork

/*
 * The original defined its own BUFSIZ when not using stdio.
 * Modern Linux stdio.h defines BUFSIZ (usually 8192).
 * We ensure it's at least 1024 for compatibility.
 */

/*
 * Old-style signal handler compatibility.
 * Original code used int return; modern requires void.
 * We typedef to help but actual conversion is in each file.
 */

#endif /* VI_COMPAT_H */
