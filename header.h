
/**Three different page replacement policies called by page simulation and 
 * page statistics. FIFO implements the first in first out policy, LRU implements
 * least recently used and the extra method implements the random place replacement policy*/ 

float FIFO(int pages[], int pageCapacity, int numPages, int state); 
float LRU(int pages[], int pageCapacity, int numPages, int state); 
float extra(int pages[], int numCapacity, int numPages, int state); 




