/*
 * ParSquaresGlobal: computes the square roots for numbers between 1 and n using K threads and 
 *                a global variable
 *
 * Programmer: Fernando Garcia Polgatti

 * Santiago de Chile: 26/11/2018
 *
 */


#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


struct Message {

   int myid, nvalue, numthreads;
};

float *result;   

/*
 *
 */
void *Computes(void *n) {

   int i, j, size;
   struct Message *m;

   //printf("\n\n**************************************\n\n");
   m = (struct Message *) n;
   //printf("From %d Beginning The Task\n\n",m->myid);
   size = m->nvalue / m->numthreads;
   for (i = 0, j = m->myid * size + 1; i < size; i = i + 1, j = j + 1)
      result[j-1] = sqrt(j);
   //printf("From %d Ending The Task\n\n",m->myid);   
   pthread_exit(0);
}


/*
 *
 */
void *Usage(char *argv[]) {

   printf("Usage: %s n k\n", argv[0]);
   exit(1);
}


/*
 *
 */
int main(int argc, char *argv[]) {

   time_t  t0, t1; /* time_t is defined on <time.h> and <sys/types.h> as long */
   clock_t c0, c1; /* clock_t is defined on <time.h> and <sys/types.h> as int */
   pthread_t *thread;
   pthread_attr_t attribute;
   struct Message **m;
   void *exit_status;
   int n, i, k;

   if (argc != 3)
      Usage(argv);
   else {
      
      n = atoi(argv[1]);
      k = atoi(argv[2]);
      thread = calloc(k,sizeof(pthread_t));
      result = calloc(n,sizeof (float *));
      m = calloc(k,sizeof(struct Message *));
      for (i = 0; i < k; i = i + 1)
         m[i] = calloc(1,sizeof(struct Message));
      pthread_attr_init(&attribute);
      pthread_attr_setdetachstate(&attribute,PTHREAD_CREATE_JOINABLE);


      /************************* TimeClock starts ********************************/

      t0 = time(NULL);
      c0 = clock();

      for (i = 0; i < k; i = i + 1) {
         //printf("Main: creating thread %d\n", i);
         m[i]->myid = i;
         m[i]->nvalue = n;
         m[i]->numthreads = k;
         pthread_create(&thread[i],&attribute,Computes,(void *) m[i]);
      }

      


      pthread_attr_destroy(&attribute); 
      for (i = 0; i < k; i = i + 1)
         pthread_join(thread[i],&exit_status);
      //printf("\n\n**************************************\n\n");
         

      t1 = time(NULL);
      c1 = clock();

      printf("tamaño n: %d\nn° threads: %d\n",n,k );
      printf ("elapsed CPU time: %f\n\n", (float) (c1 - c0)/CLOCKS_PER_SEC);

      /************************* Time/Clock ends   ****************************/



      //for (i = 0; i < n; i = i + 1) 
         //printf("%3d - %f\n",i + 1,result[i]);
   }
   return 0;
}
