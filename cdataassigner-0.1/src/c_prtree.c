#include "c_prtree.h"

C_PRTREE* prtree_Create_Ex(C_PRTREE *tag_tree,size_t data_size,BOOL_ (*compare_fun)(PRTREE_NODE *lhs,PRTREE_NODE *rhs),void (*free_fun)(void *tag_data))
{
	if(pool_Create(&tag_tree->data_pool,sizeof(PRTREE_HEAD)+data_size)==NULL)return NULL;
	tag_tree->data_size=data_size;
	tag_tree->tree_root=NULL;
	tag_tree->compare_fun=compare_fun;
	tag_tree->free_fun=free_fun;
	return tag_tree;
};

PRTREE_NODE prtree_Leftrotate(PRTREE_NODE in_cur,C_PRTREE *tag_tree)
{
	PRTREE_NODE r_node;
	
	if(in_cur!=NULL&&prnode_right(in_cur)!=NULL)
	{
		r_node=prnode_right(in_cur);
		prnode_right(in_cur)=prnode_left(r_node);
		if(prnode_left(r_node)!=NULL)prnode_pa(prnode_left(r_node))=in_cur;
		prnode_pa(r_node)=prnode_pa(in_cur);
		if(prnode_pa(in_cur)==NULL)
		{
			tag_tree->tree_root=r_node;
		}
		else if(in_cur==prnode_left(prnode_pa(in_cur)))
		{
			prnode_left(prnode_pa(in_cur))=r_node;
		}
		else
		{
			prnode_right(prnode_pa(in_cur))=r_node;
		};
		prnode_left(r_node)=in_cur;
		prnode_pa(in_cur)=r_node;
		
	};
	
	return in_cur;
	
};

PRTREE_NODE prtree_Rightrotate(PRTREE_NODE in_cur,C_PRTREE *tag_tree)
{
	PRTREE_NODE l_node;
	
	if(in_cur!=NULL&&prnode_left(in_cur)!=NULL)
	{
		l_node=prnode_left(in_cur);
		prnode_left(in_cur)=prnode_right(l_node);
		if(prnode_right(l_node)!=NULL)prnode_pa(prnode_right(l_node))=in_cur;
		prnode_pa(l_node)=prnode_pa(in_cur);
		if(prnode_pa(in_cur)==NULL)
		{
			tag_tree->tree_root=l_node;
		}
		else if(in_cur==prnode_right(prnode_pa(in_cur)))
		{
			prnode_right(prnode_pa(in_cur))=l_node;
		}
		else
		{
			prnode_left(prnode_pa(in_cur))=l_node;
		};
		prnode_right(l_node)=in_cur;
		prnode_pa(in_cur)=l_node;
	};
	
	return in_cur;
	
};

PRTREE_NODE prtree_To(void *in_data,C_PRTREE *tag_tree)
{
	PRTREE_NODE op;
	PRTREE_NODE op_pa;
	
	op=tag_tree->tree_root;
	if(op==NULL)return NULL;
	for(;op!=NULL;)
	{
		op_pa=op;
		op=tag_tree->compare_fun(in_data,op)?prtree_Head(op)->right:prtree_Head(op)->left;
	};
	return op_pa;
};

PRTREE_NODE prtree_Get(void *in_data,BOOL_ (*ensure_fun)(PRTREE_NODE lhs,PRTREE_NODE rhs),C_PRTREE *tag_tree)
{
	PRTREE_NODE op;
		
	op=tag_tree->tree_root;
	if(op==NULL)return NULL;
	for(;op!=NULL;)
	{
		if(ensure_fun(op,in_data))
		{
			for(;prnode_pa(op)!=NULL;)
			{
				if(op==prnode_left(prnode_pa(op)))
				{
					prtree_Rightrotate(prnode_pa(op),tag_tree);
				}
				else
				{
					prtree_Leftrotate(prnode_pa(op),tag_tree);
				};
			
			};
		};
		tag_tree->tree_root=op;
		return op;
		op=tag_tree->compare_fun(in_data,op)?prtree_Head(op)->right:prtree_Head(op)->left;
	};
	return NULL;
};

