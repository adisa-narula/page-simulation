#include <stdlib.h>
#include <string.h>
#include <stdio.h> 

/**
 * The extra credit page replacement policy replaces page by picking a random 
 * location in memory and replacing it with the new page. The advantage of implementing
 * a random page replacement policy is that it is easy to implement and reduces a lot of overhead
 * cause that is required to traverse the memory to find which page should be evicted.
 * In addition, random page replacements performs equally well as FIFO and LRU and is 
 * much easier to implement. However, the downside of this page replacement algorithm is that
 * it could potentially replace a page that is consistently used in memory. 
 * The random page replacement policy is impleted using an array. A random number generator
 * decides which page should be evicted from memory*/ 

void printing(int item, int mem_ref[], int found, int size_e, int capacity_e); 
void replace(int mem_ref[], int item, int capacity_e); 
int foundIt(int item, int mem_ref[], int capacity_e); 

/**
 * Extra method is a driver for implementing the random page replacement policy. 
 * It traverses the pages that are referenced and calls the necessary methods depending
 * on whethere there was a page miss or hit. 
 * int pages[] : contains memory references entered through command line.  
 * pageCapacity: the number of physical memory frames. 
 * numPages: the total number of memory references. 
 * state: a flag of which process is calling FIFO for printing reasons.**/ 
 
float extra(int pages[], int numCapacity, int numPages, int state) {
	
	/*initializing variables*/ 
	int capacity_e = numCapacity;
	int count_e = numPages;
	int size_e = 0; 
	float misses_e = 0;
	float references_e = 0;  
	int stat_e = state; /*keep status whether its calling fifostat or not*/ 
		
	int found, i; 
	int mem_ref[capacity_e]; 
	
	/*intializes content of physical memory*/ 
	for (i = 0; i < capacity_e; i++) {
		mem_ref[i] = -1; /*initialize content of mem_ref*/ 
	}
	
	/* traverse through the page references*/ 
	for (i = 0; i < count_e; i++) { 
		
		/*start counting references when memory is full*/ 
		if (size_e >= capacity_e) {
			references_e++;
		} 
		
		/*returns 0 if there is a page fault*/ 
		found = foundIt(pages[i], &mem_ref[0], capacity_e);	
		if (found) { 
			if(!stat_e) {/*prints memory content if page simulation calls extra*/ 
			printing(pages[i], &mem_ref[0], found, size_e, capacity_e);
			} 
		} 
		else {
			/*memory isn't not yet full*/ 
			if (size_e < capacity_e) {
				mem_ref[size_e] = pages[i];
				size_e++; /*just add at the back*/ 
			}
			else {
				/*memory is full*/ 
				replace(&mem_ref[0], pages[i], capacity_e); 
				size_e++; 
				misses_e++; /*increase number of misses*/ 
			}
			if (!stat_e) {
			printing(pages[i], &mem_ref[0], found, size_e, capacity_e);
			}
		}	 
	}
	
	/*calculates the miss rate and prints the appropriate information*/ 
	float missRate = (misses_e/references_e)*100;
	
	if (stat_e == 0) {
		printf("Miss rate = %0.0f / %0.0f = %0.2f%%\n", misses_e, references_e, missRate);
	}
	
	else if (stat_e == 1) {
		printf("Random, %2d frames: Miss rate = %0.0f / %0.0f = %0.2f%%\n", numCapacity, misses_e, references_e, missRate);
	}
	
	/*returns miss rate*/ 
	return missRate; 
} 

/**
 * Traverse the content of physical memory to determine whether there is a page
 * hit or miss. The method returns 0 if there is a page miss. 
 * int item: current item being referenced. 
 * int mem_ref: physical memory
 * int capacity_e: capacity of physical memory*/ 
 
int foundIt(int item, int mem_ref[], int capacity_e) {
	
	int i = 0; 
	/*traverses content of physical memory*/ 
	for (i = 0; i < capacity_e; i++) {
		if (mem_ref[i] == item) {
			return 1; /*page hit*/ 
		} 
	} 
	/*page fault*/ 
	return 0; 
}      

/** 
 * Replace method is called when there is a page fault. It generates a random number
 * which indicates the index of the page in memory to be replaced with the new page.
 * int mem_ref[]: content of physical memory
 * int item: current page being referenced. 
 * int capacity_e: capacity of physical memory. **/ 
 
void replace(int mem_ref[], int item, int capacity_e) {
	
	/*generating random number*/ 
	int result = random() % capacity_e;  

	/*replacing page*/ 
	mem_ref[result] = item; 
} 

/** Prints the current content of physical memory according to whether there is a miss or a hit. 
 * If there was a memory hit and memory is full print indicates that there was a page fault. 
 * int item: current page being referenced. 
 * int mem_ref[]: array representing physical memory. 
 * int found: flag whether the page has been located in memory. 
 * int size_e: current size of physical memory. 
 * int capacity_e: capacity of physical memory. **/ 

void printing(int item, int mem_ref[], int found, int size_e, int capacity_e) {
	
	int i; 
	printf("%2d: [", item); 
	
	/*prints the content of physical memory*/ 
	for (i = 0; i < capacity_e; i++) {
		if (mem_ref[i] > 99 || mem_ref[i] < 0) {
			printf("  |"); /*empty*/ 
		}
		else {
			/*print page*/ 
			printf("%2d|", mem_ref[i]); 
		} 
	}
	
	/*page fault and memory is full*/ 
	if (!found && (size_e > capacity_e)) {
		printf("]  F\n");
	}
	else { /*page hit*/ 
	printf("]\n");
	}
} 

