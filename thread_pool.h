/*
* Copyright (C) 2007 Natalia B. Bidart
* 
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2.1 of the License, or any
* later version.
* 
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
* 
* You should have received a copy of the GNU General Public License along
* with this library; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#ifndef __THREAD_POOL_H
#define __THREAD_POOL_H

#include "task.h" /* routine_t, arg_t */

typedef struct _thread_pool_t *thread_pool_t;

/*  PRE: pool_size > 0 && queue_size > 0 */
/* POST: result != NULL */
thread_pool_t thread_pool_create (const unsigned int pool_size,
                                  const unsigned int queue_size);

/*  PRE: p != NULL && f != NULL */
/* POST: p->tasks has f (arg) as last task */
void thread_pool_execute_task (thread_pool_t p, routine_t f, arg_t arg);

/*  PRE: p != NULL */
/* POST: all memory used by p is freed && result == NULL */
thread_pool_t thread_pool_destroy (thread_pool_t p);

#endif /* __THREAD_POOL_H */
