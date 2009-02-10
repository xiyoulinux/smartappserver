#ifndef DB_FUNS_H
#define DB_FUNS_H

#include <cda/c_define.h>
#include <malloc.h>
#include <cda/c_string.h>

typedef enum link_type
{
	MYSQL=0,
	SQLITE=1
}LINK_TYPE;

typedef struct dbo_link
{
	LINK_TYPE dbo_type;
	
	/*MYSQL *sql_link;*/
}DBO_LINK;

typedef struct dbo_data
{
	
	/*MYSQL_RES *sql_data;*/
}DBO_RESULT;

typedef struct dbo_row
{
	
	/*MYSQL_ROW sql_row;*/
}DBO_ROW;

typedef struct dbo_p
{
	DBO_LINK* (*dbo_Connect)();
	DBO_RESULT* (*dbo_Query)();
	BOOL_ (*dbo_Exec)();
	DBO_ROW* (*dbo_Fetch)();
}DBO_FUN;

void dbo_Init();

DBO_LINK* dbo_Connect(DBO_LINK *link,CHAR_ *user_name,CHAR_ *user_password,CHAR_ *server_name,CHAR_ *database_name);

DBO_RESULT* dbo_Query(DBO_RESULT *result_data,CHAR_ *sql_command,DBO_LINK *link);

BOOL_ dbo_Exec(CHAR_ *sql_command,DBO_LINK *tag_db);

DBO_ROW* dbo_Fetch(DBO_ROW *result_row,DBO_RESULT *tag_data);

DBO_ROW* dbo_Fetch_Ex(DBO_ROW *result_row,UINT_ row,DBO_LINK *link);

CHAR_* dbo_Get(UINT_ field,DBO_ROW *tag_row);

void dbo_Free_result(DBO_RESULT *tag_data);

void dbo_Drop(DBO_LINK *tag_db);

void dbo_End();

#endif
