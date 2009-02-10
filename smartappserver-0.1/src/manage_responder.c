#include "manage_responder.h"

int main()
{
	SESSION_LIST current_session;
	USER_DATA user_info;
		
	CHAR_ *get_data;
	UINT_ session_id;
	HTML_OP get_op;
	BOOL_ logined;
	
	DB_LINKER db_linker;
	
	/*init*/
	/*session init*/
	session_Create(&current_session,sizeof(USER_DATA));
	
	/*start link the database*/
	
	if(db_Connect(&db_linker,DB_USER,DB_PAS,DB_HOST,DATABASE_NAME_)==NULL)
	{
		
		return 1;
	};
	
	while(FCGI_Accept()>=0)
	{
		/*get login info*/
		logined=(login_Check(&user_info,&current_session)==NULL?FALSE_:TRUE_);
		
		/*now start the command check*/
		get_data=getenv("QUERY_STRING");
		
		if(logined)
		{
			if(get_data==NULL)
			{
				response_Fail();
			}
			else
			{
				/*start command check*/
				op_Get(&get_op,get_data);
				if(get_op.type==NONE)
				{
					/*response_Fail();*//*logined*/
				}
				else if(get_op.type==CHECK)
				{
					if(logined)
					{
						response_Done();
					}
					else
					{
						response_Fail();
					};
					
				}				
				else if(get_op.type==LOGIN)
				{
					if(user_Check(&user_info,&db_linker)!=NULL)
					{				
					session_id=session_Append(&user_info,&current_session);
					
					PRINTF_(ENCODE_("Set-Cookie: user=%s;\n"),user_info.user_id);
					PRINTF_(ENCODE_("Set-Cookie: id=%ld;\n"),session_id);
										
					response_Login_success(&user_info);
					}
					else
					{
						response_Fail();
					};
					
				}
				else if(get_op.type==LOGOUT)
				{
					if(!logined)
					{
						response_Fail();
					}
					else
					{
						session_Remove(&user_info,&current_session);
					
						PRINTF_(ENCODE_("Set-Cookie: user=0;\n"));
						PRINTF_(ENCODE_("Set-Cookie: id=0;\n"));
					
						response_Done();
					};
					
				}
				else if(get_op.type==BROWSE)
				{
					response_Browse(&db_linker);
				}
				else if(get_op.type==CLASS_ST)
				{
					response_Classst(&db_linker);
				}
				else if(get_op.type==ADD_ST)
				{
					response_Addst(&db_linker);
				}
				else if(get_op.type==DEL_ST)
				{
					response_Delst(&db_linker);
				}
				else if(get_op.type==ADD_GRADE)
				{
					response_Addgrade(&db_linker);
				}
				else if(get_op.type==ADD_YEAR)
				{
					response_Addyear(&db_linker);
				}
				else if(get_op.type==ADD_CLASS)
				{
					response_Addclass(&db_linker);
				}
				else if(get_op.type==CLASS_SCORE)
				{
					response_Classscore(&db_linker);
				}
				else if(get_op.type==COUNT)
				{
					/*response_Score(html_cache,&user_info,&get_op,&db_linker);*/					
					
				};
				
			};
				
		}
		else
		{			
			op_Get(&get_op,get_data);
			if(get_op.type==LOGIN)
			{
				if(user_Check(&user_info,&db_linker)!=NULL)
				{				
				session_id=session_Append(&user_info,&current_session);
				
				PRINTF_(ENCODE_("Set-Cookie: user=%s;\n"),user_info.user_id);
				PRINTF_(ENCODE_("Set-Cookie: id=%ld;\n"),session_id);
									
				response_Login_success(&user_info);
				}
				else
				{
					response_Fail();
				};
			}
			else
			{
				response_Fail();
			};
		};
	};
	session_Drop(&current_session);
	return 0;
};
