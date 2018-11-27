 /**************************************************************************
 * AUTHOR: Fernando Garcia Polgatti

 * LAST REVISED: Santiago de Chile, 26/11/2018
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>


struct Nodo{
	int data;
	struct Nodo* next_node;
};
struct DubNodo{
	int data;
	struct DubNodo* next_node_dub;
	struct Nodo* next_node;
};

void printList(struct Nodo *n) { 
	printf("[");
  	while (n != NULL){	 
     	printf("%d", n->data); 
     	n = n->next_node; 
  	} 
  	printf("]\n");
} 

void imprimirPowerSet(struct DubNodo *head, int thread, int n, int mode) { 
	struct Nodo* temp = NULL;
	struct DubNodo* tempDub = NULL;
	
	int setSize = 0;
	tempDub = head;
	temp = tempDub->next_node;
	while(temp != NULL){
		temp = temp->next_node;
		setSize++;
	}

	struct Nodo** pointers = NULL;
	pointers = malloc(n * sizeof(struct Nodo*));
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

struct DubNodo* createList(int n, int jMax){
	struct DubNodo* head = NULL;
	struct DubNodo* lastDub = NULL;
	struct DubNodo* tempDub = NULL;
	struct Nodo* lastNode = NULL;
	struct Nodo* tempNode = NULL;

	head = (struct DubNodo*)malloc(sizeof(struct DubNodo));
	lastNode = (struct Nodo*)malloc(sizeof(struct Nodo));
	head->next_node= lastNode;
	head->data=1;
	for(int i = 1;i < jMax;i++){
  		tempNode = (struct Nodo*)malloc(sizeof(struct Nodo));
  		lastNode->next_node = tempNode;
  		lastNode = tempNode;
	}
	lastDub = (struct DubNodo*)malloc(sizeof(struct DubNodo));
	head->next_node_dub = lastDub;
  	
  	
  	for(int i = 2;i < n;i++){
  		tempDub = (struct DubNodo*)malloc(sizeof(struct DubNodo));
  		lastDub->next_node_dub = tempDub;
  		lastDub->data=i;
  		lastNode = (struct Nodo*)malloc(sizeof(struct Nodo));
		lastDub->next_node= lastNode;
		for(int i = 1;i < jMax;i++){
	  		tempNode = (struct Nodo*)malloc(sizeof(struct Nodo));
	  		lastNode->next_node = tempNode;
	  		lastNode = tempNode;
		}
  		lastDub = tempDub;

  	}
  	lastNode = (struct Nodo*)malloc(sizeof(struct Nodo));
	lastDub->next_node= lastNode;
	lastDub->data=n;
	for(int i = 1;i < jMax;i++){
  		tempNode = (struct Nodo*)malloc(sizeof(struct Nodo));
  		lastNode->next_node = tempNode;
  		lastNode = tempNode;
	}

	return head;
}

struct DubNodo* fillPowerSet(struct DubNodo* head, int index_start,int index_end,int n,int jMax){

	struct DubNodo* head_temp = NULL;
	struct Nodo** pointers = NULL;
	struct Nodo* pointer_temp = NULL;
	pointers = malloc(n * sizeof(struct Nodo*));


	head_temp = head;
	for(int i = 0;i < n;i++){
		pointers[i] = head_temp->next_node;
		head_temp = head_temp->next_node_dub;
	}
	char* string_temp = malloc(n * sizeof(char));
	for(int i = index_start-1;i<index_end-1 && i < jMax-1;i++){
		strcpy(string_temp,decimal_to_binary(i+1,n));
		for(int j = 0;j < n;j++){
			pointers[j]->data = string_temp[j]-'0';
			pointers[j] = pointers[j]->next_node;
		}
	}
	return head;
}

struct createListArgs{
	int n;
	int jMax;
	struct DubNodo* ret;
};

struct fillPowerSetArgs{
	struct DubNodo* head;
	int index_start;
	int index_end;
	int n;
	int jMax;
};

void* createListPTHREAD(void *arg){
	int n = ((struct createListArgs*)arg)->n;
	int jMax = ((struct createListArgs*)arg)->jMax; 
	//printf("%d %d\n", n,jMax);

	struct DubNodo* head = NULL;
	struct DubNodo* lastDub = NULL;
	struct DubNodo* tempDub = NULL;
	struct Nodo* lastNode = NULL;
	struct Nodo* tempNode = NULL;

	/* CREATING LIST */
	head = (struct DubNodo*)malloc(sizeof(struct DubNodo));
	lastNode = (struct Nodo*)malloc(sizeof(struct Nodo));
	head->next_node= lastNode;
	head->data=1;
	for(int i = 1;i < jMax;i++){
  		tempNode = (struct Nodo*)malloc(sizeof(struct Nodo));
  		lastNode->next_node = tempNode;
  		lastNode = tempNode;
	}
	lastDub = (struct DubNodo*)malloc(sizeof(struct DubNodo));
	head->next_node_dub = lastDub;
  	
  	
  	for(int i = 2;i < n;i++){
  		tempDub = (struct DubNodo*)malloc(sizeof(struct DubNodo));
  		lastDub->next_node_dub = tempDub;
  		lastDub->data=i;
  		lastNode = (struct Nodo*)malloc(sizeof(struct Nodo));
		lastDub->next_node= lastNode;
		for(int i = 1;i < jMax;i++){
	  		tempNode = (struct Nodo*)malloc(sizeof(struct Nodo));
	  		lastNode->next_node = tempNode;
	  		lastNode = tempNode;
		}
  		lastDub = tempDub;

  	}
  	lastNode = (struct Nodo*)malloc(sizeof(struct Nodo));
	lastDub->next_node= lastNode;
	lastDub->data=n;
	for(int i = 1;i < jMax;i++){
  		tempNode = (struct Nodo*)malloc(sizeof(struct Nodo));
  		lastNode->next_node = tempNode;
  		lastNode = tempNode;
	}
	((struct createListArgs*)arg)->ret = head;
	return NULL;
}

