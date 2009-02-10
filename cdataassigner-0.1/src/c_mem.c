#include "c_mem.h"

C_POOL* pool_Create(C_POOL *tag_pool,size_t data_size)
{
	size_t pool_persize;
			
	if(data_size>=sizeof(char))
	{
		pool_persize=data_size-(data_size%(sizeof(struct block_node)))+sizeof(struct block_node);
		
		tag_pool->head_node=node_Create(data_size);
		tag_pool->free_mem=tag_pool->head_node->pool_mem;
		tag_pool->data_size=pool_persize;
		tag_pool->last_node=tag_pool->head_node;
		
		return tag_pool;
	};
	
	return NULL;
};

struct pool_node* node_Create(size_t data_size)
{
	size_t pool_persize;
	size_t step_length;
	size_t pool_length;
	size_t i;
	struct pool_node *head_node;
	struct block_node *block;
	i=0;
	
	pool_persize=(data_size%(sizeof(struct block_node)))>0?data_size-(data_size%(sizeof(struct block_node)))+sizeof(struct block_node):data_size;
	
	pool_length=pool_persize*__MEM_POOL_SIZE;
	step_length=pool_persize/sizeof(struct block_node);
	
	head_node=(struct pool_node*)malloc(sizeof(struct pool_node));
	head_node->pool_mem=(void*)malloc(pool_length);
	head_node->pool_next=NULL;
	
	block=(struct block_node*)head_node->pool_mem;
	
	for(block=(struct block_node*)head_node->pool_mem;i<__MEM_POOL_SIZE-1;++i)
	{
		block->next_block=((struct block_node*)block)+step_length;
		block=block->next_block;
	};
	
	block->next_block=NULL;
	
	return head_node;
};

C_POOL* pool_Create_Ex(C_POOL *tag_pool,size_t data_size,void *free_mem,size_t mem_size)
{
	size_t pool_persize;
	struct block_node *block;
	size_t step_length;
	size_t i=0;
			
	if(data_size>=sizeof(char))
	{
		pool_persize=(data_size%(sizeof(struct block_node)))>0?data_size-(data_size%(sizeof(struct block_node)))+sizeof(struct block_node):data_size;
		
		step_length=pool_persize/sizeof(struct block_node);
				
		tag_pool->head_node=(C_POOL_NODE*)malloc(sizeof(C_POOL_NODE));
		tag_pool->head_node->pool_mem=free_mem;
		tag_pool->head_node->pool_next=NULL;
		
		block=(struct block_node*)tag_pool->head_node->pool_mem;
		
		for(block=(struct block_node*)tag_pool->head_node->pool_mem;i<mem_size/pool_persize-1;++i)
		{
			block->next_block=((struct block_node*)block)+step_length;
			block=block->next_block;
		};
		block->next_block=NULL;
				
		tag_pool->free_mem=tag_pool->head_node->pool_mem;
		tag_pool->data_size=pool_persize;
		tag_pool->last_node=NULL;
		
		return tag_pool;
	};
	
	return NULL;
};

void* pool_Malloc(struct c_pool *tag_pool)
{
	void *return_mem;
	struct pool_node *new_node;
			
	return_mem=(void*)tag_pool->free_mem;
	
	if(tag_pool->free_mem->next_block!=NULL)
	{
		tag_pool->free_mem=tag_pool->free_mem->next_block;
	}
	else if(tag_pool->last_node!=NULL)
	{
		new_node=node_Create(tag_pool->data_size);
		tag_pool->last_node->pool_next=new_node;
		tag_pool->last_node=new_node;
		tag_pool->free_mem=new_node->pool_mem;
	}
	else
	{
		return NULL;
	};
	
	return return_mem;
};

void pool_Free(void *free_mem,struct c_pool *tag_pool)
{
	((struct block_node*)free_mem)->next_block=tag_pool->free_mem;
	tag_pool->free_mem=free_mem;
};

void pool_Drop(struct c_pool *tag_pool)
{
	struct pool_node *op;
	struct pool_node *tmp;
	
	if(tag_pool->last_node!=NULL)
	{
		for(op=tag_pool->head_node;op!=tag_pool->last_node;)
		{
			free(op->pool_mem);
			tmp=op;
			op=op->pool_next;
			free(tmp);
			
		};
		free(op->pool_mem);
		free(op);
	}
	else
	{
		free(tag_pool->head_node);
	};
	
	
	
};
