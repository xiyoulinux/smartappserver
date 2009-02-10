#ifndef C_ARRAY_H
#define C_ARRAY_H

#include <malloc.h>
#include <string.h>
#include "c_define.h"

typedef void* C_ARRAY;

typedef struct array_head
{
	size_t data_size;
	size_t array_length;
	size_t array_space;
	void (*free_fun)(void* tag_data);
}ARRAY_HEAD;

#define array_Head(tag_array) ((ARRAY_HEAD*)(tag_array)-1)

#define array_Create(size_t) (array_Create_Ex(size_t,NULL))

#define array_Length(tag_array) (array_Head(tag_array)->array_length)

/*C_ARRAY array_Create(size_t data_size);*/

C_ARRAY array_Create_Ex(size_t data_size,void (*free_fun)(void *tag_data));

C_ARRAY array_Resize(C_ARRAY tag_array,size_t new_size);

C_ARRAY array_Append(C_ARRAY tag_array,const void *in_data);

C_ARRAY array_Insert(C_ARRAY tag_array,const void *in_data,void *in_cur);

C_ARRAY array_Remove(C_ARRAY tag_array,void *in_cur);

void array_Drop(C_ARRAY tag_array);

#endif