void* fillPowerSetPTHREAD(void *arg){
	struct DubNodo* head = ((struct fillPowerSetArgs*)arg)->head;
	int index_start = ((struct fillPowerSetArgs*)arg)->index_start;
	int index_end = ((struct fillPowerSetArgs*)arg)->index_end;
	int n = ((struct fillPowerSetArgs*)arg)->n;
	int jMax = ((struct fillPowerSetArgs*)arg)->jMax; 
	struct DubNodo* head_temp = NULL;
	struct Nodo** pointers = NULL;
	struct Nodo* pointer_temp = NULL;
	pointers = malloc(n * sizeof(struct Nodo*));

	head_temp = head;
	for(int i = 0;i < n;i++){
		pointers[i] = head_temp->next_node;
		head_temp = head_temp->next_node_dub;
	}
	char* string_temp = malloc(n * sizeof(char));
	for(int i = index_start-1;i<index_end-1 && i < jMax-1;i++){
		strcpy(string_temp,decimal_to_binary(i+1,n));
		for(int j = 0;j < n;j++){
			pointers[j]->data = string_temp[j]-'0';
			pointers[j] = pointers[j]->next_node;

		}
	}
	return NULL;
}




int main(int argc, char const *argv[]){
	if(argc == 4 && (!strcmp(argv[1],"-v") || !strcmp(argv[1],"-s"))){
		int n = atoi(argv[2]);
		int l = atoi(argv[3]);
		int jMax = powpow(2,n);
		int temp;
		
		int value,k,change;
		int thread_elements,thread_elements_mod;
		thread_elements_mod = 0;

		if(jMax%l==0){
			thread_elements = jMax/l;
		}
		else{
			thread_elements = (jMax/l)+1;
			thread_elements_mod = jMax- thread_elements*(l-1);
		}

		pthread_t *threads;
		threads = malloc(l * sizeof(pthread_t));

		time_t  t0, t1; 
	  	clock_t c0, c1; 


		t0 = time(NULL);
	  	c0 = clock();

		struct DubNodo** heads;
		heads = malloc(thread_elements * sizeof(struct DubNodo*));

		struct createListArgs* create_list_args;
		create_list_args = malloc(l * sizeof(struct createListArgs));
		for(int i = 0;i < l;i++){
			if(i == l-1 && thread_elements_mod != 0){
		
				create_list_args[i].n = n;
				create_list_args[i].jMax = thread_elements_mod;
				pthread_create(&threads[i],NULL,createListPTHREAD,&create_list_args[i]);
			}
			else if (thread_elements != 0){
				
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
				imprimirPowerSet(heads[i],i+1,n,1);
			}
			else if(strcmp(argv[1],"-s")==0){
				imprimirPowerSet(heads[i],i+1,n,0);
			}
		}

		t1 = time(NULL);
	  	c1 = clock();
	  	free(heads);
  	}
  	return 0;
}