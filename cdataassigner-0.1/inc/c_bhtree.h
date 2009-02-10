#ifndef C_BHTREE_H
#define C_BHTREE_H

#include <string.h>

#include "c_mem.h"
#include "c_stack.h"
#include "c_define.h"

typedef void* BHTREE_NODE;

typedef enum bhtree_color
{
	BLACK=0,
	RED=1
}BHTREE_COLOR;

typedef struct bhtree_head
{
	BHTREE_COLOR color;
	BHTREE_NODE *pa;
	BHTREE_NODE *left;
	BHTREE_NODE *right;
}BHTREE_HEAD;

typedef struct bhtree_type
{
	size_t data_size;
	C_POOL data_pool;
	BHTREE_NODE tree_root;
	BOOL_ (*compare_fun)(BHTREE_NODE *lhs,BHTREE_NODE *rhs);
	void (*free_fun)(void* tag_data);
}C_BHTREE;

/*basic tree op*/

#define bhtree_Head(node) ((BHTREE_HEAD*)node-1)
#define node_left(node) (bhtree_Head(node)->left)
#define node_right(node) (bhtree_Head(node)->right)
#define node_color(node) (bhtree_Head(node)->color)
#define node_pa(node) (bhtree_Head(node)->pa)

#define bhtree_Create(tag_tree,data_size,compare_fun) (bhtree_Create_Ex(tag_tree,data_size,compare_fun,NULL))

C_BHTREE* bhtree_Create_Ex(C_BHTREE *tag_tree,size_t data_size,BOOL_ (*compare_fun)(BHTREE_NODE *lhs,BHTREE_NODE *rhs),void (*free_fun)(void *tag_data));

BHTREE_NODE bhtree_Leftrotate(BHTREE_NODE in_cur,C_BHTREE *tag_tree);

BHTREE_NODE bhtree_Rightrotate(BHTREE_NODE in_cur,C_BHTREE *tag_tree);

BHTREE_NODE bhtree_To(void *in_data,C_BHTREE *tag_tree);

BHTREE_NODE bhtree_Get(void *in_data,BOOL_ (*ensure_fun)(BHTREE_NODE lhs,BHTREE_NODE rhs),C_BHTREE *tag_tree);

BHTREE_NODE bhtree_Append(void *in_data,C_BHTREE *tag_tree);

BHTREE_NODE bhtree_Remove(BHTREE_NODE in_cur,C_BHTREE *tag_tree);

/*#define bhtree_Drop(tag_tree) (pool_Drop(&((C_BHTREE*)tag_tree)->data_pool));*/
void bhtree_Drop(C_BHTREE *tag_tree);

#endif
