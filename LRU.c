#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**Implements least recently used (LRU) page replacement policy. The LRU policy replaces 
 * the page that has been in the memory for the longest time without having been referenced.
 * The implementation works by keeping an internal "clock" or count that increases 
 * every time memory is referenced. If a page in memory is referenced again the clock is 
 * reset to 0. When a page fault occurs the replacement method evicts the page in memory 
 * with the highest clock count. The policy is implemented using a doubly linked list. **/ 
 
 
/** Each node contains the page number, an internal clock count and two pointers that
 * points to the next node and the previous node. **/ 
 
struct node { 
	int page;
	int count;  /*works like a clock, tracks how long the page has been in memory */ 
	struct node *next;
	struct node *previous;  
}; 

int search(struct node **head, int item); 
void swap(struct node **head, int item);
void addBack(struct node **head, int item, int *sizeL);
void print(struct node **head, int item, int hit, int capacityL); 


/** LRU method is a driver for implementing LRU policy. It traverses the page references
 * and determines whether there was a page hit or miss and calls the corresponding methods. 
 * int pages[] : contains memory references entered through command line.  
 * pageCapacity: the number of physical memory frames. 
 * numPages: the total number of memory references. 
 * state: a flag of which process is calling FIFO for printing reasons.*/ 

float LRU(int pages[], int pageCapacity, int numPages, int state) {
	
	/*initalizing variables*/ 
	int capacityL = pageCapacity; 
	int countL = numPages; 
	int sizeL = 0; 
	float missesL = 0; 
	float referencesL = 0; 
	int status = state; 
	
	int i;  
	int found; 
	
	/*initialize head of doubly linked list*/ 
	struct node *head = NULL;
	
	
	for (i = 0; i < countL; i++) { 
		
		/*memory is full start counting references*/ 
		if (sizeL >= capacityL) {
			referencesL++;
		}
		
		/*returns 0 if there is a page fault*/ 
		found = search(&head, pages[i]); 
		if (found) {
			if (!status) {/* does not print if page stats is calling LRU*/ 
			print(&head, pages[i], 1, capacityL);} 
		}
		else {
			if (sizeL < capacityL) { /*has not yet exceeded capacity*/ 
				/*add to the back of physical memory*/ 
				addBack(&head, pages[i], &sizeL);
				if (!status) {
				print(&head, pages[i],1, capacityL);} 
			}
			else { /*page fault and memory is full*/ 
				swap(&head, pages[i]); /*swaps out least recently used page in memory*/ 
				if (!status){
				print(&head, pages[i],0, capacityL); }
				missesL++; /*count page miss*/ 
			}
		}
	}
	
	
	/*calculates miss rate and prints according to which method is calling LRU*/ 
	
	float missRate = (missesL/referencesL)*100;
	
	if (status == 0) { /*page simulation calling LRU*/ 
		printf("Miss rate = %0.0f / %0.0f = %0.2f%%\n", missesL, referencesL, missRate);
	}
	else if (status == 1) { /*page statistics calling LRU*/ 
		printf("LRU, %2d frames: Miss rate = %0.0f / %0.0f = %0.2f%%\n", pageCapacity, missesL, referencesL, missRate);
	} 
	
	/*returns miss rate*/ 
	return missRate; 
}

/** Traverses the content of memory to determine whether there is a page miss or hit. 
 *  It incremements the time count for each page that is currently in memory. If a page is found
 * 	it's internal clock is reset to 0. The method returns 0 if there is a page fault. 
 *  struct node **head: pointer to the head of the doubly linked list. 
 *  int item: current page being referenced*/ 
 
int search(struct node **head, int item) { 
	
	/*set flag*/ 
	
	int found = 0; 
	
	/*empty list return 0 */ 
	if ((*head) == NULL) {
		return found;
	} 
	
	/*set current pointer to the head of the list*/ 
	struct node *current = (*head);
	
	/*while there are more elements in the list*/ 
	while (current != NULL) {
		
		
		/*increment the time that the page has been in memory*/ 
		(current->count) = (current->count) + 1; 
		
		if ((current->page) == item) {
			
			/*the page is used again reset it*/ 
			(current->count) = 0; 
			found = 1; /*page found*/ 
		}
		current = current->next; 
	}
	return found; 
}


