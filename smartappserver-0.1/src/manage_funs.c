#include "manage_funs.h"

USER_DATA* login_Check(USER_DATA *tag_data,SESSION_LIST *tag_session)
{
	CHAR_ tag_string[NAME_SIZE_];
	SESSION_NODE *tag_node;
	CHAR_ *cookie;
	
	/*读取user_name*/
	cookie=getenv("HTTP_COOKIE");
	if(cookie==NULL)return NULL;
	ca_Get(tag_string,"user",cookie);
	
	tag_node=session_Get(tag_string,tag_session);
	
	if(tag_node==NULL)
	{
		return NULL;
	}
	else
	{	
		ca_Get(tag_string,"id",cookie);

		if(atoi(tag_string)==tag_node->session_id)
		{
			memcpy(tag_data,&tag_node->user_data,sizeof(USER_DATA));
			return tag_data;
		}
		else
		{
			return NULL;
		};
	};
	
	return NULL;
};

USER_DATA* user_Check(USER_DATA *tag_data,DB_LINKER *tag_db)
{
	
	CHAR_ user_id[NAME_SIZE_];
	CHAR_ user_pas[NAME_SIZE_];
	CHAR_ *user_name;
	CHAR_ *user_no;
	CHAR_ post_data[200];
	CHAR_ sql_command[SQL_LENGTH_];
	DB_DATA res_data;
	DB_ROW res_row;
	
	if(get_Post(post_data)==NULL)
	{
		return NULL;
	};
	ca_Get(user_id,"user",post_data);
	ca_Get(user_pas,"password",post_data);
	
	if(!input_Check(user_id)||!input_Check(user_pas))return NULL;
	
	SPRINTF_(sql_command,SQL_CHECK_USER_,user_id,user_pas);
	if(db_Query(&res_data,sql_command,tag_db)==NULL)return NULL;
	if(db_Fetch(&res_row,&res_data)==NULL)return NULL;
	
	user_no=db_Get(0,&res_row);
	user_name=db_Get(1,&res_row);
	
	STRCPY_(tag_data->user_name,user_name);
	STRCPY_(tag_data->user_id,user_no);
	
	tag_data->user_level=TA;
	
	db_Free_data(&res_data);
	
	return tag_data;
};

BOOL_ input_Check(CHAR_ *in_char)
{
	return TRUE_;
};

CHAR_ level_Convert(int level)
{
	return (CHAR_)(level+'a'-1);
};

CHAR_* id_Create(CHAR_ *tag_string)
{
	static UINT_ counter=0;
	struct tm *current_time_tm;
	time_t current_time_t;
	
	time(&current_time_t);
	current_time_tm=localtime(&current_time_t);
	
	strftime(tag_string,50,"%Y%m%d%H%M%S",current_time_tm);
	sprintf(tag_string,"%s%ld",tag_string,counter);
	
	++counter;
	
	return tag_string;
};

void response_Done()
{
	OUT_HEAD_();
	OUT_RESULT_("1");
};

void response_Fail()
{
	OUT_HEAD_();
	OUT_RESULT_("0");
};

void response_Login_success(USER_DATA *user_info)
{
	OUT_HEAD_();
	OUT_RESULT_("1");
	PRINTF_(ITEM_BEGIN_,"name");
	PRINTF_(user_info->user_name);
	PRINTF_(ITEM_END_,"name");
};

