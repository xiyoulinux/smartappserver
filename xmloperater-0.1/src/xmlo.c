#include "xmlo.h"

XML_DOC* xml_Open(XML_DOC *doc,CHAR_ const *filename,XML_MODE mode)
{
	CHAR_ s_char;
	char *ansi_filename;
	FILE *fp;
	int state;
	XML_NODE **node_list;
	XML_NODE *current_node;
	XML_NODE build_node;
	XML_NODE *pa_node;

	fp=NULL;
	node_list=NULL;
	ansi_filename=NULL;
	
	PRINTF_(ENCODE_("START OPEN\n"));
	#ifndef WITHOUT_WIDECHAR_SUPPORT_
	if(string_Widetoansi(&ansi_filename,filename)==NULL)goto fail_return;
	#else
	ansi_filename=filename;
	#endif

	fp=fopen(ansi_filename,"r");
	if(fp==NULL)goto fail_return;
	node_list=stack_Create(sizeof(XML_NODE*));
	if(node_list==NULL)goto fail_return;

	if(xml_Nodecreate(&build_node)==NULL)goto fail_return;
	memcpy(doc->root,&build_node,sizeof(XML_NODE));
	
	state=INSIDE_BLOCK;
	
	node_list=stack_Append(node_list,&doc->root);
	pa_node=stack_Get(node_list);

	
	for(s_char=FGETC_(fp);s_char!=EOF;s_char=FGETC_(fp))
	{
		switch(state)
		{
			case INSIDE_BLOCK:
			{
				if(s_char==ENCODE_('<'))state=START_HEAD;
				else if(s_char!=ENCODE_('\n')||s_char!=ENCODE_('\t'))
				{
					current_node=stack_Get(node_list);
					if(current_node->data==NULL)current_node->data=array_Create(sizeof(CHAR_));
					current_node->data=array_Append(current_node->data,&s_char);
					if(current_node->data==NULL)goto fail_return;
				}
				else
				{};
				break;
			}
			case START_HEAD:
			{
				if(s_char==ENCODE_('/'))
				{

					while(s_char!=ENCODE_('>'))s_char=FGETC_(fp);

					node_list=stack_Remove(node_list);
					pa_node=stack_Get(node_list);
					state=INSIDE_BLOCK;
				}
				else if(s_char==ENCODE_('!'))
				{
					while(s_char!=ENCODE_('>'))s_char=FGETC_(fp);
					state=INSIDE_BLOCK;
				}
				else
				{
					if(xml_Nodecreate(&build_node)==NULL)goto fail_return;

					if(pa_node->childs==NULL)pa_node->childs=array_Create(sizeof(XML_NODE));
					if(pa_node->childs==NULL)goto fail_return;
					pa_node->childs=array_Append(pa_node->childs,&build_node);
					current_node=&(pa_node->childs[array_Head(pa_node->childs)->array_length-1]);

					node_list=stack_Append(node_list,&current_node);
					pa_node=stack_Get(node_list);
					current_node=pa_node;
					if(current_node->name==NULL)current_node->name=array_Create(sizeof(CHAR_));
					if(current_node->name==NULL)goto fail_return;
					current_node->name=array_Append(current_node->name,&s_char);
					state=INSIDE_HEAD;

				};
				break;
			}
			case INSIDE_HEAD:
			{
				if(s_char==ENCODE_('/'))
				{
					while(s_char!=ENCODE_('>'))s_char=FGETC_(fp);
					if(current_node->name!=NULL)
					{
						s_char=ENCODE_('\0');
						current_node->name=array_Append(current_node->name,&s_char);
					};

					node_list=stack_Remove(node_list);
					pa_node=stack_Get(node_list);
					state=INSIDE_BLOCK;
				}
				else if(s_char==ENCODE_('>'))
				{
					current_node=stack_Get(node_list);
					if(current_node->name!=NULL)
					{
						s_char=ENCODE_('\0');
						current_node->name=array_Append(current_node->name,&s_char);
					};
					state=INSIDE_BLOCK;
				}
				else
				{
					current_node=stack_Get(node_list);
					current_node->name=array_Append(current_node->name,&s_char);
					state=INSIDE_HEAD;
				};
				break;
			};
			default:
			{};/*nothing to do*/
		};
	};

	
	if(array_Head(node_list)->array_length>1)goto fail_return;
	else
	{
		stack_Drop(node_list);
		doc->fp=fp;
		return doc;
	};
	

	fail_return:
	PRINTF_(ENCODE_("Somewhere error!\n"));
	return NULL;
};

XML_NODE* xml_Nodecreate(XML_NODE *node)
{
	node->name=NULL;
	node->data=NULL;
	node->childs=NULL;
	return node;
};

XML_NODE* xml_Nodebyname(CHAR_ const *name,XML_NODE const *source)
{
	size_t i;
	
	if(source->childs!=NULL)for(i=0;i<array_Head(source)->array_length;++i)if(!STRCMP_(source->childs[i].name,name))return &source->childs[i];
	return NULL;
};

