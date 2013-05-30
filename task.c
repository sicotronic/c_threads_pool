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
#include <stdlib.h> /* calloc */
#include <assert.h> /* assert */
#include "task.h" /* routine_t, arg_t, task_t */

struct _task_t {
	routine_t f;
	arg_t arg;
};

task_t task_create (routine_t f, arg_t arg) {
	task_t result = NULL;
	
	/* PRE: */assert (f != NULL);
	
	result = calloc (1, sizeof (*result));
	result->f = f;
	result->arg = arg;
	
	/* POST: */assert (result != NULL);
	/* POST: */assert (result->f == f);
	/* POST: */assert (result->arg == arg);
	return (result);
}

routine_t task_routine (task_t t) {
	/* PRE: */assert (t != NULL);
	return (t->f);
}

arg_t task_arg (task_t t) {
	/* PRE: */assert (t != NULL);
	return (t->arg);
}

task_t task_destroy (task_t t) {
	/* PRE: */assert (t != NULL);
	
	free (t);
	t = NULL;
	/* POST: */assert (t == NULL);
	return (t);
}