void response_Browse(DB_LINKER *tag_db)
{
	DB_DATA res_data;
	DB_ROW res_row;
	
	BOOL_ time_flag;
	
	CHAR_ time[NAME_SIZE_];
	CHAR_ level[NAME_SIZE_];
	
	if(db_Query(&res_data,SQL_BROWSE_,tag_db)==NULL)
	{
		return;
	};
	
	OUT_HEAD_();
	OUT_RESULT_("1");
	
	*time='\0';
	*level='\0';
	
	time_flag=FALSE_;
	while(db_Fetch(&res_row,&res_data)!=NULL)
	{
		
		if(STRCMP_(time,db_Get(0,&res_row)))
		{
			if(*time!='\0')
			{
				PRINTF_(ITEM_END_C_,level_Convert(atoi(level)));
				PRINTF_(ITEM_END_,time);
				time_flag=TRUE_;
			};
			strcpy(time,db_Get(0,&res_row));
			PRINTF_(ITEM_BEGIN_,time);
		};
		if(STRCMP_(level,db_Get(1,&res_row)))
		{
			if(*level!='\0'&&time_flag!=TRUE_)PRINTF_(ITEM_END_C_,level_Convert(atoi(level)));
			strcpy(level,db_Get(1,&res_row));
			PRINTF_(ITEM_BEGIN_C_,level_Convert(atoi(level)));
			time_flag=FALSE_;
		};
		
		PRINTF_(ITEM_BEGIN_,db_Get(3,&res_row));
		PRINTF_(db_Get(2,&res_row));
		PRINTF_(ITEM_END_,db_Get(3,&res_row));		
	};
	
	PRINTF_(ITEM_END_C_,level_Convert(atoi(level)));
	PRINTF_(ITEM_END_,time);
	
};

void response_Classst(DB_LINKER *tag_db)
{
	DB_DATA res_data;
	DB_ROW res_row;
	CHAR_ post_data[200];
	CHAR_ sql_command[SQL_LENGTH_];
	int i;
	
	if(get_Post(post_data)==NULL)
	{
		response_Fail();
		return;
	};
	
	if(!input_Check(post_data))
	{
		response_Fail();
		return;
	};
	
	SPRINTF_(sql_command,SQL_CLASSST_,post_data);
	
	if(db_Query(&res_data,sql_command,tag_db)==NULL)
	{
		response_Fail();
		return;
	};
	
	i=0;
	OUT_HEAD_();
	
	OUT_RESULT_("1");
	while(db_Fetch(&res_row,&res_data)!=NULL)
	{
		PRINTF_(ITEM_BEGIN_I_,i);
		PRINTF_(CLASSST_OUT_,db_Get(0,&res_row),db_Get(1,&res_row));
		PRINTF_(ITEM_END_I_,i);
		++i;
	};
	
	db_Free_data(&res_data);
};

void response_Addst(DB_LINKER *tag_db)
{
	DB_DATA res_data;
	CHAR_ st_no[NAME_SIZE_];
	CHAR_ st_name[NAME_SIZE_];
	CHAR_ class_no[NAME_SIZE_];
	CHAR_ post_data[200];
	CHAR_ sql_command[SQL_LENGTH_];
	
	if(get_Post(post_data)==NULL)
	{
		response_Fail();
		return;
	};
	
	if((ca_Get(st_no,"no",post_data)==NULL)||(ca_Get(st_name,"name",post_data)==NULL)||(ca_Get(class_no,"class",post_data)==NULL))
	{
		response_Fail();
		return;
	};
	
	if(!input_Check(st_no)||!input_Check(st_name)||!input_Check(class_no))
	{
		response_Fail();
		return;
	};
		
	SPRINTF_(sql_command,SQL_ADDST_A,st_no,st_name);
	
	if(db_Query(&res_data,sql_command,tag_db)==NULL)
	{
		response_Fail();
		return;
	};
	
	db_Free_data(&res_data);
	
	SPRINTF_(sql_command,SQL_ADDST_B,st_no,class_no);
	
	if(db_Query(&res_data,sql_command,tag_db)==NULL)
	{
		response_Fail();
		return;
	};
	
	db_Free_data(&res_data);
		
	OUT_HEAD_();
	
	OUT_RESULT_(ENCODE_("1"));
};

