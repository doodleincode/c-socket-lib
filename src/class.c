/**
 * Object related functions
 *
 * @author 	Daniel Hong
 * 
 * GNU GENERAL PUBLIC LICENSE Version 2. A LICENSE file should have
 * accompanied this library.
 */
 
#include <stdlib.h>
#include "../include/class.h"

void release(void *ptr)
{
    free(ptr);
    ptr = NULL;
}
