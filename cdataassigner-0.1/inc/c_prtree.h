#ifndef C_PRTREE_H
#define C_PRTREE_H

#include "c_define.h"
#include "c_mem.h"
#include "c_stack.h"
#include <string.h>

typedef void* PRTREE_NODE;

typedef struct prtree_head
{
	PRTREE_NODE *pa;
	PRTREE_NODE *left;
	PRTREE_NODE *right;
}PRTREE_HEAD;

typedef struct prtree_type
{
	size_t data_size;
	C_POOL data_pool;
	PRTREE_NODE tree_root;
	BOOL_ (*compare_fun)(PRTREE_NODE *lhs,PRTREE_NODE *rhs);
	void (*free_fun)(void* tag_data);
	
}C_PRTREE;

/*basic tree op*/

#define prtree_Head(node) ((PRTREE_HEAD*)node-1)
#define prnode_left(node) (prtree_Head(node)->left)
#define prnode_right(node) (prtree_Head(node)->right)
#define prnode_pa(node) (prtree_Head(node)->pa)

/*C_PRTREE* prtree_Create(C_PRTREE *tag_tree,size_t data_size,BOOL_ (*compare_fun)(PRTREE_NODE *lhs,PRTREE_NODE *rhs));*/
#define prtree_Create(tag_tree,data_size,compare_fun) prtree_Create_Ex(tag_tree,data_size,compare_fun,NULL);

C_PRTREE* prtree_Create_Ex(C_PRTREE *tag_tree,size_t data_size,BOOL_ (*compare_fun)(PRTREE_NODE *lhs,PRTREE_NODE *rhs),void (*free_fun)(void *tag_data));

PRTREE_NODE prtree_Leftrotate(PRTREE_NODE in_cur,C_PRTREE *tag_tree);

PRTREE_NODE prtree_Rightrotate(PRTREE_NODE in_cur,C_PRTREE *tag_tree);

PRTREE_NODE prtree_To(void *in_data,C_PRTREE *tag_tree);

PRTREE_NODE prtree_Get(void *in_data,BOOL_ (*ensure_fun)(PRTREE_NODE lhs,PRTREE_NODE rhs),C_PRTREE *tag_tree);

PRTREE_NODE prtree_Append(void *in_data,C_PRTREE *tag_tree);

PRTREE_NODE prtree_Remove(PRTREE_NODE in_cur,C_PRTREE *tag_tree);

void prtree_Drop(C_PRTREE *tag_tree);

#endif