void response_Delst(DB_LINKER *tag_db)
{
	DB_DATA res_data;
	CHAR_ st_no[NAME_SIZE_];
	CHAR_ post_data[200];
	CHAR_ sql_command[SQL_LENGTH_];
	
	/*read data from post*/
	if(get_Post(post_data)==NULL)
	{
		response_Fail();
		return;
	};
	
	ca_Get(st_no,"no",post_data);
	
	SPRINTF_(sql_command,SQL_DELST_A,st_no);
		
	if(db_Query(&res_data,sql_command,tag_db)==NULL)
	{
		response_Fail();
		return;
	};
	
	SPRINTF_(sql_command,SQL_DELST_B,st_no);
		
	if(db_Query(&res_data,sql_command,tag_db)==NULL)
	{
		response_Fail();
		return;
	};
	
	SPRINTF_(sql_command,SQL_DELST_C,st_no);
		
	if(db_Query(&res_data,sql_command,tag_db)==NULL)
	{
		response_Fail();
		return;
	};
	
	SPRINTF_(sql_command,SQL_DELST_D,st_no);
		
	if(db_Query(&res_data,sql_command,tag_db)==NULL)
	{
		response_Fail();
		return;
	};
	
	db_Free_data(&res_data);
		
	OUT_HEAD_();
	
	OUT_RESULT_(ENCODE_("1"));
	
};

void response_Addgrade(DB_LINKER *tag_db)
{
	DB_DATA res_data;
	DB_ROW res_row;
	
	CHAR_ post_data[200];
	CHAR_ sql_command[SQL_LENGTH_];
	CHAR_ new_id[NAME_SIZE_];
	CHAR_ new_level[NAME_SIZE_];
	CHAR_ grade[NAME_SIZE_];
	C_STRING s_command;
	C_STRING *command_list;
	C_STRING s_grade;
	C_STRING *grade_list;
	C_STRING n_s_grade;
	C_STRING *new_grade_list;
	C_STRING s_class;
	C_STRING *class_list;
	C_STRING n_s_class;
	C_STRING *new_class_list;
	
	CHAR_ *get_op;
	UINT_ i;
	
	/*read data from post*/
	if(get_Post(post_data)==NULL)
	{
		response_Fail();
		return;
	};
	
	/*first step,get the new grades*/
	SPRINTF_(sql_command,SQL_ADD_GRADE_A,post_data);
	if(db_Query(&res_data,sql_command,tag_db)==NULL)
	{
		response_Fail();
		return;
	};
	
	command_list=array_Create_Ex(sizeof(C_ARRAY),&string_Drop);
	grade_list=array_Create_Ex(sizeof(C_ARRAY),&string_Drop);
	new_grade_list=array_Create_Ex(sizeof(C_ARRAY),&string_Drop);
	class_list=array_Create_Ex(sizeof(C_ARRAY),&string_Drop);
	new_class_list=array_Create_Ex(sizeof(C_ARRAY),&string_Drop);
	
	while(db_Fetch(&res_row,&res_data)!=NULL)
	{
		SPRINTF_(sql_command,SQL_ADD_GRADE_B,id_Create(new_id),db_Get(0,&res_row),db_Get(1,&res_row));
		s_command=string_Create(sql_command);
		command_list=array_Append(&s_command,command_list);
		/*add the grade list*/
		s_grade=string_Create(db_Get(2,&res_row));
		n_s_grade=string_Create(new_id);
		grade_list=array_Append(&s_grade,grade_list);
		new_grade_list=array_Append(&n_s_grade,new_grade_list);
	};
	db_Free_data(&res_data);
	
	/*second step,get the new classes*/
	
	for(i=0;i<array_Head(grade_list)->array_length;++i)
	{
		SPRINTF_(sql_command,SQL_ADD_GRADE_C,grade_list[i]);
		if(db_Query(&res_data,sql_command,tag_db)==NULL)
		{
			response_Fail();
			for(i=0;i<array_Head(command_list)->array_length;++i)string_Drop(command_list[i]);
			array_Drop(command_list);
			return;
		};
	
		while(db_Fetch(&res_row,&res_data)!=NULL)
		{
			SPRINTF_(sql_command,SQL_ADD_GRADE_D,db_Get(0,&res_row),id_Create(new_id),new_grade_list[i]);
			s_command=string_Create(sql_command);
			command_list=array_Append(&s_command,command_list);
			
			s_class=string_Create(db_Get(1,&res_row));
			class_list=array_Append(&s_class,class_list);
			n_s_class=string_Create(new_id);
			new_class_list=array_Append(&n_s_class,new_class_list);
		};
		db_Free_data(&res_data);
	};	
	
	/*third step,set the new st*/
	
	for(i=0;i<array_Head(class_list)->array_length;++i)
	{
		SPRINTF_(sql_command,SQL_ADD_GRADE_E,class_list[i]);
		if(db_Query(&res_data,sql_command,tag_db)==NULL)
		{
			response_Fail();
			array_Drop(command_list);
			array_Drop(new_grade_list);
			array_Drop(grade_list);
			array_Drop(class_list);
			array_Drop(new_class_list);
			return;
		};
	
		while(db_Fetch(&res_row,&res_data)!=NULL)
		{
			SPRINTF_(sql_command,SQL_ADD_GRADE_F,new_class_list[i],db_Get(0,&res_row));
			s_command=string_Create(sql_command);
			command_list=array_Append(&s_command,command_list);
		};
		db_Free_data(&res_data);
	};
	
	
	if(!db_Batchquery((CHAR_**)command_list,array_Head(command_list)->array_length,tag_db))
	{
		array_Drop(command_list);
		array_Drop(new_grade_list);
		array_Drop(grade_list);
		array_Drop(class_list);
		array_Drop(new_class_list);
		
		response_Fail();
		return;
	};	
	
	array_Drop(command_list);
	array_Drop(new_grade_list);
	array_Drop(grade_list);
	array_Drop(class_list);
	array_Drop(new_class_list);
	
	/*db_Free_data(&res_data);*/
	
	OUT_HEAD_();
	
	OUT_RESULT_(ENCODE_("1"));
};

