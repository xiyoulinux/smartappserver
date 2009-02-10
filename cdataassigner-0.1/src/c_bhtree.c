#include "c_bhtree.h"

/*
C_BHTREE* bhtree_Create(C_BHTREE *tag_tree,size_t data_size,BOOL_ (*compare_fun)(BHTREE_NODE *lhs,BHTREE_NODE *rhs))
{
	tag_tree->data_size=data_size;
	if(pool_Create(&tag_tree->data_pool,sizeof(BHTREE_HEAD)+data_size)==NULL)return NULL;
	tag_tree->tree_root=NULL;
	tag_tree->compare_fun=compare_fun;
	return tag_tree;
};
*/

C_BHTREE* bhtree_Create_Ex(C_BHTREE *tag_tree,size_t data_size,BOOL_ (*compare_fun)(BHTREE_NODE *lhs,BHTREE_NODE *rhs),void (*free_fun)(void *tag_data))
{
	if(pool_Create(&tag_tree->data_pool,sizeof(BHTREE_HEAD)+data_size)==NULL)return NULL;
	tag_tree->data_size=data_size;	
	tag_tree->tree_root=NULL;
	tag_tree->compare_fun=compare_fun;
	tag_tree->free_fun=free_fun;
	return tag_tree;
};

BHTREE_NODE bhtree_Leftrotate(BHTREE_NODE in_cur,C_BHTREE *tag_tree)
{
	BHTREE_NODE r_node;
	
	if(in_cur!=NULL&&node_right(in_cur)!=NULL)
	{
		r_node=node_right(in_cur);
		node_right(in_cur)=node_left(r_node);
		if(node_left(r_node)!=NULL)node_pa(node_left(r_node))=in_cur;
		node_pa(r_node)=node_pa(in_cur);
		if(node_pa(in_cur)==NULL)
		{
			tag_tree->tree_root=r_node;
		}
		else if(in_cur==node_left(node_pa(in_cur)))
		{
			node_left(node_pa(in_cur))=r_node;
		}
		else
		{
			node_right(node_pa(in_cur))=r_node;
		};
		node_left(r_node)=in_cur;
		node_pa(in_cur)=r_node;
		
	};
	
	return in_cur;
	
};

BHTREE_NODE bhtree_Rightrotate(BHTREE_NODE in_cur,C_BHTREE *tag_tree)
{
	BHTREE_NODE l_node;
	
	if(in_cur!=NULL&&node_left(in_cur)!=NULL)
	{
		l_node=node_left(in_cur);
		node_left(in_cur)=node_right(l_node);
		if(node_right(l_node)!=NULL)node_pa(node_right(l_node))=in_cur;
		node_pa(l_node)=node_pa(in_cur);
		if(node_pa(in_cur)==NULL)
		{
			tag_tree->tree_root=l_node;
		}
		else if(in_cur==node_right(node_pa(in_cur)))
		{
			node_right(node_pa(in_cur))=l_node;
		}
		else
		{
			node_left(node_pa(in_cur))=l_node;
		};
		node_right(l_node)=in_cur;
		node_pa(in_cur)=l_node;
	};
	
	return in_cur;
	
};

BHTREE_NODE bhtree_To(void *in_data,C_BHTREE *tag_tree)
{
	BHTREE_NODE op;
	BHTREE_NODE op_pa;
	
	op=tag_tree->tree_root;
	if(op==NULL)return NULL;
	for(;op!=NULL;)
	{
		op_pa=op;
		op=tag_tree->compare_fun(in_data,op)?bhtree_Head(op)->right:bhtree_Head(op)->left;
	};
	return op_pa;
};

BHTREE_NODE bhtree_Get(void *in_data,BOOL_ (*ensure_fun)(BHTREE_NODE lhs,BHTREE_NODE rhs),C_BHTREE *tag_tree)
{
	BHTREE_NODE op;
		
	op=tag_tree->tree_root;
	if(op==NULL)return NULL;
	for(;op!=NULL;)
	{
		if(ensure_fun(op,in_data))return op;
		op=tag_tree->compare_fun(in_data,op)?bhtree_Head(op)->right:bhtree_Head(op)->left;
	};
	return NULL;
};

