#ifndef CGI_FUNS_H
#define CGI_FUNS_H

#include <fcgi_stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include <cda/c_prtree.h>
#include <cda/c_define.h>
#include <dbo/db_funs.h>
#include <cda/c_array.h>
#include <cda/c_stack.h>
#include <cda/c_string.h>

/*input output*/
/*#define FCGI_PRINTF_(format,...) (FPRINTF_((FCGI_stdout),(format),(...)))*/

#define OUT_HEAD_() (PRINTF_("Content-Type:text/plain;charset=utf-8\n\n"))

#define NAME_SIZE_ 500
#define SQL_LENGTH_ 1000

#define DATABASE_NAME_ ENCODE_("st_database")

#define SESSION_MAX_TIME_ 60*30

typedef enum user_type
{
	ST=0,
	TA=1,
	MA=2
}USER_TYPE;

typedef struct user_data
{
	CHAR_ user_name[NAME_SIZE_];
	CHAR_ user_id[NAME_SIZE_];
	USER_TYPE user_level;
}USER_DATA;

typedef struct session_node
{
	UINT_ session_id;
	time_t last_time;
	USER_DATA user_data;
}SESSION_NODE;

typedef struct session_list
{
	C_PRTREE session_tree;

	C_POOL data_pool;
	UINT_ last_id;
	size_t data_size;	
}SESSION_LIST;

BOOL_ session_Compare(struct session_node *lhs,struct session_node *rhs);

SESSION_LIST* session_Create(SESSION_LIST *tag_list,size_t data_size);

UINT_ session_Append(USER_DATA *user_data,struct session_list *in_list);

SESSION_NODE* session_Get(CHAR_ *user_name,struct session_list *in_list);

void session_Remove(USER_DATA *user_data,SESSION_LIST *in_list);

void session_Clean(struct session_list *in_list);

void session_Drop(struct session_list *in_list);
/*end*/

/*usful*/
CHAR_* ca_Get(CHAR_ *tag_array,CHAR_ *key_string,CHAR_ *tag_string);

CHAR_* get_Post(CHAR_ *tag_string);

/*stander op*/
BOOL_ debug_Output(CHAR_ *message);

#endif
