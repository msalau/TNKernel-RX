#ifndef STDOUT_H__
#define STDOUT_H__

void stdout_init (void);

#if 0
int putchar (int c);

int puts (const char *s);

__attribute__ ((format (printf, 1, 2)))
int printf (const char *fmt, ...);
#endif

#endif // STDOUT_H__
