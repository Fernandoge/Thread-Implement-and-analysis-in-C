#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>


struct Node{
	int data;
	struct Node* next_node;
};
struct NodeDub{
	int data;
	struct NodeDub* next_node_dub;
	struct Node* next_node;
};

void printList(struct Node *n) { 
	printf("[");
  	while (n != NULL){	 
     	printf("%d", n->data); 
     	n = n->next_node; 
  	} 
  	printf("]\n");
} 
void printListDub(struct NodeDub *n) { 
  	while (n != NULL){	 
     	printf("%d", n->data);
     	printList(n->next_node); 
     	n = n->next_node_dub; 
  	} 
} 

void printPowerSet(struct NodeDub *head, int thread, int n, int mode) { 
	//mode = 0: -s
	//mode = 1: -v
	struct Node* temp = NULL;
	struct NodeDub* tempDub = NULL;
	
	// Retrieving size of the set 
	int setSize = 0;
	tempDub = head;
	temp = tempDub->next_node;
	while(temp != NULL){
		temp = temp->next_node;
		setSize++;
	}

	struct Node** pointers = NULL;
	pointers = malloc(n * sizeof(struct Node*));
	tempDub = head;
	for(int i = 0;i < n;i++){
		pointers[i] = tempDub->next_node;
		tempDub = tempDub->next_node_dub;
	}
	for(int i = 0;i < setSize;i++){
		if(mode == 1){printf("Thread %d: {",thread);}
		for(int j = 0;j < n;j++){
			if(pointers[j]->data == 1){
				if(mode == 1){printf(" %d", j+1);}
			}
			pointers[j]=pointers[j]->next_node;
		}
		if(mode == 1){printf(" }\n");}
	}
} 

char *decimal_to_binary(int n, int digits){
   	int d, count;
   	char *pointer;
   
   	count = 0;
   	pointer = (char*)malloc(32+1);
   
   	if (pointer == NULL)
    exit(EXIT_FAILURE);
     
   	for (digits = digits-1;digits >= 0;digits--){
    	d = n >> digits;
     
      	if (d & 1)
        	*(pointer+count) = 1 + '0';
      	else
         	*(pointer+count) = 0 + '0';  
      	count++;
   }
   *(pointer+count) = '\0';
 
   return  pointer;
}

int powpow(int number, int times) {
	if(times == 0) {
		return 1;
	}
	else {
		int new_number = 1;
		for(int i = 0;i < times;i++) {
			new_number = new_number * number;
		}
		return new_number;
	}
}

struct NodeDub* createList(int n, int jMax){
	struct NodeDub* head = NULL;
	struct NodeDub* lastDub = NULL;
	struct NodeDub* tempDub = NULL;
	struct Node* lastNode = NULL;
	struct Node* tempNode = NULL;

	/* CREATING LIST */
	head = (struct NodeDub*)malloc(sizeof(struct NodeDub));
	lastNode = (struct Node*)malloc(sizeof(struct Node));
	head->next_node= lastNode;
	head->data=1;
	for(int i = 1;i < jMax;i++){
  		tempNode = (struct Node*)malloc(sizeof(struct Node));
  		lastNode->next_node = tempNode;
  		lastNode = tempNode;
	}
	lastDub = (struct NodeDub*)malloc(sizeof(struct NodeDub));
	head->next_node_dub = lastDub;
  	
  	
  	for(int i = 2;i < n;i++){
  		tempDub = (struct NodeDub*)malloc(sizeof(struct NodeDub));
  		lastDub->next_node_dub = tempDub;
  		lastDub->data=i;
  		lastNode = (struct Node*)malloc(sizeof(struct Node));
		lastDub->next_node= lastNode;
		for(int i = 1;i < jMax;i++){
	  		tempNode = (struct Node*)malloc(sizeof(struct Node));
	  		lastNode->next_node = tempNode;
	  		//lastDub->next_node = lastNode;
	  		lastNode = tempNode;
		}
  		//lastDub->next_node = lastNode;
  		lastDub = tempDub;

  	}
  	lastNode = (struct Node*)malloc(sizeof(struct Node));
	lastDub->next_node= lastNode;
	lastDub->data=n;
	for(int i = 1;i < jMax;i++){
  		tempNode = (struct Node*)malloc(sizeof(struct Node));
  		lastNode->next_node = tempNode;
  		lastNode = tempNode;
	}
	/* END LIST */
	return head;
}