PRTREE_NODE prtree_Append(void *in_data,C_PRTREE *tag_tree)
{
	PRTREE_NODE tree_op;
	PRTREE_HEAD *new_data;
	
	tree_op=prtree_To(in_data,tag_tree);

	if(tree_op!=NULL)/*已有节点*/
	{

		if(tag_tree->compare_fun(in_data,tree_op))
		{
			
			new_data=pool_Malloc(&tag_tree->data_pool);
			if(new_data==NULL)return NULL;
			
			new_data->left=NULL;
			new_data->right=NULL;
			new_data->pa=tree_op;
			
			++new_data;
			prnode_right(tree_op)=(PRTREE_NODE)new_data;
			memcpy(new_data,in_data,tag_tree->data_size);
			/*prtree_Fix(new_data,tag_tree);*/
		}
		else
		{
			
			new_data=pool_Malloc(&tag_tree->data_pool);
			if(new_data==NULL)return NULL;
			
			new_data->left=NULL;
			new_data->right=NULL;
			
			new_data->pa=tree_op;
			
			++new_data;
			prnode_left(tree_op)=(PRTREE_NODE)new_data;
			memcpy(new_data,in_data,tag_tree->data_size);
			/*prtree_Fix(new_data,tag_tree);*/
		};
	}			
	else/*首次插入节点*/
	{
		
		new_data=pool_Malloc(&tag_tree->data_pool);
		if(new_data==NULL)return NULL;
				
		new_data->left=NULL;
		new_data->right=NULL;
		
		new_data->pa=tree_op;
		
		++new_data;
		tag_tree->tree_root=new_data;
		memcpy(new_data,in_data,tag_tree->data_size);
	};
	
	return (PRTREE_NODE)new_data;
};

PRTREE_NODE prtree_Remove(PRTREE_NODE in_cur,C_PRTREE *tag_tree)
{
	PRTREE_NODE op;
	PRTREE_NODE op_op;
	
	if(prnode_left(in_cur)==NULL||prnode_right(in_cur)==NULL)
	{
		op=in_cur;
		
	}
	else/*有两个孩子时找出其后继*/
	{
		if(prnode_right(in_cur)!=NULL)
		{
			op=in_cur;
			for(;prnode_left(op)!=NULL;)op=prnode_left(op);
		}
		else
		{
			op_op=in_cur;
			op=prnode_pa(op_op);
			for(;op!=NULL&&op_op==prnode_right(op);)
			{
				op_op=op;
				op=prnode_pa(op);
			};
		};
	};
	
	op_op=(prnode_left(op)!=NULL)?prnode_left(op):prnode_right(op);
		
	if(op_op!=NULL)prnode_pa(op_op)=prnode_pa(op);
	if(prnode_pa(op)==NULL)
	{
		tag_tree->tree_root=op_op;
	}
	else if(op==prnode_left(prnode_pa(op)))
	{
		prnode_left(prnode_pa(op))=op_op;
	}
	else
	{
		prnode_right(prnode_pa(op))=op_op;
	};
	
	if(tag_tree->free_fun!=NULL)tag_tree->free_fun(*(void**)in_cur);
	pool_Free(prtree_Head(in_cur),&tag_tree->data_pool);
	
	return NULL;
};

void prtree_Drop(C_PRTREE *tag_tree)
{
	PRTREE_NODE *stack_op;
	PRTREE_NODE op;
	
	if(tag_tree->tree_root!=NULL&&tag_tree->free_fun!=NULL)
	{
		stack_op=stack_Create(sizeof(PRTREE_NODE));
		stack_op=stack_Append(stack_op,&tag_tree->tree_root);
		while(stack_Head(stack_op)->array_length>0)
		{
			op=(PRTREE_NODE)stack_Get(stack_op);
			stack_op=stack_Remove(stack_op);
			if(prnode_left(op)!=NULL)stack_op=stack_Append(stack_op,&prnode_left(op));
			if(prnode_right(op)!=NULL)stack_op=stack_Append(stack_op,&prnode_right(op));
			tag_tree->free_fun(*(void**)op);
		};
		stack_Drop(stack_op);
	};
	
	pool_Drop(&tag_tree->data_pool);
};
