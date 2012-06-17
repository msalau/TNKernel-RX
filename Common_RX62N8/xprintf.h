/*------------------------------------------------------------------------*/
/* Universal string handler for user console interface  (C)ChaN, 2011     */
/*------------------------------------------------------------------------*/

#ifndef _STRFUNC
#define _STRFUNC

#include <stdarg.h>

int xvprintf (int (*xputc)(int), const char* fmt, va_list arp);

#endif

