/******************************************************************************
 * 
 * FILE: StarsPar.c
 *
 * DESCRIPTION: determines the position of stars of a sky sector taking as 
 *              input the light intensity. The program uses K threads to 
 *              solve the problem.      
 * 
 * AUTHOR: Fernando Garcia Polgatti

 * LAST REVISED: Santiago de Chile, 26/11/2018
 *
 *****************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define STAR    '*'
#define NOSTAR  ' '
#define SILENT  0
#define VERBOSE 1


struct Message {

   int myid, rvalue, cvalue, size, opmode;
};


unsigned char **sky;
char **map;


/*
 * 
 * 
 */
void *Process(void *p) {
  
   int i, j, k;
   float sum, v;
   struct Message *m;
  
   m = (struct Message *) p;
   if (m->opmode == VERBOSE) {
      printf("\n\n**************************************\n\n");
      printf("From %d Rows = %d Cols = %d Size = %d - Beginning The Task\n\n",m->myid,m->rvalue,m->cvalue,m->size);
   }     
    
   for (j = 1; j <= m->rvalue; j = j + 1)
      for (i = m->cvalue, k = 1; k <= m->size; i = i + 1, i = i + 1) {
         sum  = (float)(sky[i][j] + sky[i - 1][j] + sky[i][j + 1] + sky[i + 1][j] + sky[i][j - 1]);
         v = sum / 5.0;
	 if (v > 6.0)
	    map[i-1][j-1] = STAR;
         else
	    map[i-1][j-1] = NOSTAR;
      } 
   if (m->opmode == VERBOSE)
      printf("From %d Ending The Task\n\n",m->myid);   
    
   pthread_exit(0);
}   


/*
 * 
 * 
 */
void PrintMap(int r, int c) {
  
   int i, j;
 
   printf("\n+++++++++++++++ MAP +++++++++++\n\n");
   for (i = 0; i <= r; i = i + 1)
      printf("-");
   printf("\n");
    for (i = 0; i < r; i = i + 1) {
       printf("|");
       for (j = 0; j < c; j = j + 1)
	  printf("%c",map[i][j]);
       printf("\n");
    }
}


/*
 * 
 * 
 */
void PrintData(int r, int c) {
  
   int i, j;
 
   printf("\n+++++++++++++++ DATA +++++++++++\n\n");
    for (i = 1; i <= r; i = i + 1) {
       for (j = 1; j <= c; j = j + 1)
	  printf(" %2d ",sky[i][j]);
       printf("\n");
    }
}


/*
 * 
 * 
 */
void ReadData(int r, int c) {

   int i, j;
   
   sky = calloc(r + 2,sizeof(unsigned char *));
   for (i = 0; i < r + 2; i = i + 1)
      sky[i] = calloc(c + 2,sizeof(unsigned char));
   //for (i = 1; i <= r; i = i + 1)
     // for (j = 1; j <= c; j = j + 1)  
        // scanf("%d",&sky[i][j]);
}


/*
 * 
 * 
 */
void Usage(char *message) {
  
   printf("\nUsage: %s -O N K",message);
   printf("\n\nO in {S,V}\n\n");
   printf("K: Theads Number\n\n");
}   
   
   
/*
 * 
 * 
 */
int main(int argc, char **argv) {

   int i, k, r, c, mode, s, rem, l;
   pthread_t *thread;
   pthread_attr_t attribute;
   struct Message **m;
   void *exit_status;   

   if (argc == 4) {
      

      if (strcmp(argv[1],"-S") == 0)
      
	     mode = SILENT;
      if (strcmp(argv[1],"-V") == 0)
         mode = VERBOSE;
      k = atoi(argv[3]);
      //scanf("%d",&r);
      //scanf("%d",&c);
      r = c = atoi(argv[2]);
      ReadData(r,c);
      if (mode == 1)
	    PrintData(r,c);
      map = calloc(r,sizeof(char *));
      for (i = 0; i < r; i = i + 1)
         map[i] = calloc(c,sizeof(char));      
      thread = calloc(k,sizeof(pthread_t));
      m = calloc(k,sizeof(struct Message *));
      for (i = 0; i < k; i = i + 1)
         m[i] = calloc(1,sizeof(struct Message));
      pthread_attr_init(&attribute);
      pthread_attr_setdetachstate(&attribute,PTHREAD_CREATE_JOINABLE);
      s = c / k;
      rem = c % k;
      l = 1;
      

      time_t  t0, t1; /* time_t is defined on <time.h> and <sys/types.h> as long */
      clock_t c0, c1; /* clock_t is defined on <time.h> and <sys/types.h> as int */

      printf ("using UNIX function time to measure wallclock time ... \n");
      printf ("using UNIX function clock to measure CPU time ... \n");

      t0 = time(NULL);
      c0 = clock();

      printf ("\tbegin (wall):            %ld\n", (long) t0);
        printf ("\tbegin (CPU):             %d\n", (int) c0);


      for (i = 0; i < k; i = i + 1) {
	          if (mode == VERBOSE) 
                printf("Main: creating thread %d\n", i);
	          m[i]->myid = i;
            m[i]->rvalue = r;
	         if (rem != 0) {
    	         m[i]->size = s + 1;
    	          rem = rem - 1;
          	 }
        	 else
        	    m[i]->size = s;
    	    m[i]->cvalue = l;
    	    m[i]->opmode = mode;
    	    l = l + m[i]->size;


         pthread_create(&thread[i],&attribute,Process,(void *) m[i]);


      }      

       

      pthread_attr_destroy(&attribute); 

      

      for (i = 0; i < k; i = i + 1)
         pthread_join(thread[i],&exit_status);
      //PrintMap(r,c);      
      t1 = time(NULL);
      c1 = clock();

      printf ("\tend (wall):              %ld\n", (long) t1);
      printf ("\tend (CPU);               %d\n", (int) c1);
      printf ("\telapsed wall clock time: %ld\n", (long) (t1 - t0));
      printf ("\telapsed CPU time:        %f\n", (float) (c1 - c0)/CLOCKS_PER_SEC);
     

      
  }
   else
      Usage(argv[0]);
   return 0;
}   

     