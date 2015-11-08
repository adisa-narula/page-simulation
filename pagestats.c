#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h" 

/** 
 * Page statistics runs through all the page replacement policies with different
 * number of physical memory frames as the argument. It then prints out the miss rate
 * for different numbers of physical memory frame. The method reads the content of 
 * the command line and writes the results for each page repalce policy to a file
 * called "pagerates.txt". **/ 

/**Main method reads in the arguments from the command line and validates them. 
 * The method also calls different page replacement policies with different number of
 * physical frames as the argument.
 * int arg: the number of arguments in the command line. 
 * char *argv[]: array of pointer of arguments entered through the commnad line. **/ 
 
int main(int argc, char *argv[]) {
	
	/*variables to store arguement from command line*/ 
	int value, min, max, step;  
	
	
	/*validates number of arguments*/ 
	if (argc != 5) {	  
		printf("Error invalid number of parameters for %s\n", argv[0]);
		exit(1); /*exits*/ 
	}
	
	/*reading input from command line */  
	/********************************************************/ 
	
	/*reads in minimum and maximum for number of physical memory frames*/ 
	sscanf(argv[1], "%d", &min);
	sscanf(argv[2], "%d", &max);
	
	/*validates arguments*/ 
	if ((min < 2 && min > 100) && (max < 2 && max > 100)) {
		printf("The range number of physical memory frames must be between 2 and 100\n");
		exit(1);
	}
	
	sscanf(argv[3], "%d", &step);
	if (step < 1) {
		printf("The frame number increment must be greater than 0\n");
		exit(1);
	}
	
	/*open arg[4] for reading*/ 
	 
	FILE *file = fopen(argv[4], "r"); 
	 
	 int maxSize = 1000; /*initial size of memory to be read in*/ 
	 
	 
	 /*temporary pointer to content of page references that are read in from 
	  * file*/ 
	  
	 int *ref_temp = (int*)malloc(maxSize*sizeof(int)); /*big enough for 1000 numbers*/ 
	 
	if (ref_temp == NULL) {
		 printf("Could not allocate memory"); 
		 return 0;
	} 
	
	/* failed to open file */ 
	
	if (file == 0) {
		printf("Could not open file %s for reading \n", argv[4]);
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
	
	/*intialize another array*/ 
	int *ref = (int*)malloc(count * sizeof(int)); 
	
	if (ref_temp == NULL) {
		return 0; /*memory wasn't allocated*/ 
	}  
	 
	/*copy into local array*/ 
	for (i = 0; i < count; i++) {
		ref[i] = ref_temp[i]; 
	}  
	
	 /*free temporary array incase it is too large*/
	free(ref_temp); 
	
	/********************************************************/
	
	/*open file for reading*/ 
	FILE *f = fopen("pagerates.txt", "w");
	int state = 1; 
	
	/*cannot open file */ 
	if (f == NULL) {
		printf("Error opening write file\n"); 
		exit(1);
	}
	
	/*initilize variables to keep track of number of physical frames*/ 
	int j = min, k = min, l = min;
	float rate; 
	
	fprintf(f, "\n");
	fprintf(f, "LRU  "); 
	
	/*The three different while loops loops throuh all of the replacement policies
	 * then writes the result to a file*/ 
	
	/*calls LRU*/ 
	while (j <= max) {
		rate = LRU(&ref[0], j, count, state);
		fprintf(f, "%6.2f",rate);
		j+=step;
	}
	
	fprintf(f,"\n"); 
	fprintf(f, "FIFO "); 
	
	printf("\n");
	
	/*calls FIFO*/ 
	while (k <= max) {
		rate = FIFO(&ref[0], k, count, state);
		fprintf(f, "%6.2f", rate);
		k+=step;
	}
	
	fprintf(f,"\n"); 
	fprintf(f, "Random "); 
	
	printf("\n");
	
	/*calls random page replacement*/ 
	while (l <= max) {
		rate = extra(&ref[0], l, count, state);
		fprintf(f, "%6.2f", rate);
		l+=step;
	}
	
	/*done writing to file*/ 
	fclose(f);
	
	return 0;		
}
