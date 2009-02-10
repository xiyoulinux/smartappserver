#include "c_hash.h"

C_HASH* hash_Create(C_HASH *tag_hash,UINT_ (*hash_fun)(void *in_data),BOOL_ (*ensure_fun)(void *lhs,void *rhs),size_t data_size,UINT_ max_size)
{
	UINT_ i;
	
	tag_hash->hash_fun=hash_fun;
	tag_hash->ensure_fun=ensure_fun;
	tag_hash->data_size=data_size;
	tag_hash->peer_pool=NULL;
	tag_hash->max_size=max_size;
	tag_hash->peer_pool=malloc(max_size*sizeof(HASH_HEAD));
	if(tag_hash->peer_pool==NULL)return NULL;
	for(i=0;i<max_size;++i)tag_hash->peer_pool[i].next=NULL;
	
	if(!pool_Create(&tag_hash->data_pool,sizeof(HASH_HEAD)+data_size))
	{
		free(tag_hash->peer_pool);
		return NULL;
	};
	return tag_hash;
};

HASH_DATA hash_Append(HASH_DATA in_data,C_HASH *tag_hash)
{
	UINT_ data_code;
	HASH_HEAD *op;
	HASH_HEAD *op_op;
		
	data_code=tag_hash->hash_fun(in_data);
	op=(HASH_HEAD*)pool_Malloc(&tag_hash->data_pool);
	op->next=NULL;
	++op;
	memcpy(op,in_data,tag_hash->data_size);
	op_op=&(tag_hash->peer_pool[data_code]);
	if(op_op->next==NULL)
	{
		op_op->next=op;
	}
	else
	{
		for(op_op=op_op->next;hash_next(op_op)!=NULL;op_op=hash_next(op_op))hash_next(op_op)=op;
	};
	return op;	
};

HASH_DATA hash_Get(HASH_DATA in_data,C_HASH *tag_hash)
{
	UINT_ data_code;
	HASH_HEAD *op;
	
	data_code=tag_hash->hash_fun(in_data);
	op=&(tag_hash->peer_pool[data_code]);
	
	if(op->next!=NULL)
	{
		
		for(op=op->next;;op=(HASH_HEAD*)hash_next(op))
		{
			
			if(tag_hash->ensure_fun(in_data,op))return (HASH_DATA)op;
			if(hash_next(op)==NULL)break;
		};
	};
	
	return NULL;
};

void hash_Remove(HASH_DATA in_data,C_HASH *tag_hash)
{
	UINT_ data_code;
	HASH_HEAD *op;
	HASH_DATA tmp_op;
	
	data_code=tag_hash->hash_fun(in_data);
	op=&tag_hash->peer_pool[data_code];
	if(op->next!=NULL)
	{
		if(hash_next(op->next)==NULL)
		{
			pool_Free(op->next,&tag_hash->data_pool);
			op->next=NULL;
		}
		else
		{
			
			for(op=(HASH_HEAD*)op->next;op!=NULL;op=(HASH_HEAD*)hash_next(op))
			{
				if(tag_hash->ensure_fun(in_data,hash_next(op)))
				{
					tmp_op=hash_next(op);
					hash_next(op)=hash_next(tmp_op);
					pool_Free(tmp_op,&tag_hash->data_pool);
					
					break;
					
				};
				
			};
		};
	};
	
};

void hash_Drop(C_HASH *tag_hash)
{
	free(tag_hash->peer_pool);
	pool_Drop(&tag_hash->data_pool);
};
