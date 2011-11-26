/*

  TNKernel real-time kernel - YRDKRX62N test  (RX core)

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

/*============================================================================
  A basic test - (Hello-LED-World)
*===========================================================================*/

#include <tn.h>
#include <iodefine.h>
#include <yrdkrx62ndef.h>
#include "tn_user.h"
#include "hwsetup.h"
#include "stdout.h"
#include <stdio.h>

//----------- Tasks ----------------------------------------------------------

#define  TASK_0_PRIORITY 7
#define  TASK_1_PRIORITY 10
#define  TASK_2_PRIORITY 12

#define  TASK_0_STK_SIZE 128
#define  TASK_1_STK_SIZE 128
#define  TASK_2_STK_SIZE 128

align_attr_start unsigned int task_0_stack[TASK_0_STK_SIZE] align_attr_end;
align_attr_start unsigned int task_1_stack[TASK_1_STK_SIZE] align_attr_end;
align_attr_start unsigned int task_2_stack[TASK_2_STK_SIZE] align_attr_end;

TN_TCB  task_0;
TN_TCB  task_1;
TN_TCB  task_2;

void task_0_func(void * par unused_attr);
void task_1_func(void * par unused_attr);
void task_2_func(void * par unused_attr);

//----------------------------------------------------------------------------

noreturn_attr
void main (void)
{

   HardwareSetup();
   stdout_init();
   rtc_init();

   printf("Start system.\n");

   tn_start_system();

   for (;;);

}

//----------------------------------------------------------------------------

void tn_app_init ()
{

	printf("tn_app_init()\n");

   //--- Task 0

	printf("Create Task 0\n");

   task_0.id_task = 0;
   tn_task_create(&task_0,               //-- task TCB
                 task_0_func,            //-- task function
                 TASK_0_PRIORITY,        //-- task priority
                 &(task_0_stack          //-- task stack first addr in memory
                    [TASK_0_STK_SIZE-1]),
                 TASK_0_STK_SIZE,        //-- task stack size (in int,not bytes)
                 NULL,                          //-- task function parameter
                 TN_TASK_START_ON_CREATION      //-- Creation option
                 );

   //--- Task 1

   printf("Create Task 1\n");

   task_1.id_task = 0;
   tn_task_create(&task_1,               //-- task TCB
                 task_1_func,            //-- task function
                 TASK_1_PRIORITY,        //-- task priority
                 &(task_1_stack          //-- task stack first addr in memory
                    [TASK_1_STK_SIZE-1]),
                 TASK_1_STK_SIZE,        //-- task stack size (in int,not bytes)
                 NULL,                          //-- task function parameter
                 TN_TASK_START_ON_CREATION      //-- Creation option
                 );

   //--- Task 2

   printf("Create Task 2\n");

   task_2.id_task = 0;
   tn_task_create(&task_2,                      //-- task TCB
                 task_2_func,                   //-- task function
                 TASK_2_PRIORITY,               //-- task priority
                 &(task_2_stack                 //-- task stack first addr in memory
                    [TASK_2_STK_SIZE-1]),
                 TASK_2_STK_SIZE,               //-- task stack size (in int,not bytes)
                 NULL,                           //-- task function parameter
                 TN_TASK_START_ON_CREATION       //-- Creation option
                 );
}

//----------------------------------------------------------------------------
void task_0_func (void * par unused_attr)
{
   for(;;)
   {
	   LED10 ^= 1;
	   printf("task_0\n");
	   tn_task_sleep(128);
   }
}

//----------------------------------------------------------------------------
void task_1_func (void * par unused_attr)
{
	for (;;)
	{
	   LED9 ^= 1;
	   printf("task_1\n");
	   tn_task_sleep(256);
   }
}

//----------------------------------------------------------------------------
void task_2_func (void * par unused_attr)
{
   for(;;)
   {
	   LED11 ^= 1;
	   printf("task_2\n");
	   tn_task_sleep(512);
   }
}

//----------------------------------------------------------------------------
