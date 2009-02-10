#include "c_array.h"

C_ARRAY array_Create_Ex(size_t data_size,void (*free_fun)(void *tag_data))
{
	ARRAY_HEAD *new_array;
	
	new_array=(ARRAY_HEAD*)malloc(sizeof(ARRAY_HEAD));
		if(new_array==NULL)return NULL;
	
	new_array->data_size=data_size;
	new_array->array_length=0;
	new_array->array_space=0;
	new_array->free_fun=free_fun;
	
	return (C_ARRAY)(++new_array);
};

C_ARRAY array_Resize(C_ARRAY tag_array,size_t new_size)
{
	ARRAY_HEAD *array_head;
	void **array_op;
	
	array_head=array_Head(tag_array);
	
	if(array_head->array_length<=new_size)
	{
		array_head=(ARRAY_HEAD*)realloc(array_head,(new_size)*(array_head->data_size)+sizeof(ARRAY_HEAD));
			if(array_head==NULL)return NULL;
	}
	else
	{
		if(array_head->free_fun!=NULL)for(array_op=&((void**)tag_array)[new_size];*(array_op-1)!=((void**)tag_array)[array_head->array_length-1];++array_op)array_head->free_fun(*array_op);
		array_head=(ARRAY_HEAD*)realloc(array_head,(new_size)*(array_head->data_size)+sizeof(ARRAY_HEAD));
	};
	array_head->array_space=new_size;
	array_head->array_length=array_head->array_length>array_head->array_space?array_head->array_space:array_head->array_length;
	
	return (C_ARRAY)++array_head;
};

C_ARRAY array_Append(C_ARRAY tag_array,const void *in_data)
{
	ARRAY_HEAD *array_head;
	char *array_op;
	
	array_head=array_Head(tag_array);
	array_op=(char*)tag_array;
	if((array_head->array_length)>=(array_head->array_space))
	{
		array_op=(char*)array_Resize((C_ARRAY)array_op,array_head->array_length+1);
		if(array_op==NULL)return NULL;
	};
	
	array_head=array_Head(array_op);

	memcpy(array_op+array_head->array_length*array_head->data_size,in_data,array_head->data_size);
	++(array_head->array_length);
	
	return (C_ARRAY)array_op;
};

C_ARRAY array_Insert(C_ARRAY tag_array,const void *in_data,void *in_cur)
{
	ARRAY_HEAD *array_head;
	void *array_op;
	void *tmp_space;
	
	array_head=array_Head(tag_array);
	array_op=(void*)tag_array;
	
	tmp_space=(void*)malloc(array_head->data_size);
		if(tmp_space==NULL)return NULL;
	memcpy(tmp_space,&((char*)array_op)[(array_head->array_length-1)*(array_head->data_size)],array_head->data_size);
	
	memmove(((void*)in_cur)+array_head->data_size,(void*)in_cur,(array_op+(array_head->array_length-1)*(array_head->data_size))-((void*)in_cur));
	memcpy(in_cur,in_data,array_head->data_size);
	array_op=(C_ARRAY)array_Append(tmp_space,(C_ARRAY)array_op);
	free(tmp_space);
		
	return (C_ARRAY)array_op;
	
};

C_ARRAY array_Remove(C_ARRAY tag_array,void *in_cur)
{
	ARRAY_HEAD *array_head;
	void *array_op;
	
	array_head=array_Head(tag_array);
	array_op=(char*)tag_array;
	
	if(array_head->free_fun!=NULL)array_head->free_fun(*(void**)in_cur);
	memmove(in_cur,(void*)in_cur+array_head->data_size,(array_op+(array_head->array_length-1)*(array_head->data_size))-((void*)in_cur));
	--array_head->array_length;
	
	return array_op;
};

void array_Drop(C_ARRAY tag_array)
{
	ARRAY_HEAD *array_head;
	void **array_op;
	
	array_head=array_Head(tag_array);
	if(array_head->free_fun!=NULL&&array_head->array_length>0)for(array_op=(void**)tag_array;*(array_op-1)!=((void**)tag_array)[array_head->array_length-1];++array_op)array_head->free_fun(*array_op);
	
	free(array_head);
};
