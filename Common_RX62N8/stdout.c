#include <tn.h>
#include "stdout.h"
#include "sci2.h"
#include "xprintf.h"

void stdout_init (void)
{
	sci2_init();
}

int printf (const char *fmt, ...)
{
	TN_INTSAVE_DATA;
	tn_disable_interrupt();
	va_list arp;
	va_start(arp, fmt);
	int ret = xvprintf(sci2_putchar, fmt, arp);
	va_end(arp);
	tn_enable_interrupt();
	return ret;
}

int puts (const char *s)
{
	TN_INTSAVE_DATA;
	tn_disable_interrupt();
	int ret = sci2_puts(s);
	sci2_putchar('\n');
	tn_enable_interrupt();
	return ret;
}

int putchar (int c)
{
	TN_INTSAVE_DATA;
	tn_disable_interrupt();
	int ret = sci2_putchar(c);
	tn_enable_interrupt();
	return ret;
}
