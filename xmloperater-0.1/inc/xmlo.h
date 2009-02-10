#include <stdio.h>
#include <cda/c_string.h>
#include <cda/c_stack.h>

#ifndef WITHOUT_WIDECHAR_SUPPORT_

#define XML_HEAD ENCODE_("<%S>")
#define XML_TAIL ENCODE_("</%S>")

#else

#endif

typedef enum xml_mode
{
	STORE_ALL=0,
	STORE_INDEX=1,
	STORE_NONE=2
}XML_MODE;

typedef enum xml_state
{
	START_HEAD=0,
	INSIDE_HEAD=1,
	INSIDE_BLOCK=2
}XML_STATE;

typedef struct xml_node
{
	C_STRING name;
	C_STRING data;
	struct xml_node *childs;
}XML_NODE;

typedef struct xml_doc
{
	FILE *fp;
	XML_NODE *root;
}XML_DOC;

typedef struct save_node
{
	XML_NODE *node;
	size_t n;
}SAVE_NODE;

XML_DOC* xml_Open(XML_DOC *doc,CHAR_ const *filename,XML_MODE mode);

C_STRING xml_Storedata(XML_NODE const *node,XML_DOC const *doc);

XML_NODE* xml_Nodecreate(XML_NODE *node);

#define xml_Nodebyindex(n,source) ((source->childs==NULL)?NULL:(n<array_Head(source->childs)->array_length?&(source->childs[n]):NULL))

XML_NODE* xml_Nodebyname(CHAR_ const *name,XML_NODE const *source);

C_STRING xml_Parmbyname(CHAR_ const *name,XML_NODE const *source);

C_STRING xml_Nodename(XML_NODE const *node);

XML_NODE* xml_Nodeappend(XML_NODE *dest,XML_NODE const *source);

XML_NODE* xml_Parmappend(XML_NODE *dest,CHAR_ const *name,CHAR_ const *data);

XML_NODE* xml_Noderemove(XML_NODE *dest,XML_NODE *tag);

BOOL_ xml_Save(FILE *file,XML_DOC const *doc);

void xml_Nodefree(XML_NODE *node);

void xml_Close(XML_DOC *doc);