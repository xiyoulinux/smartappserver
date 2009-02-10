#include "c_chain.h"

C_CHAIN* chain_Create_Ex(C_CHAIN *tag_chain,size_t data_size,void (*free_fun)(void *tag_data))
{
	if(tag_chain==NULL)return NULL;
	
	tag_chain->chain_head=NULL;
	if(pool_Create(&tag_chain->data_pool,data_size+sizeof(C_CHAIN))==NULL)return NULL;
	
	tag_chain->chain_length=0;
	tag_chain->data_size=data_size;
	tag_chain->free_fun=free_fun;
	
	return tag_chain;
};

void* chain_Append(const void *in_data,C_CHAIN *tag_chain)
{
	CHAIN_HEAD *new_op;
	
	new_op=pool_Malloc(&tag_chain->data_pool);
	new_op->next=NULL;
	++new_op;
	memcpy(new_op,in_data,tag_chain->data_size);
	if(tag_chain->chain_head!=NULL)
	{
		(chain_Head(tag_chain->chain_tail))->next=new_op;
		tag_chain->chain_tail=new_op;
		
	}
	else
	{
		tag_chain->chain_head=new_op;
		tag_chain->chain_tail=new_op;
	};
	++tag_chain->chain_length;
	return (void*)new_op;
};

void* chain_Insert_Ex(const void *in_data,void *in_cur,C_CHAIN *tag_chain)
{
	CHAIN_HEAD *chain_head;
	CHAIN_HEAD *cur_head;
	CHAIN_HEAD *new_op;
	
	chain_head=chain_Head(in_cur);
	if(chain_head->next!=NULL)
	{
		new_op=pool_Malloc(&tag_chain->data_pool);
		cur_head=chain_Head(in_cur);
		new_op->next=cur_head->next;
		++new_op;
		memcpy(new_op,in_data,tag_chain->data_size);
		cur_head->next=(void*)new_op;
	}
	else
	{
		chain_Append(in_data,tag_chain);
	};
	return (void*)new_op;
};

void* chain_Remove_Ex(void *in_cur,C_CHAIN *tag_chain)
{
	CHAIN_HEAD *cur_head;
	void *tmp_ptr;
	
	cur_head=chain_Head(in_cur);
	tmp_ptr=cur_head->next;
	if(tmp_ptr!=NULL)
	{
		if(tag_chain->free_fun!=NULL)tag_chain->free_fun(*(void**)tmp_ptr);
		cur_head->next=(chain_Head(cur_head->next))->next;
		--tag_chain->chain_length;
		pool_Free(((char*)tmp_ptr-sizeof(C_CHAIN)),&tag_chain->data_pool);
		return in_cur;
	};
	
	return NULL;
};

void chain_Drop(C_CHAIN *tag_chain)
{
	void *tmp_ptr;
	
	if(tag_chain->free_fun!=NULL&&tag_chain->chain_length>0)for(tmp_ptr=chain_First(tag_chain);tmp_ptr!=NULL;tmp_ptr=chain_Next(tmp_ptr))tag_chain->free_fun(*(void**)tmp_ptr);
	pool_Drop(&tag_chain->data_pool);
};
