/*
TNKernel real-time kernel

Copyright © 2011 Maxim Salov
All rights reserved.

Permission to use, copy, modify, and distribute this software in source
and binary forms and its documentation for any purpose and without fee
is hereby granted, provided that the above copyright notice appear
in all copies and that both that copyright notice and this permission
notice appear in supporting documentation.

THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS ``AS IS'' AND
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

#include <iodefine.h>
#include <yrdkrx62ndef.h>
#include <tn.h>
#include <tn_user.h>

irq_handler _isr_vectors[256];

//----------------------------------------------------------------------------
// RTC initialization routine
//----------------------------------------------------------------------------

void rtc_init(void)
{
	/* For detailed description please refer to hardware manual */
	SYSTEM.SUBOSCCR.BIT.SUBSTOP = 0;                         /* Enable SubClock circuit */

	RTC.RCR2.BIT.START      = 0;
	while (0 != RTC.RCR2.BIT.START);
	RTC.RCR2.BIT.RESET      = 1;
	while (1 == RTC.RCR2.BIT.RESET);
	/* Set initial time and date */
	RTC.RSECCNT.BYTE        = 0;
	RTC.RMINCNT.BYTE        = 0;
	RTC.RHRCNT.BYTE         = 0;
	RTC.RDAYCNT.BYTE        = 0;
	RTC.RWKCNT.BYTE         = 0;
	RTC.RMONCNT.BYTE        = 0;
	RTC.RYRCNT.WORD         = 0x2000;
	/* Set periodic interrupt */
	RTC.RCR1.BYTE           = 0;
	RTC.RCR1.BIT.PES        = 1;                             /* Set periodic interrupts frequency to 256 Hz */
	RTC.RCR1.BIT.PIE        = 1;                             /* Enable periodic interrupts */
	_isr_vectors[VECT(RTC,PRD)] = rtc_irq_handler;
	IR(RTC,PRD)             = 0;                             /* Clear interrupt flag */
	IPR(RTC,PRD)            = 15;                            /* Set priority level */
	IEN(RTC,PRD)            = 1;                             /* Enable interrupt in ICU */
	RTC.RCR2.BIT.START      = 1;                             /* Start RTC */
	while (1 != RTC.RCR2.BIT.START);
}

//----------------------------------------------------------------------------
// RTC periodic interrupt
//----------------------------------------------------------------------------

__attribute__((interrupt))
void rtc_irq_handler(void)
{
	LED4 ^= 1;
	tn_tick_int_processing();
	tn_int_exit();
}

//----------------------------------------------------------------------------
