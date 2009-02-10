#ifndef C_STACK_H
#define C_STACK_H

#include "c_array.h"

typedef C_ARRAY C_STACK;

#define stack_Head array_Head

#define stack_Create(data_size) (array_Create(data_size))

#define stack_Create_Ex(data_size,free_fun) (array_Create_Ex(data_size,free_fun))

#define stack_Append(tag_array,in_data) (array_Append(tag_array,in_data))

#define stack_Get(tag_array) ((tag_array[(array_Head(tag_array))->array_length-1]))

#define stack_Remove(tag_array) (array_Remove(tag_array,&tag_array[array_Head(tag_array)->array_length-1]))

#define stack_Drop(tag_array) (array_Drop(tag_array))

#endif
