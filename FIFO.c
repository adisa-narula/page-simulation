#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** 
 * Implements first in first out page replacement policy by evicting the page
 * that has been in memory for the longest time (the first one that was placed in memory)
 * when physical memory is full. FIFO is implemented using an array-based queue that keeps
 * track of which page was the first one that was placed in memory. When a page fault occurs
 * the simulation replaces the page that has been in memory for the longest amount of time. **/            

void enqueue(int page, int *current, int mem_ref[], int *size, int capacity); 
int foundItem(int item, int mem_ref[], int capacity); 
void printMem(int item, int mem_ref[], int found, int size, int capacity); 

/**
 * FIFO method works as a driver for FIFO implementation. The function loops through page memory 
 * references and searches whether it is already in physical memory. If a page fault occurs
 * the method calls one of the two possible methods to place the page in memory depending 
 * on whether the physical memory is full. If physical memory is full, the page is evicted using 
 * FIFO policy, otherwise the page is added to the back of the array. The method returns missRate
 * to the caller. 
 * int pages[] : contains memory references entered through command line 
 * pageCapacity: the number of physical memory frames 
 * numPages: the total number of memory references
 * state: a flag of which process is calling FIFO for printing reasons **/ 
  
float FIFO(int pages[], int pageCapacity, int numPages, int state) {
	
	/* initialize variables */  
	int capacity = pageCapacity; 
	int count = numPages;
	int size = 0; 
	float misses = 0; 
	float references = 0;  
	int current = capacity - 1;
	int stat = state; /*keep status whether its calling fifostat or not*/ 

	int mem_ref[capacity]; 
	int i, found; 
	
	/* initializes content of mem_ref queue to -1
	 * to indicate wheter the posistion in physical memory is occupied*/ 
	 
	for (i = 0; i < capacity; i++) {
		mem_ref[i] = -1; 
	}
	
	/* loops through all of the page memory references in pages[] array, 
	 * checks whether page is in memory and calls the corresponding method*/ 
	for (i = 0; i < count; i++) { 
		
		/*physical memory is full start counting*/ 
		if (size >= capacity) {
			references++;
		} 
		
		/*returns 1 if page is found in memory*/ 
		found = foundItem(pages[i], &mem_ref[0], capacity); 
			
		if (found) { 
			if(!stat) { /*does not print if page stat calls FIFO*/ 
			printMem(pages[i], &mem_ref[0], found, size, capacity);
			} 
		} 
		else { /*page fault occurs*/ 
			enqueue(pages[i], &current, &mem_ref[0], &size, capacity);
			if (!stat) { 
			printMem(pages[i], &mem_ref[0], found, size, capacity);
			}
			if (size > capacity) {
				misses++; /*memory is full start counting misses*/ 
			}
		}	 
	} 
	
	/*calculates miss rate and prints it accordingly*/ 
	float missRate = (misses/references)*100; 
	
	/*FIFO called by page simulation*/ 
	if (stat == 0) {
		printf("Miss rate = %0.0f / %0.0f = %0.2f%%\n", misses, references, missRate);
	}
	/*FIFO called by page statistics*/ 
	else if (stat == 1) {
		printf("FIFO, %2d frames: Miss rate = %0.0f / %0.0f = %0.2f%%\n", pageCapacity, misses, references, missRate);
	}
	
	/*return miss rate to caller*/ 
	return missRate; 	
}

/**
 * Enqueue keeps track of the position where the next page reference should be placed
 * in physical memory. It is implemented using queue data structure. 
 * int page: current page being referenced. 
 * int *current: keeps track of where the next page reference should be placed
 * int mem_ref[]: array that represents physical memory. 
 * int *size: keeps track of how many pages are in memory and increments one everytime
 * int capacity: capacity of physical memory. **/ 
 
void enqueue(int page, int *current, int mem_ref[], int *size, int capacity) { 
	
	int temp = *current; 
	temp = ((*current)+1)%capacity; /*increment current position by one*/ 
	
	/*replaces page that was placed in memory first*/ 
	mem_ref[temp] = page; 
	*current = temp;
	
	/*increment size of pages in memory by one*/ 
	*size = (*size) + 1;  
}

/**
 * Found item method traverses through the array to find whether 
 * there is a page fault. Returns 0 if the page is not in memory. 
 * int item: current page being referenced. 
 * int mem_ref[]: array represents physcial memory. 
 * int capacity: capacity of physical memory. **/ 
 
int foundItem(int item, int mem_ref[], int capacity) {
	
	int i = 0; 
	/*traverses through physical memory*/ 
	for (i = 0; i < capacity; i++) {
		if (mem_ref[i] == item) {
			/*found*/ 
			return 1; 
		} 
	} 
	/*page fault*/ 
	return 0; 
} 

/** Prints the current content of physical memory according to whether there is a miss or a hit. 
 * If there was a memory hit and memory is full print indicates that there was a page fault. 
 * int item: current page being referenced. 
 * int mem_ref[]: array representing physical memory. 
 * int found: flag whether the page has been located in memory. 
 * int size: current size of physical memory. 
 * int capacity: capacity of physical memory. **/ 
void printMem(int item, int mem_ref[], int found, int size, int capacity) {
	
	int i; 
	printf("%2d: [", item); 
	/* prints content of physical memory*/ 
	for (i = 0; i < capacity; i++) {
		if (mem_ref[i] > 99 || mem_ref[i] < 0) {
			printf("  |"); /*content of memory is empty*/ 
		}
		else {
			printf("%2d|", mem_ref[i]); 
		} 
	}
	
	/*indicates that a page fault occured*/ 
	if (!found && (size > capacity)) {
		printf("]  F\n"); 
	}
	/*page found in memory*/ 
	else {
	printf("]\n"); }
}

	
	
	
	
	


