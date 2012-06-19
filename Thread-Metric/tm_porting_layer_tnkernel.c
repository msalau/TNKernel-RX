/**************************************************************************/ 
/*                                                                        */ 
/*            Copyright (c) 1996-2007 by Express Logic Inc.               */ 
/*            Copyright (c) 20012 by Maxim Salov                          */ 
/*                                                                        */ 
/*  This Original Work may be modified, distributed, or otherwise used in */ 
/*  any manner with no obligations other than the following:              */ 
/*                                                                        */ 
/*    1. This legend must be retained in its entirety in any source code  */ 
/*       copies of this Work.                                             */ 
/*                                                                        */ 
/*    2. This software may not be used in the development of an operating */
/*	 system product.                                                  */ 
/*                                                                        */  
/*  This Original Work is hereby provided on an "AS IS" BASIS and WITHOUT */ 
/*  WARRANTY, either express or implied, including, without limitation,   */ 
/*  the warranties of NON-INFRINGEMENT, MERCHANTABILITY or FITNESS FOR A  */ 
/*  PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE QUALITY OF this         */ 
/*  ORIGINAL WORK IS WITH the user.                                       */ 
/*                                                                        */ 
/*  Express Logic, Inc. reserves the right to modify this software        */ 
/*  without notice.                                                       */ 
/*                                                                        */ 
/*  Express Logic, Inc.                     info@expresslogic.com         */
/*  11423 West Bernardo Court               http://www.expresslogic.com   */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/

/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** Thread-Metric Component                                              */
/**                                                                       */
/**   Porting Layer (Must be completed with RTOS specifics)               */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary files.  */

#include    "tm_api.h"
#include    <tn.h>
#include    <tn_user.h>

/* Define TNKernel mapping constants. */

#define TM_TNKERNEL_MAX_THREADS          10
#define TM_TNKERNEL_MAX_QUEUES           1
#define TM_TNKERNEL_MAX_SEMAPHORES       1
#define TM_TNKERNEL_MAX_MEMORY_POOLS     1

/* Define TNKernel object's size constants. */

#define TM_TNKERNEL_THREAD_STACK_SIZE		512
#define TM_TNKERNEL_QUEUE_SIZE			200
#define TM_TNKERNEL_MEMORY_POOL_BLOCK_SIZE	128
#define TM_TNKERNEL_MEMORY_POOL_MAX_BLOCKS	32
#define TM_TNKERNEL_TICKS_PER_SECOND		100

/* Define object mapping arrays. */

TN_TCB	tm_thread_array[TM_TNKERNEL_MAX_THREADS];
TN_DQUE tm_queue_array[TM_TNKERNEL_MAX_QUEUES];
TN_SEM	tm_semaphore_array[TM_TNKERNEL_MAX_SEMAPHORES];
TN_FMP	tm_memory_pool_array[TM_TNKERNEL_MAX_MEMORY_POOLS];

/* Define associated memory regions. */

unsigned int	tm_thread_stack_area[TM_TNKERNEL_MAX_THREADS*TM_TNKERNEL_THREAD_STACK_SIZE];
void *		tm_queue_memory_area[TM_TNKERNEL_MAX_QUEUES*TM_TNKERNEL_QUEUE_SIZE];
unsigned char	tm_memory_pool_area[TM_TNKERNEL_MAX_MEMORY_POOLS*TM_TNKERNEL_MEMORY_POOL_BLOCK_SIZE*TM_TNKERNEL_MEMORY_POOL_MAX_BLOCKS];

/* Define thread entry functions array. */

void (*tm_thread_entry_function_array[TM_TNKERNEL_MAX_THREADS])(void);

/* Define thread entry wrapper function to match TNKernel. */

void tm_thread_entry_function_wrapper(void * param);

/* Remember test initialization function. */

void (*tm_initialization_function)(void);

/* This function called from main performs basic RTOS initialization, 
   calls the test initialization function, and then starts the RTOS function.  */
void  tm_initialize(void (*test_initialization_function)(void))
{
    tm_initialization_function = test_initialization_function;
    hardware_init();
    tn_start_system();
}

/* This function is called after OS is started.
   It calls test initialization routine. */
void tn_app_init(void)
{
    tm_initialization_function();
}

/* This function calls thread function by its id. */
void tm_thread_entry_function_wrapper(void * param)
{
    int thread_id = (unsigned int)param;
    tm_thread_entry_function_array[thread_id]();
}

/* This function takes a thread ID and priority and attempts to create the
   file in the underlying RTOS.  Valid priorities range from 1 through 31, 
   where 1 is the highest priority and 31 is the lowest. If successful, 
   the function should return TM_SUCCESS. Otherwise, TM_ERROR should be returned.   */
int  tm_thread_create(int thread_id, int priority, void (*entry_function)(void))
{
    TN_TCB *task = &tm_thread_array[thread_id];
    unsigned int *stack_ptr = &tm_thread_stack_area[(thread_id+1)*TM_TNKERNEL_THREAD_STACK_SIZE-1];
    int status;

    task->id_task = 0;
    tm_thread_entry_function_array[thread_id] = entry_function;
    status = tn_task_create(task,
			    tm_thread_entry_function_wrapper,
			    priority,
			    stack_ptr,
			    TM_TNKERNEL_THREAD_STACK_SIZE,
			    (void*)thread_id,
			    TN_TASK_START_ON_CREATION
	);
    if (TERR_NO_ERR != status)
    {
	return TM_ERROR;
    }

    status = tn_task_suspend(task);
    if (TERR_NO_ERR != status)
    {
	return TM_ERROR;
    }
    return TM_SUCCESS;
}


