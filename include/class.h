/**
 * Macros for creating classes and methods, and instantiating/releasing objects
 *
 * @author 	Daniel Hong
 * 
 * This program is licensed under the GNU GENERAL PUBLIC LICENSE Version 2.
 * A LICENSE file should have accompanied this program.
 */

#ifndef _DH_CLASS_H_
#define _DH_CLASS_H_

// Define a class
#define CLASS( _NAME_, _BODY_ ) typedef struct _NAME_ { _BODY_ } _NAME_

// Define member functions
#define FUNC(_TYPE_, _NAME_, _PARAM_) _TYPE_ (*_NAME_) _PARAM_

// Instantiate a class
#define NEW(_CLASS_) ((_CLASS_*)malloc(sizeof(_CLASS_)))

// Release a class object from memory
void release(void *);

#endif /* _DH_CLASS_H_ */

