/******************************************************************************
 * 
 * FILE: stars-2014.c 
 *
 * DESCRIPTION: determines the position of stars of a sky sector taking as 
 *              input the light intensity.      
 * 
 * AUTHOR: Ruben Carvajal Schiaffino
 *
 * LAST REVISED: Santiago de Chile, 30/9/2014
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define STAR    '*'
#define NOSTAR  ' '
#define SILENT  0
#define VERBOSE 1


/*
 * 
 * 
 */
char **Process(unsigned char **sky, int r, int c) {
  
   char **map;
   int i, j;
   float sum, v;
   
   map = calloc(r,sizeof(char *));
   for (i = 0; i < r; i = i + 1)
      map[i] = calloc(c,sizeof(char));
   for (j = 1; j <= r; j = j + 1)
      for (i = 1; i <= c; i = i + 1) {
         sum  = (float)(sky[i][j] + sky[i - 1][j] + sky[i][j + 1] + sky[i + 1][j] + sky[i][j - 1]);
         v = sum / 5.0;
	 if (v > 6.0)
	    map[i-1][j-1] = STAR;
         else
	    map[i-1][j-1] = NOSTAR;
      }
   return map;
}   


/*
 * 
 * 
 */
void PrintMap(char **map, int r, int c) {
  
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
void PrintData(unsigned char **sky, int r, int c) {
  
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
unsigned char **ReadData(int r, int c) {

   int i, j;
   unsigned char **sky;
   
   sky = calloc(r + 2,sizeof(unsigned char *));
   for (i = 0; i < r + 2; i = i + 1)
      sky[i] = calloc(c + 2,sizeof(unsigned char));
   for (i = 1; i <= r; i = i + 1)
      for (j = 1; j <= c; j = j + 1)  
         scanf("%d",&sky[i][j]);
   return sky;
}


/*
 * 
 * 
 */
void Usage(char *message) {
  
   printf("\nUsage: %s -O < datafile",message);
   printf("\n\nO in {S,V}\n\n");
}   
   
   
/*
 * 
 * 
 */
int main(int argc, char **argv) {

   int r, c, mode;
   unsigned char **sky;
   char **map;

   if (argc == 2) {
      if (strcmp(argv[1],"-S") == 0)
	 mode = SILENT;
      if (strcmp(argv[1],"-V") == 0)
         mode = VERBOSE;   
      scanf("%d",&r);
      scanf("%d",&c);
      sky = ReadData(r,c);
      if (mode == VERBOSE)
	 PrintData(sky,r,c);
      map = Process(sky,r,c);
      PrintMap(map,r,c);      
  }
   else
      Usage(argv[0]);
}   
