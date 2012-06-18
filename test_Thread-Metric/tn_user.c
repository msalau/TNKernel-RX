/*
TNKernel real-time kernel

Copyright © 2011-2012 Maxim Salov
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
#include <hwsetup.h>
#include <isr_vectors.h>
#include <stdout.h>

//----------------------------------------------------------------------------
// Hardware initialization routine
//----------------------------------------------------------------------------

void hardware_init(void)
{
    HardwareSetup();
    stdout_init();
    timer_init();
}

//----------------------------------------------------------------------------
// CMT0 initialization routine
//----------------------------------------------------------------------------

void timer_init(void)
{
	/* Configure CMT0 for 10ms periodic interrupt */
	MSTP(CMT0) = 0;					    /* Enable CMT0 */
	CMT.CMSTR0.BIT.STR0 = 0;			    /* Stop timer */
	CMT0.CMCR.BIT.CMIE = 1;				    /* Enable interrupt request */
	CMT0.CMCR.BIT.CKS = 2;				    /* PCLK/128 */
	CMT0.CMCOR = PCLK_FREQUENCY / 128 / 100 - 1;	    /* Set 10ms period */
	_isr_vectors[VECT(CMT0,CMI0)] = timer_irq_handler;
	IR(CMT0,CMI0) = 0;
	IPR(CMT0,CMI0) = 15;				    /* Set highest interrupt priority */
	IEN(CMT0,CMI0) = 1;				    /* Enable interrupt */
	CMT.CMSTR0.BIT.STR0 = 1;			    /* Start timer */
}

//----------------------------------------------------------------------------
// CMT0 periodic interrupt
//----------------------------------------------------------------------------

__attribute__((interrupt))
void timer_irq_handler(void)
{
	LED4 ^= 1;
	tn_tick_int_processing();
	tn_int_exit();
}

//----------------------------------------------------------------------------
