/*
* Copyright (C) 2007 
*	Navarro Horniacek, Juan Ignacio
* 	Operative Systems Lab Project - FaMAF
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

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <semaphore.h>
#include "task.h"
#include "thread_pool.h"
#include "task_queue.h"

#define NOTSHARED 0
#define _Y yield();

struct _thread_pool_t {
        pthread_t *pool;
        task_queue_t tasks;
        unsigned int thread_amount;
	unsigned int finalizados;
	pthread_mutex_t mutex;
	bool pool_finish;
};

static void yield(void);
static void *task_consum(void *pl);

thread_pool_t 
thread_pool_create(const unsigned int pool_size,const unsigned int queue_size){
	thread_pool_t p=NULL;
	unsigned int i=0;

	p=(thread_pool_t)calloc(1,sizeof(struct _thread_pool_t));
	if(p!=NULL){
		p->pool=(pthread_t *)calloc(pool_size,sizeof(pthread_t));
		if(p->pool!=NULL){
			p->pool_finish=false;
			p->thread_amount=pool_size;
			p->finalizados=0;
			/* create the queue and start the mutex */
                        p->tasks=task_queue_create(queue_size);
			pthread_mutex_init(&p->mutex,NULL);
			/* create the threads */
			for(i=0;i<pool_size;i++){
				pthread_create(&p->pool[i],NULL,
					&task_consum,(void *)&p);
			}
		}
		else {
			free(p);
			p=NULL;
			perror("NO HAY MEMORIA PARA CREAR EL THREAD POOL\n");
			assert(p!=NULL);
		}
	}
	else {
		perror("NO HAY MEMORIA PARA CREAR EL THREAD POOL\n");
		assert(p!=NULL);
	}

	return p;
}

void 
thread_pool_execute_task(thread_pool_t p, routine_t f, arg_t arg){
	task_t t=NULL;
        assert(p!=NULL && p->tasks!=NULL);
	
	/* create the task and add it to the queue */
	t=task_create(f,arg);
	p->tasks=task_queue_push(p->tasks,t);
}

thread_pool_t 
thread_pool_destroy(thread_pool_t p){
	unsigned int i=0;
	assert(p!=NULL && p->pool!=NULL && p->tasks!=NULL);
	p->pool_finish=true;
	if(p->finalizados==p->thread_amount){
		for(i=0;i<p->thread_amount;i++){
			pthread_join(p->pool[i],NULL);
		}
	}
	else {
		for(i=0;i<p->thread_amount;i++){
			pthread_cancel(p->pool[i]);
		}
	}
	p->tasks=task_queue_destroy(p->tasks);
	free(p->pool);
	p->pool=NULL;
	free(p);
	p=NULL;

        return p;
}

static void 
*task_consum(void *pl) {
	thread_pool_t p=NULL;
	task_t t=NULL;
	routine_t r=NULL;
	arg_t a=NULL;
	
	assert(pl!=NULL);
	p=*(thread_pool_t *) pl;
	assert(p!=NULL);
	assert(p->tasks!=NULL);_Y
	while(!p->pool_finish){
		p->tasks=task_queue_pop(p->tasks,&t);_Y
		assert(t!=NULL);_Y
		/* take the routine and the argument and execute it */
		r=task_routine(t);_Y
		a=task_arg(t);_Y
		t=task_destroy(t);_Y
		t=NULL;
		assert(r!=NULL);
		r(a); /* execute the routine */
	}
	pthread_mutex_lock(&p->mutex);	
		p->finalizados++;
	pthread_mutex_unlock(&p->mutex);

	pthread_exit(NULL);
}

/* it is given to the scheduler in a probabilistic way */
void yield(void){
	if (rand()%2)
		sched_yield();
}
