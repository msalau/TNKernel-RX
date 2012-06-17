#ifndef SCI2_H__
#define SCI2_H__

#include <stdint.h>

void sci2_init (void);
int sci2_putchar (int c);
void sci2_putc (unsigned char c);
int sci2_puts (const char *s);
int sci2_puthex4 (uint64_t v);

#endif	// SCI2_H__
