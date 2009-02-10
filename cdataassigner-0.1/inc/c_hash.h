#ifndef C_HASH_H
#define C_HASH_H

#include "c_define.h"
#include "c_mem.h"
#include <string.h>

typedef void* HASH_DATA;

typedef struct hash_node
{
	HASH_DATA next;
}HASH_HEAD;

typedef struct hash_type
{
	UINT_ (*hash_fun)(void *in_data);
	BOOL_ (*ensure_fun)(void *lhs,void *rhs);
	size_t data_size;
	UINT_ max_size;
	HASH_HEAD *peer_pool;
	C_POOL data_pool;
	
}C_HASH;

#define hash_next(tag_peer) (((HASH_HEAD*)tag_peer-1)->next)

C_HASH* hash_Create(C_HASH *tag_hash,UINT_ (*hash_fun)(void *in_data),BOOL_ (*ensure_fun)(void *lhs,void *rhs),size_t data_size,UINT_ max_size);

HASH_DATA hash_Append(HASH_DATA in_data,C_HASH *tag_hash);

HASH_DATA hash_Get(HASH_DATA in_data,C_HASH *tag_hash);

void hash_Remove(HASH_DATA in_data,C_HASH *tag_hash);

void hash_Drop(C_HASH *tag_hash);

#endif