struct NodeDub* fillPowerSet(struct NodeDub* head, int index_start,int index_end,int n,int jMax){
	//printf("%d %d\n",index_start,index_end );

	struct NodeDub* head_temp = NULL;
	struct Node** pointers = NULL;
	struct Node* pointer_temp = NULL;
	pointers = malloc(n * sizeof(struct Node*));
	//pointers[0] = NULL;
	//printListDub(pointers);
	//printList(pointers[0]);
	//printList(pointers[1]);
	//printList(pointers[2]);
	//printList(pointers[3]);

	head_temp = head;
	for(int i = 0;i < n;i++){
		pointers[i] = head_temp->next_node;
		//printf("%d", head_temp->data);
		head_temp = head_temp->next_node_dub;
	}
	//printf("\n");

	//head_temp = head;
	//printListDub(head_temp);
	char* string_temp = malloc(n * sizeof(char));
	for(int i = index_start-1;i<index_end-1 && i < jMax-1;i++){
		strcpy(string_temp,decimal_to_binary(i+1,n));
		for(int j = 0;j < n;j++){
			//printf("%d", string_temp[j]-'0');
			pointers[j]->data = string_temp[j]-'0';
			pointers[j] = pointers[j]->next_node;
		}
		//printf("%s %d\n", string_temp, i+1);
	}
	return head;
}

struct createListArgs{
	int n;
	int jMax;
	struct NodeDub* ret;
};

struct fillPowerSetArgs{
	struct NodeDub* head;
	int index_start;
	int index_end;
	int n;
	int jMax;
};

void* createListPTHREAD(void *arg){
	int n = ((struct createListArgs*)arg)->n;
	int jMax = ((struct createListArgs*)arg)->jMax; 
	//printf("%d %d\n", n,jMax);

	struct NodeDub* head = NULL;
	struct NodeDub* lastDub = NULL;
	struct NodeDub* tempDub = NULL;
	struct Node* lastNode = NULL;
	struct Node* tempNode = NULL;

	/* CREATING LIST */
	head = (struct NodeDub*)malloc(sizeof(struct NodeDub));
	lastNode = (struct Node*)malloc(sizeof(struct Node));
	head->next_node= lastNode;
	head->data=1;
	for(int i = 1;i < jMax;i++){
  		tempNode = (struct Node*)malloc(sizeof(struct Node));
  		lastNode->next_node = tempNode;
  		lastNode = tempNode;
	}
	lastDub = (struct NodeDub*)malloc(sizeof(struct NodeDub));
	head->next_node_dub = lastDub;
  	
  	
  	for(int i = 2;i < n;i++){
  		tempDub = (struct NodeDub*)malloc(sizeof(struct NodeDub));
  		lastDub->next_node_dub = tempDub;
  		lastDub->data=i;
  		lastNode = (struct Node*)malloc(sizeof(struct Node));
		lastDub->next_node= lastNode;
		for(int i = 1;i < jMax;i++){
	  		tempNode = (struct Node*)malloc(sizeof(struct Node));
	  		lastNode->next_node = tempNode;
	  		//lastDub->next_node = lastNode;
	  		lastNode = tempNode;
		}
  		//lastDub->next_node = lastNode;
  		lastDub = tempDub;

  	}
  	lastNode = (struct Node*)malloc(sizeof(struct Node));
	lastDub->next_node= lastNode;
	lastDub->data=n;
	for(int i = 1;i < jMax;i++){
  		tempNode = (struct Node*)malloc(sizeof(struct Node));
  		lastNode->next_node = tempNode;
  		lastNode = tempNode;
	}
	/* END LIST */
	((struct createListArgs*)arg)->ret = head;
	//return (void*)head;
	return NULL;
}

