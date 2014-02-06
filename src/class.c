/**
 * Object related functions
 *
 * @author 	Daniel Hong
 */
 
#include <stdlib.h>
#include "../include/class.h"

void release(void *ptr)
{
    free(ptr);
    ptr = NULL;
}
