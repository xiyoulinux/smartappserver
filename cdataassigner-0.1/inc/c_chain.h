#ifndef C_CHAIN_H
#define C_CHAIN_H

#include <string.h>
#include "c_mem.h"
#include "c_define.h"

typedef struct chain_head
{
	void *next;
}CHAIN_HEAD;

typedef struct chain_type
{
	size_t data_size;
	void *chain_head;
	void *chain_tail;
	C_POOL data_pool;
	size_t chain_length;
	void (*free_fun)(void* tag_data);
}C_CHAIN;

#define chain_Head(in_cur) ((CHAIN_HEAD*)in_cur-1)

#define chain_Create(tag_chain,data_size) (chain_Create_Ex(tag_chain,data_size,NULL))

C_CHAIN* chain_Create_Ex(C_CHAIN *tag_chain,size_t data_size,void (*free_fun)(void *tag_data));

/*void* chain_First(C_CHAIN *tag_chain);*/
#define chain_First(tag_chain) (((C_CHAIN*)tag_chain)->chain_head==NULL?NULL:((C_CHAIN*)tag_chain)->chain_head)

void* chain_Append(const void *in_data,C_CHAIN *tag_chain);

void* chain_Insert_Ex(const void *in_data,void *in_cur,C_CHAIN *tag_chain);

/*void* chain_Next(void *in_cur);*/
#define chain_Next(in_cur) (chain_Head(in_cur)->next)

void* chain_Remove_Ex(void *in_cur,C_CHAIN *tag_chain);

void chain_Drop(C_CHAIN *tag_chain);

#endif