void* fillPowerSetPTHREAD(void *arg){
	struct NodeDub* head = ((struct fillPowerSetArgs*)arg)->head;
	int index_start = ((struct fillPowerSetArgs*)arg)->index_start;
	int index_end = ((struct fillPowerSetArgs*)arg)->index_end;
	int n = ((struct fillPowerSetArgs*)arg)->n;
	int jMax = ((struct fillPowerSetArgs*)arg)->jMax; 
	//printf("%d %d\n",index_start,index_end );

	struct NodeDub* head_temp = NULL;
	struct Node** pointers = NULL;
	struct Node* pointer_temp = NULL;
	pointers = malloc(n * sizeof(struct Node*));
	//pointers[0] = NULL;
	//printListDub(pointers);
	//printList(pointers[0]);
	//printList(pointers[1]);
	//printList(pointers[2]);
	//printList(pointers[3]);

	head_temp = head;
	for(int i = 0;i < n;i++){
		pointers[i] = head_temp->next_node;
		//printf("%d", head_temp->data);
		head_temp = head_temp->next_node_dub;
	}
	//printf("\n");

	//head_temp = head;
	//printListDub(head_temp);
	char* string_temp = malloc(n * sizeof(char));
	for(int i = index_start-1;i<index_end-1 && i < jMax-1;i++){
		strcpy(string_temp,decimal_to_binary(i+1,n));
		for(int j = 0;j < n;j++){
			//printf("%d", string_temp[j]-'0');
			pointers[j]->data = string_temp[j]-'0';
			//printf("%d", pointers[j]->data);
			pointers[j] = pointers[j]->next_node;

		}
		//printf(" %s %d\n", "", i+1);
	}
	return NULL;
}




int main(int argc, char const *argv[]){
	if(argc == 4 && (!strcmp(argv[1],"-v") || !strcmp(argv[1],"-s"))){
		//argv[1]: MODE [-v|-s]
		//argv[2]: NUMBERS [n]
		int n = atoi(argv[2]);
		int l = atoi(argv[3]);
		int jMax = powpow(2,n);
		int temp;
		
		//Extra vars		
		int value,k,change;
		int thread_elements,thread_elements_mod;
		thread_elements_mod = 0;

		//Defining elemets per thread
		if(jMax%l==0){
			thread_elements = jMax/l;
		}
		else{
			thread_elements = (jMax/l)+1;
			thread_elements_mod = jMax- thread_elements*(l-1);
		}

		//Threads
		pthread_t *threads;
		threads = malloc(l * sizeof(pthread_t));

		//Time calculus vars
		time_t  t0, t1; /* time_t is defined on <time.h> and <sys/types.h> as long */
	  	clock_t c0, c1; /* clock_t is defined on <time.h> and <sys/types.h> as int */


		//START TIME
		t0 = time(NULL);
	  	c0 = clock();

		//Linked lists vars

		struct NodeDub** heads;// = createList(n,jMax);
		heads = malloc(thread_elements * sizeof(struct NodeDub*));

		struct createListArgs* create_list_args;
		create_list_args = malloc(l * sizeof(struct createListArgs));
		for(int i = 0;i < l;i++){
			if(i == l-1 && thread_elements_mod != 0){
				//heads[i] = createList(n,thread_elements_mod);	
				create_list_args[i].n = n;
				create_list_args[i].jMax = thread_elements_mod;
				pthread_create(&threads[i],NULL,createListPTHREAD,&create_list_args[i]);
			}
			else if (thread_elements != 0){
				//heads[i] = createList(n,thread_elements);
				create_list_args[i].n = n;
				create_list_args[i].jMax = thread_elements;
				pthread_create(&threads[i],NULL,createListPTHREAD,&create_list_args[i]);

			}
			pthread_join(threads[i],NULL);
			heads[i] = create_list_args[i].ret;
		}

		for(int i = 0;i < l;i++){
			pthread_join(threads[i],NULL);
			heads[i] = create_list_args[i].ret;
		}
		int j_temp = 0;
		
		for(int i = 0;i < jMax;i=i+thread_elements,j_temp++){
			struct fillPowerSetArgs fill_power_set_args;
			fill_power_set_args.head = heads[j_temp];
			fill_power_set_args.index_start = i;
			fill_power_set_args.index_end = i+thread_elements;
			fill_power_set_args.n = n;
			fill_power_set_args.jMax = jMax;
			pthread_create(&threads[j_temp],NULL,fillPowerSetPTHREAD,&fill_power_set_args);
			pthread_join(threads[j_temp],NULL);
		}
		for(int i = 0;i < l;i++){
			if(strcmp(argv[1],"-v")==0){
				printPowerSet(heads[i],i+1,n,1);
			}
			else if(strcmp(argv[1],"-s")==0){
				printPowerSet(heads[i],i+1,n,0);
			}
		}

		//END TIME
		t1 = time(NULL);
	  	c1 = clock();

	  	/* TIME PRINT */
		printf ("Elapsed wall clock time: %ld Elapsed CPU time: %f Problem size: %d Threads: %d\n", (long) (t1 - t0),(float) (c1 - c0)/CLOCKS_PER_SEC,n,l);
	  	free(heads);
  	}
  	return 0;
}