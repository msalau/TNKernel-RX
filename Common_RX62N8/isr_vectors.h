#ifndef ISR_VECTORS_H__
#define ISR_VECTORS_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*irq_handler)(void);
extern irq_handler _isr_vectors[256];

#ifdef __cplusplus
} // extern "C"
#endif

#define interrupt_attr __attribute__((interrupt))

#endif // ISR_VECTORS_H__