C_STRING xml_Parmbyname(CHAR_ const *name,XML_NODE const *source)
{
	#ifndef WITHOUT_WIDECHAR_SUPPORT_
	CHAR_ format[]=ENCODE_(" %S=");
	#else
	CHAR_ format[]=ENCODE_(" %s=");
	#endif
	C_STRING ca_string;
	size_t ca_length;
	KMP_KEY name_key;
	CHAR_ *loc;
	C_STRING parm_data;
	BOOL_ inside_str;

	ca_length=STRLEN_(format)+STRLEN_(name);
	ca_string=array_Create(sizeof(CHAR_));
	ca_string=array_Resize(ca_string,ca_length);

	SNPRINTF_(ca_string,ca_length,format,name);

	if(string_Kprepare(&name_key,ca_string)==NULL)goto fail_return;
	loc=string_Knsearch(source->name,&name_key,STRLEN_(source->name));
	if(loc==NULL)goto fail_return;
	parm_data=array_Create(sizeof(CHAR_));
	inside_str=FALSE_;
	for(loc+=ca_length-2;!(inside_str==FALSE_&&*loc==ENCODE_(' '))&&(*loc!=ENCODE_('\0'))&&(*loc!=ENCODE_('>'));++loc)
	{
		if(*loc==ENCODE_('"')&&inside_str==TRUE_)inside_str==FALSE_;
		else if(*loc==ENCODE_('"'))inside_str=TRUE_;
		parm_data=array_Append(parm_data,loc);
	};
	*loc=ENCODE_('\0');
	parm_data=array_Append(parm_data,loc);

	string_Kfree(&name_key);
	string_Drop(ca_string);
	return parm_data;

	fail_return:
	return NULL;
};

C_STRING xml_Nodename(XML_NODE const *node)
{
	CHAR_ *loc;
	C_STRING node_name;
	CHAR_ end_char;

	node_name=array_Create(sizeof(CHAR_));
	for(loc=node->name;*loc!=ENCODE_(' ')&&*loc!=ENCODE_('\0');++loc)node_name=array_Append(node_name,loc);
	end_char=ENCODE_('\0');
	node_name=array_Append(node_name,&end_char);
	return node_name;
};

XML_NODE* xml_Nodeappend(XML_NODE *dest,XML_NODE const *source)
{
	if(dest->childs==NULL)dest->childs=array_Create(sizeof(XML_NODE));
	if(dest->childs==NULL)goto fail_return;
	dest->childs=array_Append(dest->childs,source);
	return dest;

	fail_return:
	return NULL;
};

XML_NODE* xml_Parmappend(XML_NODE *dest,CHAR_ const *name,CHAR_ const *data)
{
	C_STRING new_parm;
	#ifndef WITHOUT_WIDECHAR_SUPPORT_
	CHAR_ format[]=ENCODE_(" %S=%S");
	#else
	CHAR_ format[]=ENCODE_(" %s=%s");
	#endif
	CHAR_ *op;

	new_parm=array_Create(sizeof(CHAR_));
	new_parm=array_Resize(new_parm,STRLEN_(name)+STRLEN_(data)+STRLEN_(format));
	SNPRINTF_(new_parm,STRLEN_(name)+STRLEN_(data)+STRLEN_(format),format,name,data);
	for(op=new_parm;*op!=ENCODE_('\0');++op)dest->name=array_Append(dest->name,op);
	return dest;
};

XML_NODE* xml_Noderemove(XML_NODE *dest,XML_NODE *tag)
{
	dest->childs=array_Remove(dest->childs,tag);
	return dest;
};

BOOL_ xml_Save(FILE *file,XML_DOC const *doc)
{
	size_t i;
	SAVE_NODE *node_list;
	SAVE_NODE *current_node;
	SAVE_NODE add_node;
	XML_NODE *current_child;
	C_STRING name_cache;

	node_list=stack_Create(sizeof(SAVE_NODE));
	add_node.node=doc->root;
	add_node.n=0;
	node_list=stack_Append(node_list,&add_node);
	do
	{
		current_node=&stack_Get(node_list);
		current_child=&(current_node->node->childs[current_node->n]);
		FPRINTF_(file,ENCODE_("<%S>\n"),current_child->name);
		
		if(current_child->childs!=NULL)
		{
			add_node.node=current_child;
			add_node.n=0;
			node_list=stack_Append(node_list,&add_node);
		}
		else
		{
			if((current_node->n)>=(array_Length(current_node->node->childs)-1))
			{
				node_list=stack_Remove(node_list);
				name_cache=xml_Nodename(current_child);
				FPRINTF_(file,ENCODE_("</%S>\n"),name_cache);
				string_Drop(name_cache);
				name_cache=xml_Nodename(current_node->node);
				FPRINTF_(file,ENCODE_("</%S>\n"),name_cache);
				string_Drop(name_cache);
			}
			else
			{
				if(current_child->data!=NULL)PRINTF_(ENCODE_("%S"),current_child->data);
				name_cache=xml_Nodename(current_child);
				FPRINTF_(file,ENCODE_("</%S>\n"),name_cache);
				string_Drop(name_cache);

				++current_node->n;
			};
		};
		
	}while(array_Length(node_list)>1);

	return TRUE_;

	fail_return:
	return FALSE_;
};

void xml_Nodefree(XML_NODE *node)
{
	
};