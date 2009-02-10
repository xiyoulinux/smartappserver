#ifndef MANAGE_FUNS_H
#define MANAGE_FUNS_H

#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "cgi_funs.h"

#include <cda/c_prtree.h>
#include <dbo/db_funs.h>
#include <cda/c_array.h>
#include <cda/c_stack.h>

#define SQL_CHECK_USER_ ENCODE_("select manage_no,info_value from manage_info where info_name=0 and manage_no = (select manage_no from manage_list where manage_no=%s and manage_password=%s)")
#define SQL_CHECK_INFO_ ENCODE_("select info_name,info_value from manage_info where manage_no=%s order by info_name")

#define SQL_CHECK_LIST_ALL_ ENCODE_("select class_no\,class_num,grade_no from class_list where grade_no in (select grade_no from grade_list where build_time=%s)")

#define SQL_CHECK_SCORE_ALL_ ENCODE_("select grade_list.build_time,grade_list.grade_level,subject_list.type_name,subject_list.mod_name,st_score.score_value from st_score,grade_list,subject_list where st_no=%s and st_score.score_name=subject_list.subject_name  and grade_list.grade_no=(select grade_no from class_list where class_no=(select class_no from class_st where st_no=%s))")
#define SQL_CHECK_SCORE_NOPASS_ ENCODE_("select grade_list.build_time,grade_list.grade_level,subject_list.type_name,subject_list.mod_name,st_score.score_value from st_score,grade_list,subject_list where st_no=%s and st_score.score_name=subject_list.subject_name  and  st_score.score_value<60 and grade_list.grade_no=(select grade_no from class_list where class_no=(select class_no from class_st where st_no=%s))")

/*new define*/

#define SQL_BROWSE_ ENCODE_("select grade_list.build_time,grade_list.grade_level,class_list.class_no,class_list.class_num from grade_list,class_list where class_list.grade_no=grade_list.grade_no order by build_time,grade_level,class_num")

#define SQL_CLASSST_ ENCODE_("select class_st.st_no,st_list.st_name from class_st,st_list where class_st.st_no=st_list.st_no and class_st.class_no=%s")

#define SQL_ADDST_A ENCODE_("insert into st_list set st_no=%s,st_name=%s")

#define SQL_ADDST_B ENCODE_("insert into class_st set st_no=%s,class_no=%s")

#define SQL_DELST_A ENCODE_("delete from st_list where st_no=%s")

#define SQL_DELST_B ENCODE_("delete from st_info where st_no=%s")

#define SQL_DELST_C ENCODE_("delete from class_st where st_no=%s")

#define SQL_DELST_D ENCODE_("delete from st_score where st_no=%s")

#define SQL_ADD_GRADE_A ENCODE_("select grade_level+1,build_time,grade_no from grade_list where build_time=(select build_time from grade_list where grade_no=(select grade_no from class_list where class_no=%s))")

#define SQL_ADD_GRADE_B ENCODE_("insert into grade_list set grade_no=%s,grade_level=%s,build_time=%s,grade_info=\"nothing\"")

#define SQL_ADD_GRADE_C ENCODE_("select class_num,class_no from class_list where grade_no=%s")

#define SQL_ADD_GRADE_D ENCODE_("insert into class_list set class_num=%s,class_no=%s,grade_no=%s,class_info=\"nothing\"")

#define SQL_ADD_GRADE_E ENCODE_("select st_no from class_st where class_no=%s")

#define SQL_ADD_GRADE_F ENCODE_("insert into class_st set class_no=%s,st_no=%s")

#define SQL_ADD_YEAR_A ENCODE_("select grade_no,grade_level+1,build_time+1 from grade_list where build_time=(select MAX(build_time) from grade_list) and grade_level<5 and grade_level%2=0 order by grade_level")

#define SQL_ADD_YEAR_B ENCODE_("insert into grade_list set grade_no=%s,grade_level=%s,build_time=%s,grade_info=\"nothing\"")

#define SQL_ADD_YEAR_C ENCODE_("select class_num,class_no from class_list where grade_no=%s")

#define SQL_ADD_YEAR_D ENCODE_("insert into class_list set class_num=%s,class_no=%s,grade_no=%s,class_info=\"nothing\"")