/* This function resumes the specified thread.  If successful, the function should
   return TM_SUCCESS. Otherwise, TM_ERROR should be returned.  */
int  tm_thread_resume(int thread_id)
{
    TN_TCB *task = &tm_thread_array[thread_id];
    int status;
    status = tn_task_resume(task);
    if (TERR_NO_ERR != status)
    {
	return TM_ERROR;
    }
    return TM_SUCCESS;
}


/* This function suspends the specified thread.  If successful, the function should
   return TM_SUCCESS. Otherwise, TM_ERROR should be returned.  */
int  tm_thread_suspend(int thread_id)
{
    TN_TCB *task = &tm_thread_array[thread_id];
    int status;
    status = tn_task_suspend(task);
    if (TERR_NO_ERR != status)
    {
	return TM_ERROR;
    }
    return TM_SUCCESS;
}


/* This function relinquishes to other ready threads at the same
   priority.  */
void tm_thread_relinquish(void)
{
    tn_task_sleep(1);
}


/* This function suspends the specified thread for the specified number
   of seconds.  If successful, the function should return TM_SUCCESS. 
   Otherwise, TM_ERROR should be returned.  */
void tm_thread_sleep(int seconds)
{
    tn_task_sleep(seconds*TM_TNKERNEL_TICKS_PER_SECOND);
}


/* This function creates the specified queue.  If successful, the function should
   return TM_SUCCESS. Otherwise, TM_ERROR should be returned.  */
int  tm_queue_create(int queue_id)
{
    TN_DQUE *queue = &tm_queue_array[queue_id];
    int status;

    queue->id_dque = 0;
    status = tn_queue_create(queue,
			     (void**)&tm_queue_memory_area,
			     TM_TNKERNEL_QUEUE_SIZE
	);
    if (TERR_NO_ERR != status)
    {
	return TM_ERROR;
    }
    return TM_SUCCESS;
}


/* This function sends a 16-byte message to the specified queue.  If successful, 
   the function should return TM_SUCCESS. Otherwise, TM_ERROR should be returned.  */
int  tm_queue_send(int queue_id, unsigned long *message_ptr)
{
    TN_DQUE *queue = &tm_queue_array[queue_id];
    int status;

    for (unsigned int i = 4; i; --i)
    {
	status = tn_queue_send(queue, (void*)(*message_ptr++), TN_WAIT_INFINITE);
	if (TERR_NO_ERR != status)
	{
	    return TM_ERROR;
	}
    }
    return TM_SUCCESS;
}


/* This function receives a 16-byte message from the specified queue.  If successful, 
   the function should return TM_SUCCESS. Otherwise, TM_ERROR should be returned.  */
int  tm_queue_receive(int queue_id, unsigned long *message_ptr)
{
    TN_DQUE *queue = &tm_queue_array[queue_id];
    int status;

    for (unsigned int i = 4; i; --i)
    {
	status = tn_queue_receive(queue, (void**)(message_ptr++), TN_WAIT_INFINITE);
	if (TERR_NO_ERR != status)
	{
	    return TM_ERROR;
	}
    }
    return TM_SUCCESS;
}


/* This function creates the specified semaphore.  If successful, the function should
   return TM_SUCCESS. Otherwise, TM_ERROR should be returned.  */
int  tm_semaphore_create(int semaphore_id)
{
    TN_SEM *semaphore = &tm_semaphore_array[semaphore_id];
    int status;

    semaphore->id_sem = 0;
    status = tn_sem_create(semaphore, 1, 1);
    if (TERR_NO_ERR != status)
    {
	return TM_ERROR;
    }
    return TM_SUCCESS;
}


/* This function gets the specified semaphore.  If successful, the function should
   return TM_SUCCESS. Otherwise, TM_ERROR should be returned.  */
int  tm_semaphore_get(int semaphore_id)
{
    TN_SEM *semaphore = &tm_semaphore_array[semaphore_id];
    int status;

    status = tn_sem_acquire(semaphore, TN_WAIT_INFINITE);
    if (TERR_NO_ERR != status)
    {
	return TM_ERROR;
    }
    return TM_SUCCESS;
}


/* This function puts the specified semaphore.  If successful, the function should
   return TM_SUCCESS. Otherwise, TM_ERROR should be returned.  */
int  tm_semaphore_put(int semaphore_id)
{
    TN_SEM *semaphore = &tm_semaphore_array[semaphore_id];
    int status;

    status = tn_sem_signal(semaphore);
    if (TERR_NO_ERR != status)
    {
	return TM_ERROR;
    }
    return TM_SUCCESS;
}


/* This function creates the specified memory pool that can support one or more
   allocations of 128 bytes.  If successful, the function should
   return TM_SUCCESS. Otherwise, TM_ERROR should be returned.  */
int  tm_memory_pool_create(int pool_id)
{
    (void)pool_id;
    return TM_ERROR;
}


/* This function allocates a 128 byte block from the specified memory pool.  
   If successful, the function should return TM_SUCCESS. Otherwise, TM_ERROR 
   should be returned.  */
int  tm_memory_pool_allocate(int pool_id, unsigned char **memory_ptr)
{
    (void)pool_id;
    (void)memory_ptr;
    return TM_ERROR;
}


/* This function releases a previously allocated 128 byte block from the specified 
   memory pool. If successful, the function should return TM_SUCCESS. Otherwise, TM_ERROR 
   should be returned.  */
int  tm_memory_pool_deallocate(int pool_id, unsigned char *memory_ptr)
{
    (void)pool_id;
    (void)memory_ptr;
    return TM_ERROR;
}