BHTREE_NODE bhtree_Append(void *in_data,C_BHTREE *tag_tree)
{
	BHTREE_NODE tree_op;
	BHTREE_HEAD *new_data;
	
	tree_op=bhtree_To(in_data,tag_tree);

	if(tree_op!=NULL)/*已有节点*/
	{

		if(tag_tree->compare_fun(in_data,tree_op))
		{
			
			new_data=pool_Malloc(&tag_tree->data_pool);
			
			new_data->color=RED;
			
			new_data->left=NULL;
			new_data->right=NULL;
			new_data->pa=tree_op;
			
			++new_data;
			node_right(tree_op)=(BHTREE_NODE)new_data;
			memcpy(new_data,in_data,tag_tree->data_size);
			/*bhtree_Fix(new_data,tag_tree);*/
		}
		else
		{
			
			new_data=pool_Malloc(&tag_tree->data_pool);
			
			new_data->color=RED;
			
			new_data->left=NULL;
			new_data->right=NULL;
			
			new_data->pa=tree_op;
			
			++new_data;
			node_left(tree_op)=(BHTREE_NODE)new_data;
			memcpy(new_data,in_data,tag_tree->data_size);
			/*bhtree_Fix(new_data,tag_tree);*/
		};
		
		
		
		BHTREE_NODE u_node;
		
		for(;new_data!=NULL&&node_pa(new_data)!=NULL&&node_color(node_pa(new_data))==RED;)
		{
		
			if(node_left(node_pa(node_pa(new_data)))!=NULL&&node_pa(new_data)==node_left(node_pa(node_pa(new_data))))
			{
			
				u_node=node_right(node_pa(node_pa(new_data)));
		
				if(u_node!=NULL&&node_color(u_node)==RED)
				{
					node_color(node_pa(new_data))=BLACK;
					if(u_node!=NULL)node_color(u_node)=BLACK;
					node_color(node_pa(node_pa(new_data)))=RED;
					new_data=(BHTREE_HEAD*)node_pa(node_pa(new_data));
				}
				else if(node_right(node_pa(new_data))!=NULL&&(BHTREE_NODE)new_data==node_right(node_pa(new_data)))
				{
					new_data=(BHTREE_HEAD*)node_pa(new_data);
					bhtree_Leftrotate(new_data,tag_tree);
				
				};
			
				if(node_pa(new_data)!=NULL)node_color(node_pa(new_data))=BLACK;
				if(node_pa(new_data)!=NULL&&node_pa(node_pa(new_data))!=NULL)node_color(node_pa(node_pa(new_data)))=RED;
				if(node_pa(new_data)!=NULL&&node_pa(node_pa(new_data))!=NULL)bhtree_Rightrotate(node_pa(node_pa(new_data)),tag_tree);
			
			}
			else
			{
		
			u_node=node_left(node_pa(node_pa(new_data)));
	
			if(u_node!=NULL&&node_color(u_node)==RED)
			{
				node_color(node_pa(new_data))=BLACK;
				if(u_node!=NULL)node_color(u_node)=BLACK;
				node_color(node_pa(node_pa(new_data)))=RED;
				new_data=(BHTREE_HEAD*)node_pa(node_pa(new_data));
			}
			else if(node_left(node_pa(new_data))!=NULL&&(BHTREE_NODE)new_data==node_left(node_pa(new_data)))
			{
				new_data=(BHTREE_HEAD*)node_pa(new_data);
				bhtree_Rightrotate(new_data,tag_tree);
			
			};
		
			if(node_pa(new_data)!=NULL)node_color(node_pa(new_data))=BLACK;
			if(node_pa(new_data)!=NULL&&node_pa(node_pa(new_data))!=NULL)node_color(node_pa(node_pa(new_data)))=RED;
			if(node_pa(new_data)!=NULL&&node_pa(node_pa(new_data))!=NULL)bhtree_Leftrotate(node_pa(node_pa(new_data)),tag_tree);
			};
		};
	
		node_color(tag_tree->tree_root)=BLACK;
					
	}
	else/*首次插入节点*/
	{
		
		new_data=pool_Malloc(&tag_tree->data_pool);
				
		new_data->color=BLACK;
			
		new_data->left=NULL;
		new_data->right=NULL;
		
		new_data->pa=tree_op;
		
		++new_data;
		tag_tree->tree_root=new_data;
		memcpy(new_data,in_data,tag_tree->data_size);
	};
	
	return (BHTREE_NODE)new_data;
};

