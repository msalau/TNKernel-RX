/*
TNKernel real-time kernel

Copyright © 2004,2010 Yuri Tiomkin
Copyright © 2011 Maxim Salov (RX port)

All rights reserved.

Permission to use, copy, modify, and distribute this software in source
and binary forms and its documentation for any purpose and without fee
is hereby granted, provided that the above copyright notice appear
in all copies and that both that copyright notice and this permission
notice appear in supporting documentation.

THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
*/

/* ver 2.6 */

#include <tn.h>

//----------------------------------------------------------------------------
// Processor specific routine for RX
// sizeof(void*) = sizeof(unsigned int)
//----------------------------------------------------------------------------
unsigned int * tn_stack_init(void * task_func, void * stack_start, void * param)
{
   unsigned int * stk = (unsigned int *)stack_start;

   stk--;
   *stk = ((unsigned int)tn_task_exit);							/* Task termination routine address. You'll get there if will return from main. */
   stk--;
   *stk = 0x15151515UL;
   stk--;
   *stk = 0x14141414UL;
   stk--;
   *stk = 0x13131313UL;
   stk--;
   *stk = 0x12121212UL;
   stk--;
   *stk = 0x11111111UL;
   stk--;
   *stk = 0x10101010UL;
   stk--;
   *stk = 0x09090909UL;
   stk--;
   *stk = 0x08080808UL;
   stk--;
   *stk = 0x07070707UL;
   stk--;
   *stk = 0x06060606UL;
   stk--;
   *stk = 0x05050505UL;
   stk--;
   *stk = 0x04040404UL;
   stk--;
   *stk = 0x03030303UL;
   stk--;
   *stk = 0x02020202UL;
   stk--;
   *stk = (unsigned int)param;										/* Parameter passed to main function of the task. */
   stk--;
   *stk = (1UL << 16) |													/* I = 1 : Enable interrupts */
	   (1UL << 17) |														/* U = 1 : Use user stack pointer (USP) */
	   (1UL << 20);														/* PM = 1 : user threads operate in user mode (non privileged) */
   stk--;
   *stk = ((unsigned int)task_func);

   return stk;
}

//----------------------------------------------------------------------------
