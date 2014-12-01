#ifndef __COMPAT_H__
#define __COMPAT_H__

#ifndef __AVR__
#define __memx
#define PSTR(x) x
#define printf_P printf
#endif

#endif
