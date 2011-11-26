#ifndef TN_USER_H__
#define TN_USER_H__

typedef void (*irq_handler)(void);
extern irq_handler _isr_vectors[256];

void rtc_init(void);
void rtc_irq_handler(void);

#endif	// TN_USER_H__