BHTREE_NODE bhtree_Remove(BHTREE_NODE in_cur,C_BHTREE *tag_tree)
{
	BHTREE_NODE op;
	BHTREE_NODE op_op;
	
	if(node_left(in_cur)==NULL||node_right(in_cur)==NULL)
	{
		op=in_cur;
		
	}
	else/*有两个孩子时找出其后继*/
	{
		if(node_right(in_cur)!=NULL)
		{
			for(op=in_cur;node_left(op)!=NULL;op=node_left(op));
		}
		else
		{
			op_op=in_cur;
			op=node_pa(op_op);
			for(;op!=NULL&&op_op==node_right(op);)
			{
				op_op=op;
				op=node_pa(op);
			};
		};
	};
	
	op_op=(node_left(op)!=NULL)?node_left(op):node_right(op);
		
	if(op_op!=NULL)node_pa(op_op)=node_pa(op);
	if(node_pa(op)==NULL)
	{
		tag_tree->tree_root=op_op;
	}
	else if(op==node_left(node_pa(op)))
	{
		node_left(node_pa(op))=op_op;
	}
	else
	{
		node_right(node_pa(op))=op_op;
	};
	
	if(tag_tree->free_fun!=NULL)tag_tree->free_fun(*(void**)in_cur);
	pool_Free(bhtree_Head(in_cur),&tag_tree->data_pool);

	/*start fix*/
	if(node_color(op)==BLACK)
	{
		op=op_op;
		for(;op!=NULL&&node_color(op)==BLACK;)
		{
			if(op==node_left(node_pa(op)))
			{
				op_op=node_right(node_pa(op));
				if(node_color(op_op)==RED)
				{
					node_color(op_op)=BLACK;
					node_color(node_pa(op))=RED;
					bhtree_Leftrotate(node_pa(op),tag_tree);
					op_op=node_right(node_pa(op));
				};
				if(node_color(node_left(op_op))==BLACK&&node_color(node_right(op_op))==BLACK)
				{
					node_color(op_op)=RED;
					op=node_pa(op);
				}
				else if(node_color(node_right(op_op))==BLACK)
				{
					node_color(node_left(op_op))=BLACK;
					node_color(op_op)=RED;
					bhtree_Rightrotate(op_op,tag_tree);
					op_op=node_right(node_pa(op));
				};
				node_color(op_op)=node_color(node_pa(op));
				node_color(node_pa(op))=BLACK;
				node_color(node_right(op_op))=BLACK;
				bhtree_Leftrotate(node_pa(op),tag_tree);
				op=tag_tree->tree_root;
				node_color(op)=BLACK;
			}
			else
			{
				op_op=node_left(node_pa(op));
				if(node_color(op_op)==RED)
				{
					node_color(op_op)=BLACK;
					node_color(node_pa(op))=RED;
					bhtree_Rightrotate(node_pa(op),tag_tree);
					op_op=node_left(node_pa(op));
				};
				if(node_color(node_right(op_op))==BLACK&&node_color(node_left(op_op))==BLACK)
				{
					node_color(op_op)=RED;
					op=node_pa(op);
				}
				else if(node_color(node_left(op_op))==BLACK)
				{
					node_color(node_right(op_op))=BLACK;
					node_color(op_op)=RED;
					bhtree_Leftrotate(op_op,tag_tree);
					op_op=node_left(node_pa(op));
				};
				node_color(op_op)=node_color(node_pa(op));
				node_color(node_pa(op))=BLACK;
				node_color(node_left(op_op))=BLACK;
				bhtree_Rightrotate(node_pa(op),tag_tree);
				op=tag_tree->tree_root;
				node_color(op)=BLACK;
			};
		
		};
		
		
	};
	return NULL;
};

void bhtree_Drop(C_BHTREE *tag_tree)
{
	BHTREE_NODE *stack_op;
	BHTREE_NODE op;
	
	if(tag_tree->tree_root!=NULL&&tag_tree->free_fun!=NULL)
	{
		stack_op=stack_Create(sizeof(BHTREE_NODE));
		stack_op=stack_Append(stack_op,&tag_tree->tree_root);
		while(stack_Head(stack_op)->array_length>0)
		{
			op=(BHTREE_NODE)stack_Get(stack_op);
			stack_op=stack_Remove(stack_op);
			if(node_left(op)!=NULL)stack_op=stack_Append(stack_op,&node_left(op));
			if(node_right(op)!=NULL)stack_op=stack_Append(stack_op,&node_right(op));
			tag_tree->free_fun(*(void**)op);
		};
		stack_Drop(stack_op);
	};
	
	pool_Drop(&tag_tree->data_pool);
};