/**
 * The swap method is called when there is a page fault and the memory is full. It replaces the
 * page in memory that has been least recently used in memory. The method traverses the linked list
 * to find the page that has been in memory for the longest time without having been referenced. 
 * Once the method finds the page, the new page is swapped in. 
 * struct node **head: pointer to the head of the doubly linked list. 
 * int item: current page being referenced*/ 
 
void swap(struct node **head, int item) {
	
	
	/*empty list*/ 
	if (head == NULL) {
		return;  
	} 
	
	/*assigns pointer to the new page that is going to be inserted*/ 
	
	struct node *n = (struct node *)malloc(sizeof(struct node)); 
	
	if (n == NULL) {
		return; /*memory hasn't been allocated */ 
	}
	
	/*start at the beginning*/ 
	struct node *current = (*head);
	struct node *remove = (*head);
	
	while ((current->next) != NULL) {
		/*compare the count to find maximum*/  
		if ((remove->count) < ((current->next)->count)) { 
			/*move the remove pointer*/
			remove = current->next; 
		}
		current = current->next;
	}
	
	/*initialize content of the new node being added*/ 
	(n->count) = 0; 
	(n->page) = item;
	
	if ((remove->previous) != NULL) {
		(remove -> previous)->next = n; 
	}
	else {
		(*head) = n; /*change head pointer to the first element*/  
	} 
		
	if ((remove->next) != NULL) {
		(remove->next)->previous = n;
	} 
	
	(n->previous) = (remove->previous);
	(n->next) = (remove -> next); 
	
	/*free the page that has been removed*/ 
	free(remove); 	
}

/** 
 * Add back method is called when there is a page fault by memory content is not yet full. 
 * The method adds the new page to the back of the linked list and increases the size
 * of the pages in physical memory. 
 * struct node **head: pointer to the beginning of the doubly linked list. 
 * int item: current page being referenced. 
 * int *sizeL: keeps track of the how many pages are currently in memory. **/ 
 
void addBack(struct node **head, int item, int *sizeL) {
	
	/*new page to be added to the linked list*/ 
	struct node *n = (struct node *)malloc(sizeof(struct node)); 
	
	if (n == NULL) {
		return; 
	}
	
	/*adding the first item*/ 
	if ((*head) == NULL) { 
		(n->next) = (*head); 
		(*head) = n; 
		(n->page) = item;
		(n->count) = 0; /*set count to 0*/ 
		(n->next) = NULL;
		(n->previous) = NULL; 	
	} 
	else {
		
		/*traverses to the back of the list*/ 
		struct node *current = (*head);
		while ((current->next) != NULL) {
			current = current->next;
		}
		/*initilized content of the new page*/ 
		(n->page) = item; 
		(n->count) = 0; /*set count to 0*/ 
		(current->next) = n;
		(n->previous) = current; 
		(n->next) = NULL;  
	} 
	/*increment size by one*/ 
	*sizeL = (*sizeL) + 1;
}

/**Print the current content of physical memory depending on whether there 
 * was a page hit or miss. 
 * struct **head: pointer to the beginning of the doubly linked list. 
 * int teim: current item being referenced. 
 * int hit: flag that indicates wheter there was a hit or a miss. 
 * int capacityL: capacity of physical memory. **/ 
 
void print(struct node **head, int item, int hit, int capacityL) {
	
	/*nothing to print*/ 
	if ((*head) == NULL) {
		return; 
	} 
	 
	int i = 0; 
	printf("%2d: [", item); 
	
	/*start at the begnning of the list*/ 
	struct node* current = (*head); 
	
	/*prints memory content*/ 
	while (current != NULL) {
		printf("%2d|", current->page); 
		current = current->next; 
		i++;
	} 
	
	/*if size is less than capacity print empty content of memory*/ 
	while (i < capacityL) {
		printf("%2s|",""); 
		i++;
	}
	/*page fault*/ 
	if (!hit) {
		printf("]  F\n"); }
	
	/*page hit*/ 
	else {
		printf("]\n"); }
}




