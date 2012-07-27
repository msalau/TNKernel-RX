#ifndef TN_PORT_RX_H__
#define TN_PORT_RX_H__


  /* ---------- RX ----------------- */

#if defined  TNKERNEL_PORT_RX

  /* ------------------------------- */

#include <iodefine.h>

#undef  USE_ASM_FFS

#ifndef _RX_INT_REGISTERS
#define _RX_INT_REGISTERS 0
#endif

#define align_attr_start
#define align_attr_end     __attribute__((aligned(0x8)))
#define naked_attr         __attribute__((naked))
#define inline_attr        __attribute__((inline))
#define always_inline_attr __attribute__((always_inline))
#define naked_attr         __attribute__((naked))
#define pure_attr          __attribute__((pure))
#define noreturn_attr      __attribute__((noreturn))
#define unused_attr        __attribute__((unused))

#define  TN_TIMER_STACK_SIZE       128
#define  TN_IDLE_STACK_SIZE        128
#define  TN_MIN_STACK_SIZE         40      //--  +20 for exit func when ver GCC > 4

#define  TN_BITS_IN_INT            32

#define  TN_ALIG                   sizeof(void*)

#define  MAKE_ALIG(a)  ((sizeof(a) + (TN_ALIG-1)) & (~(TN_ALIG-1)))

#define  TN_PORT_STACK_EXPAND_AT_EXIT  17

  //----------------------------------------------------

#define  TN_NUM_PRIORITY        TN_BITS_IN_INT  //-- 0..31  Priority 0 always is used by timers task

#define  TN_WAIT_INFINITE       0xFFFFFFFF
#define  TN_FILL_STACK_VAL      0xDEADBEEF
#define  TN_INVALID_VAL         0xFFFFFFFF

#define  TN_TRAP_SAVE_INT       10
#define  TN_TRAP_RESTORE_INT    11

    //-- Assembler functions prototypes

#ifdef __cplusplus
extern "C"  {
#endif

  void  tn_switch_context_exit(void);
  void  tn_switch_context(void);
  void  tn_switch_context_irq_handler(void);

	unsigned int tn_cpu_save_sr(void);
	void  tn_cpu_restore_sr(unsigned int sr);

  unsigned int tn_icpu_save_sr(void);
  void  tn_icpu_restore_sr(unsigned int sr);

  void  tn_start_exe(void);
  int   tn_chk_irq_disabled(void);

	void  tn_int_exit(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

    //-- Interrupt processing   - processor specific

#define  TN_INTSAVE_DATA_INT     unsigned int tn_save_status_reg = 0;
#define  TN_INTSAVE_DATA         unsigned int tn_save_status_reg = 0;
#define  tn_disable_interrupt()  tn_save_status_reg = tn_cpu_save_sr()
#define  tn_enable_interrupt()   tn_cpu_restore_sr(tn_save_status_reg)

#define  tn_idisable_interrupt() tn_save_status_reg = tn_icpu_save_sr()
#define  tn_ienable_interrupt()  tn_icpu_restore_sr(tn_save_status_reg)

#define  TN_CHECK_INT_CONTEXT   \
             if(!tn_inside_int()) \
                return TERR_WCONTEXT;

#define  TN_CHECK_INT_CONTEXT_NORETVAL  \
             if(!tn_inside_int())     \
                return;

#define  TN_CHECK_NON_INT_CONTEXT   \
             if(tn_inside_int()) \
                return TERR_WCONTEXT;

#define  TN_CHECK_NON_INT_CONTEXT_NORETVAL   \
             if(tn_inside_int()) \
                return ;

  /* ----------Port not defined  ----- */

#endif // TNKERNEL_PORT_RX

#endif // TN_PORT_RX_H__
