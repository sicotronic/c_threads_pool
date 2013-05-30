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
#ifndef __TASK_H
#define __TASK_H

typedef void *arg_t;

typedef void *(*routine_t)(void *);

typedef struct _task_t *task_t;

task_t task_create (routine_t f, arg_t arg);
routine_t task_routine (task_t t);
arg_t task_arg (task_t t);
task_t task_destroy (task_t t);

#endif /* __TASK_H */