void response_Addyear(DB_LINKER *tag_db)
{
	DB_DATA res_data;
	DB_ROW res_row;
	
	CHAR_ post_data[200];
	CHAR_ sql_command[SQL_LENGTH_];
	CHAR_ new_id[NAME_SIZE_];
	CHAR_ new_ids[NAME_SIZE_];
	CHAR_ new_level[NAME_SIZE_];
	CHAR_ grade[NAME_SIZE_];
	CHAR_ current_grade[NAME_SIZE_];
	C_STRING s_command;
	C_STRING *command_list;
	C_STRING s_grade;
	C_STRING *grade_list;
	C_STRING n_s_grade;
	C_STRING *new_grade_list;
	C_STRING s_class;
	C_STRING *class_list;
	C_STRING n_s_class;
	C_STRING *new_class_list;
	
	CHAR_ *get_op;
	UINT_ i;
	
	/*first step,get the new grades*/
	STRCPY_(sql_command,SQL_ADD_YEAR_A);
	if(db_Query(&res_data,sql_command,tag_db)==NULL)
	{
		response_Fail();
		return;
	};
	
	command_list=array_Create_Ex(sizeof(C_ARRAY),&string_Drop);
	grade_list=array_Create_Ex(sizeof(C_ARRAY),&string_Drop);
	new_grade_list=array_Create_Ex(sizeof(C_ARRAY),&string_Drop);
	class_list=array_Create_Ex(sizeof(C_ARRAY),&string_Drop);
	new_class_list=array_Create_Ex(sizeof(C_ARRAY),&string_Drop);
	
	*current_grade='\0';
	while(db_Fetch(&res_row,&res_data)!=NULL)
	{
		SPRINTF_(sql_command,SQL_ADD_YEAR_B,id_Create(new_id),db_Get(1,&res_row),db_Get(2,&res_row));
		if(*current_grade=='\0')strcpy(current_grade,db_Get(2,&res_row));
		s_command=string_Create(sql_command);
		command_list=array_Append(&s_command,command_list);
		/*add the grade list*/
		s_grade=string_Create(db_Get(0,&res_row));
		n_s_grade=string_Create(new_id);
		grade_list=array_Append(&s_grade,grade_list);
		new_grade_list=array_Append(&n_s_grade,new_grade_list);
	};
	
	/*add the grade 1*/
	
	SPRINTF_(sql_command,SQL_ADD_YEAR_G,id_Create(new_id),current_grade);
	s_command=string_Create(sql_command);
	command_list=array_Append(&s_command,command_list);	
	
	SPRINTF_(sql_command,SQL_ADD_YEAR_H,id_Create(new_ids),new_id);	
	s_command=string_Create(sql_command);
	command_list=array_Append(&s_command,command_list);
	
	SPRINTF_(sql_command,SQL_ADD_YEAR_I,new_ids);
	s_command=string_Create(sql_command);
	command_list=array_Append(&s_command,command_list);
	
	db_Free_data(&res_data);
	
	/*second step,get the new classes*/
	
	for(i=0;i<array_Head(grade_list)->array_length;++i)
	{
		SPRINTF_(sql_command,SQL_ADD_YEAR_C,grade_list[i]);
		if(db_Query(&res_data,sql_command,tag_db)==NULL)
		{
			response_Fail();
			for(i=0;i<array_Head(command_list)->array_length;++i)string_Drop(command_list[i]);
			array_Drop(command_list);
			return;
		};
	
		while(db_Fetch(&res_row,&res_data)!=NULL)
		{
			SPRINTF_(sql_command,SQL_ADD_YEAR_D,db_Get(0,&res_row),id_Create(new_id),new_grade_list[i]);
			s_command=string_Create(sql_command);
			command_list=array_Append(&s_command,command_list);
			
			s_class=string_Create(db_Get(1,&res_row));
			class_list=array_Append(&s_class,class_list);
			n_s_class=string_Create(new_id);
			new_class_list=array_Append(&n_s_class,new_class_list);
		};
		db_Free_data(&res_data);
	};	
	
	/*third step,set the new st*/
	
	for(i=0;i<array_Head(class_list)->array_length;++i)
	{
		SPRINTF_(sql_command,SQL_ADD_YEAR_E,class_list[i]);
		if(db_Query(&res_data,sql_command,tag_db)==NULL)
		{
			response_Fail();
			array_Drop(command_list);
			array_Drop(new_grade_list);
			array_Drop(grade_list);
			array_Drop(class_list);
			array_Drop(new_class_list);
			return;
		};
	
		while(db_Fetch(&res_row,&res_data)!=NULL)
		{
			SPRINTF_(sql_command,SQL_ADD_YEAR_F,new_class_list[i],db_Get(0,&res_row));
			s_command=string_Create(sql_command);
			command_list=array_Append(&s_command,command_list);
		};
		db_Free_data(&res_data);
	};
	
	if(!db_Batchquery((CHAR_**)command_list,array_Head(command_list)->array_length,tag_db))
	{
		array_Drop(command_list);
		array_Drop(new_grade_list);
		array_Drop(grade_list);
		array_Drop(class_list);
		array_Drop(new_class_list);
		
		response_Fail();
		return;
	};	
	
	array_Drop(command_list);
	array_Drop(new_grade_list);
	array_Drop(grade_list);
	array_Drop(class_list);
	array_Drop(new_class_list);
	
	db_Free_data(&res_data);
	
	OUT_HEAD_();
	
	OUT_RESULT_(ENCODE_("1"));
};

