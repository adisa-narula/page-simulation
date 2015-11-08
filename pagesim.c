#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h" 

/** 
 * Page simulation implements the page replacement policy according to the argument
 * from the command line. Executing page simulation prints out the content of 
 * physical memory for the specified replacement policy as each page is being placed 
 * into memory and whether there was a hit or a miss. **/ 

/**Main method reads in the arguments from the command line and validates them. 
 * The method also calls the page replacement policy as specified by the user. 
 * int arg: the number of arguments in the command line. 
 * char *argv[]: array of pointer of arguments entered through the commnad line. **/ 

int main(int argc, char *argv[]) {
	
	int value, numframe;
	char *method; 
		
	/*validates number of arguments in the command line*/ 
	if (argc != 4) {	 /* argument not found in commnad line */ 
		printf("Error invalid number of parameters for %s\n", argv[0]);
		exit(1);
	}
	
	/*reading input from command line */  
	/********************************************************/ 
	
	method = argv[3]; /*page replacement method */
	if (((strcmp(method, "fifo")) != 0) && ((strcmp(method, "lru")) != 0) && ((strcmp(method, "extra")) != 0)) {
		printf("Incorrect algorithm\nPlease pick either fifo or lru\n");
	}
	
	/*number of physical frames in memory*/ 
	sscanf(argv[1], "%d", &numframe);
	if (numframe < 1 || numframe > 100) {
		printf("The number of physical memory frames must range from 1 to 100\n");
		exit(1);
	}
	
	/*open arg[2] for reading*/ 
	 
	 FILE *file = fopen(argv[2], "r"); 
	 
	 /*intial size of memory to be read in*/ 
	 int maxSize = 1000; /*initial size*/ 
	 
	 int *ref_temp = (int*)malloc(maxSize*sizeof(int)); /*big enough for 1000 numbers*/ 
	 
	 /*memory was not allocated*/ 
	if (ref_temp == NULL) {
		 printf("Could not allocate memory"); 
		 return 0;
	} 
	
	/* failed to open file */ 
	
	if (file == 0) {
		printf("Could not open file %s for reading \n", argv[2]);
		exit(1);
	}
	
	int count = 0, x = 0; 
	
	/*counts the number of page references in the file
	 * and store page reference in local array*/ 

	do {
		x = fscanf(file, "%d", &value);
		if (x > 0) {
			/*if number of page reference exceeds the size of memory
			 * use realloc to make the memory content larger.*/ 
			if (maxSize == count-1) {
				maxSize = maxSize * 2; 
				ref_temp = (int*)realloc(ref_temp, sizeof(int)*maxSize); 
			} 
			/*store page reference in a local array*/ 
			ref_temp[count] = value;
			
			/*increase count*/ 
			count++;
		}
	} while (x > 0);
	
	fclose(file); /*close file*/ 
		
	int i;
	int *ref = (int*)malloc(count * sizeof(int)); 
	
	if (ref_temp == NULL) {
		return 0; /*memory wasn't allocated*/ 
	}  
	 
	/*copy into local array to be passed into methods*/ 
	for (i = 0; i < count; i++) {
		ref[i] = ref_temp[i]; 
	}  
	
	free(ref_temp); /*free temporary array*/ 
	
	/********************************************************/
	int state = 0; 
	
	/*calls the page replacement policy specified by the user*/ 
	
	if ((strcmp(method, "fifo"))== 0) {
		FIFO(&ref[0], numframe, count, state); 
	}
	
	else if ((strcmp(method, "lru")) == 0) {
		LRU(&ref[0], numframe, count, state);
	}
	
	else if ((strcmp(method, "extra")) == 0) {
		extra(&ref[0], numframe, count, state); 
	} 
	
	return 0;
}

