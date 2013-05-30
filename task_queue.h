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
#ifndef __TASK_QUEUE_H
#define __TASK_QUEUE_H

#include "task.h" /* task_t */

typedef struct _task_queue_t *task_queue_t;

/* CLASS INVARIANT (task_queue_t q): forall i in q : i != NULL */

/*  PRE: assert (size > 0) */
/* POST: result != NULL */
task_queue_t task_queue_create (const unsigned int size);

/*  PRE: q != NULL && t != NULL */
/* POST: result != NULL && task_queue_count (result) > 0 */
task_queue_t task_queue_push (task_queue_t q, task_t t);

/*  PRE: q != NULL && *t == NULL */
/* POST: result != NULL && t != NULL && *t != NULL */
task_queue_t task_queue_pop (task_queue_t q, task_t *t);

/*  PRE: q != NULL */
/* POST: all memorys used by q is freed and result == NULL */
task_queue_t task_queue_destroy (task_queue_t q);

/*  PRE: q != NULL */
/* POST: there's no side-effects */
void task_queue_print (task_queue_t q);

/*  PRE: q != NULL */
/* POST: result >= 0 &&  there's no side-effects */
unsigned int task_queue_count (task_queue_t q);

#endif /* __TASK_QUEUE_H */
