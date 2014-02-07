/**
 * Object related functions
 *
 * @author 	Daniel Hong
 * 
 * This program is licensed under the GNU GENERAL PUBLIC LICENSE Version 2.
 * A LICENSE file should have accompanied this program.
 */
 
#include <stdlib.h>
#include "../include/class.h"

void release(void *ptr)
{
    free(ptr);
    ptr = NULL;
}