#define SQL_ADD_YEAR_E ENCODE_("select st_no from class_st where class_no=%s")

#define SQL_ADD_YEAR_F ENCODE_("insert into class_st set class_no=%s,st_no=%s")

#define SQL_ADD_YEAR_G ENCODE_("insert into grade_list set grade_no=%s,grade_level=1,build_time=%s,grade_info=\"nothing\"")

#define SQL_ADD_YEAR_H ENCODE_("insert into class_list set class_num=1,class_no=%s,grade_no=%s,class_info=\"nothing\"")

#define SQL_ADD_YEAR_I ENCODE_("insert into class_st set class_no=%s,st_no=0")

#define SQL_ADD_CLASS_A ENCODE_("select grade_no from class_list where class_no=%s")

#define SQL_ADD_CLASS_B ENCODE_("insert into class_list set class_no=%s,class_num=%s,grade_no=%s,class_info=\"nothing\"")

#define SQL_ADD_CLASS_C ENCODE_("insert into class_st set class_no=%s,st_no=0")

#define SQL_CLASS_SCORE_ ENCODE_(" select grade_list.grade_no,grade_list.grade_level,class_list.class_no,class_list.class_num,st_list.st_no,st_list.st_name,score_type.type_name,subject_list.subject_name,subject_list.type_name,subject_list.mod_name,st_score.score_value from grade_list,class_list,st_list,subject_list,st_score,score_type where grade_list.grade_no=class_list.grade_no and score_type.type_id=st_score.score_type  and st_list.st_no=st_score.st_no and st_list.st_no in (select st_no from class_st where class_no=%s) and class_list.class_no=%s and subject_list.subject_name=st_score.score_name and class_list.class_no=st_score.class_no order by st_list.st_no")

#define SQL_GRADE_SCORE_ ENCODE_("")

#define ITEM_BEGIN_ ENCODE_("<%s>")

#define ITEM_BEGIN_C_ ENCODE_("<%c>")

#define ITEM_BEGIN_I_ ENCODE_("<%d>")

#define ITEM_END_ ENCODE_("</%s>")

#define ITEM_END_C_ ENCODE_("</%c>")

#define ITEM_END_I_ ENCODE_("</%d>")

#define RESULT_TYPE_ ENCODE_("result")

#define OUT_RESULT_(result) PRINTF_(ITEM_BEGIN_,RESULT_TYPE_);PRINTF_(result);PRINTF_(ITEM_END_,RESULT_TYPE_)

#define CLASSST_OUT_ ENCODE_("%s,%s")

#define CLASSSCORE_OUT_ ENCODE_("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s")

typedef enum op_type
{
	NONE=0,
	CHECK=1,
	LOGIN=2,
	LOGOUT=3,
	BROWSE=4,
	CLASS_ST=5,
	ADD_ST=6,
	DEL_ST=7,
	ADD_GRADE=8,
	ADD_YEAR=9,
	ADD_CLASS=10,
	CLASS_SCORE=11,
	COUNT=12
}OP_TYPE;

typedef struct html_op
{
	OP_TYPE type;		
}HTML_OP;

/*login*/
USER_DATA* login_Check(USER_DATA *tag_data,SESSION_LIST *tag_session);

USER_DATA* user_Check(USER_DATA *tag_data,DB_LINKER *tag_db);

BOOL_ input_Check(CHAR_ *in_char);

/*convert*/
CHAR_ level_Convert(int level);

CHAR_* id_Create(CHAR_ *tag_string);

/*Html response*/
void response_Done();

void response_Fail();

void response_Login_success(USER_DATA *user_info);

void response_Browse(DB_LINKER *tag_db);

void response_Classst(DB_LINKER *tag_db);

void response_Addst(DB_LINKER *tag_db);

void response_Delst(DB_LINKER *tag_db);

void response_Addgrade(DB_LINKER *tag_db);

void response_Addyear(DB_LINKER *tag_db);

void response_Addclass(DB_LINKER *tag_db);

void response_Classscore(DB_LINKER *tag_db);

HTML_OP* op_Get(HTML_OP *tag_op,CHAR_ *get_data);

#endif
