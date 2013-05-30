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
#include <stdio.h> /* printf */
#include <stdbool.h> /* bool, true, false */
#include <assert.h> /* assert */
#include <stdlib.h> /* calloc, free, strtoull */
#include <math.h> /* ceil, sqrt, fmod */
#include <pthread.h> /* pthread_mutex_{t, init, destroy} */
                     /* pthread_cond_{t, init, destroy, wait, signal} */
#include "thread_pool.h" /* thread_pool_{t, create, destroy, execute_task} */

#define BOTTOM 2
#define POOL_SIZE 2
#define QUEUE_SIZE 100
#define CHUNK_SIZE 1000

/* shared data */
unsigned long long int number_to_test = 0, intervals = 0, tested = 0;
bool is_prime = true;
pthread_mutex_t mutex;
pthread_cond_t done;

/* testing routine's prototype */
void *check_primality (void *arg);

/* helper struct to pass useful arguments to testing routine */
struct range {
	unsigned long long int start;
	unsigned long long int end;
};

/* main routine */
int main (int argc, char *argv[]) {
	unsigned int i = 0;
	double q = 0.0;
	struct range *tests = NULL;
	thread_pool_t p = NULL;
	
	/* grab number to test from arguments */
	if (argc < 2) {
		printf ("Usage: test_thread_pool <number to test for primality>.\n");
		return (-1);
	}
	assert (argc > 1);
	number_to_test = strtoull (argv[1], NULL, 10);
	/* one of the longuest primes: */
	/*number_to_test = 2305843009213693951ULL;*/
	if (number_to_test < 1) {
		printf ("Please enter a positive number.\n");
		return (-1);
	}
	
	/* from: http://en.wikipedia.org/wiki/Primality_testing
	 *
	 * Rather than testing all numbers up to `number_to_test - 1', we need
	 * only test numbers up to `sqrt (number_to_test)': if `number_to_test'
	 * is composite then it can be factored into two values, at least one
	 * of which must be less than or equal to `sqrt (number_to_test)'.
	 */
	q = sqrt (number_to_test);
	
	/* divide work for threads */
	intervals = ceil (q / CHUNK_SIZE);
	
	/* init stuff */
	pthread_mutex_init (&mutex, NULL);
	pthread_cond_init (&done, NULL);
	
	tests = calloc (intervals, sizeof (struct range));
	assert (tests != NULL);
	
	p = thread_pool_create (POOL_SIZE, QUEUE_SIZE);
	assert (p != NULL);
	
	/* execute tasks through thread pool `p' */
	for (i = 0; i < intervals && is_prime; i++) {
		tests[i].start = BOTTOM + i * CHUNK_SIZE;
		if (i == intervals - 1) { /* last interval */
			tests[i].end = ceil (q) + 1; /* the nearest integer to `q' */
		} else {
			tests[i].end = tests [i].start + CHUNK_SIZE;
		}
		thread_pool_execute_task (p, check_primality, (void *)&tests[i]);
	}
	
	/* wait for all intervals to be processed */
	pthread_mutex_lock (&mutex);
	pthread_cond_wait (&done, &mutex);
	pthread_mutex_unlock (&mutex);
	
	/* print result on stdout */
	printf ("test_thread_pool.c: main: %lli is %sprime!\n",
	        number_to_test, (is_prime ? "" : "not "));
	
	/* destroy stuff */
	p = thread_pool_destroy (p);
	pthread_mutex_destroy (&mutex);
	pthread_cond_destroy (&done);
	free (tests);
	tests = NULL;
	
	return (0);
}

/* testing routine */
void *check_primality (void *arg) {
	struct range interval;
	unsigned int i = 0;
	
	interval = *(struct range *)arg;
	for (i = interval.start; i < interval.end && is_prime; i++){
		if (i != number_to_test && (number_to_test % i) == 0) {
			/* printf ("check_primality: divisor: %u.\n",i); */
			is_prime = false;
		}
	}
	assert (i >= interval.end || !is_prime);
	pthread_mutex_lock (&mutex);
	tested++;
	pthread_mutex_unlock (&mutex);
	
	if (tested == intervals)
		pthread_cond_signal (&done);
	
	return NULL;
}