void response_Addclass(DB_LINKER *tag_db)
{
	DB_DATA res_data;
	DB_ROW res_row;
	
	CHAR_ new_id[NAME_SIZE_];
	CHAR_ class_no[NAME_SIZE_];
	CHAR_ class_num[NAME_SIZE_];
	CHAR_ grade_no[NAME_SIZE_];
	CHAR_ post_data[200];
	CHAR_ sql_command[SQL_LENGTH_];
	
	if(get_Post(post_data)==NULL)
	{
		response_Fail();
		return;
	};
	
	if((ca_Get(class_no,"no",post_data)==NULL)||(ca_Get(class_num,"num",post_data)==NULL))
	{
		response_Fail();
		return;
	};
	
	if(!input_Check(class_no)||!input_Check(class_num))
	{
		response_Fail();
		return;
	};
		
	SPRINTF_(sql_command,SQL_ADD_CLASS_A,class_no);
	
	if(db_Query(&res_data,sql_command,tag_db)==NULL)
	{
		response_Fail();
		return;
	};
	
	db_Fetch(&res_row,&res_data);
	
	STRCPY_(grade_no,db_Get(0,&res_row));
	
	db_Free_data(&res_data);
	
	SPRINTF_(sql_command,SQL_ADD_CLASS_B,id_Create(new_id),class_num,grade_no);
	
	if(db_Query(&res_data,sql_command,tag_db)==NULL)
	{
		response_Fail();
		return;
	};
	
	db_Free_data(&res_data);
	
	SPRINTF_(sql_command,SQL_ADD_CLASS_C,new_id);
	
	if(db_Query(&res_data,sql_command,tag_db)==NULL)
	{
		response_Fail();
		return;
	};
	
	db_Free_data(&res_data);
	
	OUT_HEAD_();
	
	OUT_RESULT_(ENCODE_("1"));
};

