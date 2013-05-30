/*
* Copyright (C) 2007
*	Navarro Horniacek, Juan Ignacio
* 	Operative Systems Lab Proyect - FaMAF
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


#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h> /*semaforos*/
#include "task.h" /* task_t */
#include "task_queue.h"

#define NOT_SHARED 0
#define _Y yield()

struct _task_queue_t {
	task_t *task_buffer;
	unsigned int in;
	unsigned int out;
	unsigned int max_size;
	pthread_mutex_t mutex;
	sem_t empty,full;
};

static void yield(void);

task_queue_t
task_queue_create(const unsigned int size){
	task_queue_t q=NULL;
	assert(size>0);
	q=(task_queue_t)calloc(1,sizeof(struct _task_queue_t));
	
	if(q!=NULL){
		pthread_mutex_init(&q->mutex,NULL);
		sem_init(&q->full,NOT_SHARED,0);
		sem_init(&q->empty,NOT_SHARED,size);
		q->max_size=size;
		q->out=0;
		q->in=0;
		q->task_buffer=(task_t*)calloc(size,sizeof(task_t));
	}
	else {
		free(q);
		q=NULL;
		perror("Not enough memory to create the task_queue\n");
		assert(q!=NULL);
	}

	return q;
}

task_queue_t
task_queue_push(task_queue_t q, task_t t){ /*Modelo: Producer*/
	assert(q!=NULL && t!=NULL);
	
	/**CRITICAL SECTION*/
	sem_wait(&q->empty); _Y;
		pthread_mutex_lock(&q->mutex);_Y;
			q->task_buffer[q->in]=t;_Y;
			q->in=(q->in+1) % (q->max_size);_Y;
		pthread_mutex_unlock(&q->mutex);_Y;
	sem_post(&q->full);_Y;
	/**END CRITICAL SECTION*/
	
	return q;
}

task_queue_t
task_queue_pop(task_queue_t q, task_t *t){ /*Modelo: Consumer*/
	assert(q!=NULL && *t==NULL);_Y;
	
	/**CRITICAL SECTION*/
	sem_wait(&q->full);_Y;
		pthread_mutex_lock(&q->mutex);_Y;
			*t=q->task_buffer[q->out];_Y;
			q->out=(q->out+1) % (q->max_size);_Y;
		pthread_mutex_unlock(&q->mutex);_Y;
	sem_post(&q->empty);_Y;
	/**END CRITICAL SECTION*/
	return q;
	
}

task_queue_t
task_queue_destroy(task_queue_t q){
	assert(q!=NULL && q->task_buffer!=NULL);
	sem_destroy(&q->empty);
	sem_destroy(&q->full);
	pthread_mutex_destroy(&q->mutex);
	free(q->task_buffer);
	q->task_buffer=NULL;
	free(q);
	q=NULL;
	return q;
}

void
task_queue_print(task_queue_t q){
	char *sa=NULL,*sr=NULL;
	unsigned int i=0,size=0;
	task_t t=NULL;
	routine_t r=NULL;
	arg_t a=NULL;
	assert(q!=NULL);
	size=task_queue_count(q);
	for(i=0;i<size;i++){
		q=task_queue_pop(q,&t);	
		r=task_routine(t);
		a=task_arg(t);
		q=task_queue_push(q,t);
		t=NULL;
		sa=(char*)a;
		sr=(char*)r;
		printf("Task number %i:\n"
			"\t %s (%s)",i,sr,sa);
		free(sa);
		sa=NULL;
		free(sr);
		sr=NULL;
		free(a);
		a=NULL;
		free(r);
		r=NULL;
	}
}

unsigned int
task_queue_count(task_queue_t q){
	return (abs(q->out-q->in)%(q->max_size+1));
}

static void
yield(void){
	if(rand()%2)
		sched_yield();
}
