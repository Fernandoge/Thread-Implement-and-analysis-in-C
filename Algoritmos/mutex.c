/*
 * mutex.c: example of mutual exculsion with pthreads. Based on an example of a
 *          tutorial of Peter C. Chapin
 *
 * Programmer: Ruben Carvajal Schiaffino
 *
 * Santiago de Chile, 19/11/2013
 *
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

pthread_mutex_t lock;
int shared;

/*
 *
 */
void *ThreadFunction(void *arg) {

   int i;

   sleep(3);
   for (i = 0, shared = 0; i < 10 * 10; i = i + 1) {
      pthread_mutex_lock(&lock);
      shared = shared + 1;
      printf("From ThreadFunction shared = %d\n",shared);
      pthread_mutex_unlock(&lock);
   }
   return NULL;
}

/*
 *
 */
int main(void) {

   pthread_t thread_ID;
   void *exit_status;
   int i;

   pthread_mutex_init(&lock,NULL);
   pthread_create(&thread_ID,NULL,ThreadFunction,NULL);
   for (i = 0; i < 10; i = i + 1) {
      sleep (1);
      pthread_mutex_lock(&lock);
      printf("Shared Integer Value = %d\n",shared);
      pthread_mutex_unlock(&lock);
    }
    printf("\n");
    pthread_join(thread_ID,&exit_status);
    pthread_mutex_destroy(&lock);
    return 0;
}