void response_Classscore(DB_LINKER *tag_db)
{
	DB_DATA res_data;
	DB_ROW res_row;
	UINT_ i;
	
	CHAR_ class_no[NAME_SIZE_];
	CHAR_ post_data[200];
	CHAR_ sql_command[SQL_LENGTH_];
	
	if(get_Post(post_data)==NULL)
	{
		response_Fail();
		return;
	};
		
	memcpy(class_no,post_data,NAME_SIZE_);
	
	if(!input_Check(class_no))
	{
		response_Fail();
		return;
	};
		
	SPRINTF_(sql_command,SQL_CLASS_SCORE_,class_no,class_no);
	
	if(db_Query(&res_data,sql_command,tag_db)==NULL)
	{
		response_Fail();
		return;
	};
	
	i=0;
	OUT_HEAD_();
	
	OUT_RESULT_(ENCODE_("1"));
	while(db_Fetch(&res_row,&res_data)!=NULL)
	{
		PRINTF_(ITEM_BEGIN_I_,i);
		PRINTF_(CLASSSCORE_OUT_,db_Get(0,&res_row),db_Get(1,&res_row),db_Get(2,&res_row),db_Get(3,&res_row),db_Get(4,&res_row),db_Get(5,&res_row),db_Get(6,&res_row),db_Get(7,&res_row),db_Get(8,&res_row),db_Get(9,&res_row),db_Get(10,&res_row),db_Get(11,&res_row));
		PRINTF_(ITEM_END_I_,i);
		++i;
	};	
	
	db_Free_data(&res_data);	
};

HTML_OP* op_Get(HTML_OP *tag_op,CHAR_ *get_data)
{
	CHAR_ op[NAME_SIZE_];
		
	if(ca_Get(op,ENCODE_("op"),get_data)==NULL)
	{
		tag_op->type=NONE;
		return tag_op;
	};
	
	if(!STRCMP_(op,ENCODE_("check")))
	{
		tag_op->type=CHECK;
	}
	else if(!STRCMP_(op,ENCODE_("login")))
	{
		tag_op->type=LOGIN;
	}
	else if(!STRCMP_(op,ENCODE_("logout")))
	{
		tag_op->type=LOGOUT;
	}
	else if(!STRCMP_(op,ENCODE_("browse")))
	{
		tag_op->type=BROWSE;
	}
	else if(!STRCMP_(op,ENCODE_("classst")))
	{
		tag_op->type=CLASS_ST;
	}
	else if(!STRCMP_(op,ENCODE_("addst")))
	{
		tag_op->type=ADD_ST;
	}
	else if(!STRCMP_(op,ENCODE_("delst")))
	{
		tag_op->type=DEL_ST;
	}
	else if(!STRCMP_(op,ENCODE_("addgrade")))
	{
		tag_op->type=ADD_GRADE;
	}
	else if(!STRCMP_(op,ENCODE_("addyear")))
	{
		tag_op->type=ADD_YEAR;
	}
	else if(!STRCMP_(op,ENCODE_("addclass")))
	{
		tag_op->type=ADD_CLASS;
	}
	else if(!STRCMP_(op,ENCODE_("classscore")))
	{
		tag_op->type=CLASS_SCORE;
	}
	else if(!STRCMP_(op,ENCODE_("count")))
	{
		tag_op->type=COUNT;
	}
	else
	{
		tag_op->type=NONE;
	};
	return tag_op;
};
