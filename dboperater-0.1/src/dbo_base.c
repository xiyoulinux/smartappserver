#include "dbo_base.h"
/*
void db_Init()
{
	
};

struct db_linker* db_Connect(struct db_linker *link,CHAR_ *user_name,CHAR_ *user_password,CHAR_ *server_name,CHAR_ *database_name)
{	
	link->sql_link=mysql_init(NULL);
	
	mysql_real_connect(link->sql_link,server_name,user_name,user_password,database_name,0,NULL,CLIENT_MULTI_STATEMENTS);
	
	if(link->sql_link==NULL)return NULL;
	return link;
};

struct db_data* db_Query(struct db_data *result_data,CHAR_ *sql_command,struct db_linker *tag_db)
{
	if(mysql_query(tag_db->sql_link,sql_command)!=0)return NULL;
	result_data->sql_data=mysql_store_result(tag_db->sql_link);
	
	return result_data;
};

BOOL_ db_Batchquery(CHAR_ *command_list[],UINT_ list_count,DB_LINKER *tag_db)
{
	int i;
	for(i=0;i<list_count;++i)if(mysql_query(tag_db->sql_link,command_list[i])!=0)return FALSE_;
	return TRUE_;	
};

struct db_row* db_Fetch(struct db_row *result_row,struct db_data *tag_data)
{
	result_row->sql_row=mysql_fetch_row(tag_data->sql_data);
	if(result_row->sql_row==NULL)return NULL;
	return result_row;
};

CHAR_* db_Get(int field_count,struct db_row *tag_row)
{
	return tag_row->sql_row[field_count];
};

void db_Free_data(struct db_data *tag_data)
{
	mysql_free_result(tag_data->sql_data);
};

void db_Drop(struct db_linker *tag_db)
{
	
};

void db_End()
{
	mysql_library_end();
};
*/