/*
 * mutexk.c: example of mutual exclusion with pthreads. Based on an example of a
 *          tutorial of Peter C. Chapin
 *
 * Programmer: Ruben Carvajal Schiaffino
 *
 * Santiago de Chile, 6/10/2017
 *
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 


pthread_mutex_t lock;
int shared;

/*
 *
 */
void *ThreadFunction(void *n) {

   int i, *me;

   me = (int *) n; 
   sleep(3);
   for (i = 0, shared = 0; i < 10; i = i + 1) {
      pthread_mutex_lock(&lock);
      shared = shared + 1;
      printf("From %d ThreadFunction shared = %d\n",me,shared);
      pthread_mutex_unlock(&lock);
      sleep(2);
   }
   pthread_exit(0);
}

/*
 *
 */
int main(int argc, char **argv) {

   pthread_t *thread;
   pthread_attr_t attribute;
   void *exit_status;
   int i, k;

   if (argc == 2) {
      k = atoi(argv[1]);
      pthread_mutex_init(&lock,NULL);   
      pthread_attr_init(&attribute);
      thread = calloc(k,sizeof(pthread_t));
      pthread_attr_setdetachstate(&attribute,PTHREAD_CREATE_JOINABLE); 
      for (i = 0; i < k; i = i + 1) 
         pthread_create(&thread[i],&attribute,ThreadFunction,(void *) i);
      for (i = 0; i < 10; i = i + 1) {
         sleep (1);
         pthread_mutex_lock(&lock);
         printf("From main Shared Integer Value = %d\n",shared);
         pthread_mutex_unlock(&lock);
      }
      printf("\n");
      for (i = 0; i < k; i = i + 1)
         pthread_join(thread[i],&exit_status);
      pthread_mutex_destroy(&lock);
   }
   return 0;
}

