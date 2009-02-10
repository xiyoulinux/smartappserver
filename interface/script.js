var RESPONDER_NAME_="/cgi-bin/"
var POST_HEAD_="application/x-www-form-urlencoded";

var WELCOME_A="<h2>欢迎来访</h2>"

var WELCOME_B="<h3>请选择功能</h3>"

var LOGIN_FORM_="<div id=\"login\">请登录<p><form name=\"login_form\">帐号<input type=\"text\" name=\"user\" size=\"12\"><p>密码<input type=\"password\" name=\"password\" size=\"12\"><p><input type=\"button\" value=\"登录\" onclick=\"form_Login()\"></form></div>";

var LOGINED_FORM="<div id=\"login\">欢迎登录！<br><input type=\"button\" name=\"logout\" value=\"退出登录\" onclick=\"form_Logout()\"></div>";

var ST_LIST_FORM_="请选择查看的范围：<p><form name=\"class_select_form\"><p>年度：<select name=\"year_list\" onchange=\"on_year_change()\"></select><input type=\"button\" name=\"new_grade\" value=\"开始新学年\" onclick=\"show_new_year()\"><p>学期：<select name=\"grade_list\" onchange=\"on_grade_change()\"></select><input type=\"button\" name=\"new_grade\" value=\"开始新学期\" onclick=\"show_new_grade()\"><p>班级：<select name=\"class_list\"><option value=\"0\">1</option></select><input type=\"button\" name=\"new_class\" value=\"新班级\" onclick=\"show_new_class()\">  <p><input type=\"button\" name=\"class_query\" value=\"查询\" onclick=\"show_grade_Query()\"/><input type=\"button\" name=\"class_add\" value=\"增加\" onclick=\"show_add_Query()\"/><input type=\"button\" name=\"class_remove\" value=\"删除\" onclick=\"show_remove_Query()\"/></form>";

var ST_OP_FORM_="<p><input type=\"button\" name=\"new_grade\" value=\"开始新学期\" onclick=\"show_new_grade()\"><p><input type=\"button\" name=\"new_class\" value=\"新班级\" onclick=\"show_new_class()\">";

var NEW_YEAR_FORM_="<p><input type=\"checkbox\" name=\"checked\" checked disabled/>延续上学年的数据<input type=\"button\" name=\"new_year_button\" value=\"确定\" onclick=\"on_new_year_click()\"/><input type=\"button\" name=\"cancel_button\" value=\"取消\" onclick=\"on_edit_cancel_click()\"/>";

var NEW_GRADE_FORM_="<p><input type=\"checkbox\" name=\"checked\" checked disabled/>延续上学期的数据<input type=\"button\" name=\"new_grade_button\" value=\"确定\" onclick=\"on_new_grade_click()\"/><input type=\"button\" name=\"cancel_button\" value=\"取消\" onclick=\"on_edit_cancel_click()\"/>";

var NEW_CLASS_FORM_="<form name=\"add_class\">序号：<input type=\"text\" name=\"class_num\" onkeyup=\"on_num_change()\"/><input type=\"button\" name=\"ok\" value=\"确定\"onclick=\"on_add_class_click()\"/>	<input type=\"button\" name=\"cancel\" value=\"取消\" onclick=\"on_edit_cancel_click()\"/></form>";

var ADD_ST_FORM_="<form name=\"add_st\">学号：<input type=\"text\" name=\"st_no\" onkeyup=\"on_num_change()\"/>姓名：<input type=\"text\" name=\"st_name\"/><input type=\"button\" name=\"ok\" value=\"确定\"onclick=\"on_add_ok_click()\"/>	<input type=\"button\" name=\"cancel\" value=\"取消\" onclick=\"on_edit_cancel_click()\"/></form>";


var SCORE_LIST_FORM_="请选择查看的范围：<p><form name=\"class_select_form\"><p>年度：<select name=\"year_list\" onchange=\"on_year_change()\"></select><p>学期：<select name=\"grade_list\" onchange=\"on_grade_change()\"></select><p>班级：<select name=\"class_list\"><option value=\"0\">1</option></select> <p><input type=\"button\" name=\"score_query\" value=\"查询\" onclick=\"show_score_Query()\"/><input type=\"button\" name=\"score_filter\" value=\"过滤器\" onclick=\"show_score_Filter()\"/><input type=\"button\" name=\"score_count\" value=\"统计\" onclick=\"show_score_Count()\"/></form>";

var logined;
var form_class_list;

var current_table;
var current_filter;
var filter_table;
var current_page;

/*var grade_name;*/
var current_class;

/*basic struct*/
function year_info()
{
	var year_grades;
	var year_age;
};

function grade_info()
{
	var grade_level;
	var grade_class;
};

function class_info()
{
	var class_num;
	var class_no;
};

function table_type()
{
	var table_type;
	var table_columns;
	var table_data;
};

function filter_type()
{
	var filter;
};

/*basic function*/
function xmlhttp_Create()
{
	var xmlhttp;
	try
	{
		xmlhttp=new ActiveXObject("Msxml2.XMLHTTP");
	}
	catch(e)
	{
		try
		{
			xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
		}
		catch(e)
		{
			xmlhttp=new XMLHttpRequest();
		};
	};
	return xmlhttp;
};

function login_Check()
{
	var xmlhttp;
	
	xmlhttp=xmlhttp_Create();
	xmlhttp.open("get",RESPONDER_NAME_+"?op=check",false);
	xmlhttp.send(null);
	
	return get_Value("result",xmlhttp.responseText)=="1";
};

function user_Login(user_id,user_password)
{
	var xmlhttp;
	
	xmlhttp=xmlhttp_Create();
	xmlhttp.open("post",RESPONDER_NAME_+"?op=login",false);
	xmlhttp.setRequestHeader("Content-Type",POST_HEAD_);
	xmlhttp.send("user="+user_id+"&"+"password="+user_password);
	
	if(xmlhttp.readyState==4)
	{
		if(xmlhttp.status==200)
		{
			if(get_Value("result",xmlhttp.responseText)=="1")return 1;	
		};
	};
	
	return 0;
};

function form_Login()
{
	var login_form;
	
	login_form=document.forms["login_form"];
	
	if(user_Login(login_form["user"].value,login_form["password"].value))
	{
		logined=1;
		show_Logined();
	}
	else
	{
		logined=0;
		show_Login();
	};
};

function form_Logout()
{
	var xmlhttp;
	
	xmlhttp=xmlhttp_Create();
	xmlhttp.open("get",RESPONDER_NAME_+"?op=logout",false);
	xmlhttp.send(null);
	
	logined=0;
	
	show_Login();
};

function get_Value(value_name,tag_string)
{
	return tag_string.slice(tag_string.indexOf("<"+value_name+">")+value_name.length+2,tag_string.indexOf("</"+value_name+">"));
};

function get_Name_byIndex(value_index,tag_string)
{
	var i;
	var child_string;
	var found_name;
	
	i=0;
	child_string=tag_string;
	while(i<=value_index)
	{
		if(child_string.length<1)
		{
			found_name="";
			break;
		};
		
		found_name=child_string.slice(child_string.indexOf("<")+1,child_string.indexOf(">"));
		child_string=child_string.substring(child_string.indexOf("</"+found_name+">")+found_name.length+3,child_string.length);
				
		++i;
	};
	return found_name;
};

function get_Value_byIndex(value_index,tag_string)
{
	var i;
	var child_string;
	var found_name;
	var found_value;
	
	i=0;
	child_string=tag_string;
	while(i<=value_index)
	{
		if(child_string.length<1)
		{
			found_value="";
			break;
		};
		
		found_name=child_string.slice(child_string.indexOf("<")+1,child_string.indexOf(">"));
		found_value=get_Value(found_name,child_string);
		child_string=child_string.substring(child_string.indexOf("</"+found_name+">")+found_name.length+3,child_string.length);
		
		++i;
	};
	return found_value;
};

function level_Convert(level)
{
	switch(level)
	{
		case 'a':return "高一上学期";break;
		case 'b':return "高一下学期";break;
		case 'c':return "高二上学期";break;
		case 'd':return "高二下学期";break;
		case 'e':return "高三上学期";break;
		case 'f':return "高三下学期";break;
	};
}

function load_Browse()
{
	form_class_list.length=0;
	if(logined==1)
	{
		xmlhttp=xmlhttp_Create();
		xmlhttp.open("get",RESPONDER_NAME_+"?op=browse",false);
		xmlhttp.send(null);
		
		if(xmlhttp.readyState==4)
		{
			if(xmlhttp.status==200)
			{
				if(get_Value("result",xmlhttp.responseText)=="1")
				{
					year_string="";
					grade_string="";
					
					i=1;
					total_list=new Array();
					
					while(get_Name_byIndex(i,xmlhttp.responseText).length>0)
					{
						year_string=get_Value_byIndex(i,xmlhttp.responseText);
						
						j=0;
						/*grade_list=new Array();*/
						grade_list=new year_info()
						grade_list.year_age=get_Name_byIndex(i,xmlhttp.responseText);
						grade_list.year_grades=new Array();
						
						while(get_Name_byIndex(j,year_string).length>0)
						{
							grade_string=get_Value_byIndex(j,year_string);
							
							z=0;
							class_list=new grade_info();
							class_list.grade_class=new Array();
							
							while(get_Name_byIndex(z,grade_string).length>0)
							{
								s_class=new class_info();
								s_class.class_num=get_Name_byIndex(z,grade_string);
								s_class.class_no=get_Value_byIndex(z,grade_string);
								
								class_list.grade_class.push(s_class);
								
								++z;
							};
							class_list.grade_level=get_Name_byIndex(j,year_string);
							
							grade_list.year_grades.push(class_list);
							/*grade_list.grade_level=get_Name_byIndex(j,year_string);*/
							
							++j;
						};
						
						
						total_list.push(grade_list);
						++i;
					};
				};
				
				form_class_list=total_list;								
			};
		};
	};
};

function load_Table()
{
	
};

function page_Init()
{
	var select;
	var entry;
	
	form_class_list=new Array();
	select=document.getElementById("select");
	entry=document.getElementById("entry");
	select.innerHTML=WELCOME_A;
	entry.innerHTML=WELCOME_B;
	
	if(login_Check())
	{
		show_Logined();
		/*logined*/
	}
	else
	{
		show_Login();
		/*no login*/
	};
			
};

/*show function*/
function show_Login()
{
	var sidebar=document.getElementById("sidebar");
	sidebar.innerHTML=LOGIN_FORM_;
	
	button=document.getElementById("browse");
	button.src="button/browse_disabled.jpg";
	button=document.getElementById("print");
	button.src="button/print_disabled.jpg";
	button=document.getElementById("import");
	button.src="button/import_disabled.jpg";
};

function show_Logined()
{
	var sidebar;
	var button;
	sidebar=document.getElementById("sidebar");
	sidebar.innerHTML=LOGINED_FORM;
	button=document.getElementById("browse");
	button.src="button/browse_enabled.jpg";
	button=document.getElementById("print");
	button.src="button/print_enabled.jpg";
	button=document.getElementById("import");
	button.src="button/import_enabled.jpg";
};

function show_Browse()
{
	var select_form;
	var select_html;
	
	load_Browse();
	
	select_html=document.getElementById("select");
	select_html.innerHTML=ST_LIST_FORM_;
	
	select_form=document.forms["class_select_form"];
	
	select_form["year_list"].length=0;
		
	for(i=0;i<form_class_list.length;++i)
	{
		select_form["year_list"].options[select_form["year_list"].length]=new Option(form_class_list[i].year_age+"级",i);
	};
	
	on_year_change();	
};

function show_Score()
{
	var select_form;
	var select_html;
	
	select_html=document.getElementById("select");
	select_html.innerHTML=SCORE_LIST_FORM_;
	
	select_form=document.forms["class_select_form"];
	
	select_form["year_list"].length=0;
		
	for(i=0;i<form_class_list.length;++i)
	{
		select_form["year_list"].options[select_form["year_list"].length]=new Option(form_class_list[i].year_age+"级",i);
	};
	
	on_year_change();
};

function show_add_Query()
{
	/*alert("you have select add");*/
	var edit_form=document.getElementById("edit");
	edit_form.innerHTML=ADD_ST_FORM_;
};

function show_remove_Query()
{
	var st_list;
	var del_list;
	var i;
	var xmlhttp;
	
	st_list=document.forms["st_select"];
	del_list=new Array();
	for(i=0;i<st_list.elements.length;++i)
	{
		if(st_list.elements[i].checked==true)
		{
			del_list.push(st_list.elements[i].name);
		};
	};
	
	if(del_list.length>0)
	{
		for(i=0;i<del_list.length;++i)
		{
			xmlhttp=xmlhttp_Create();
			xmlhttp.open("post",RESPONDER_NAME_+"?op=delst",false);
			xmlhttp.setRequestHeader("Content-Type", POST_HEAD_);
			xmlhttp.send("no="+del_list[i]);	
		
			if(xmlhttp.readyState==4)
			{
				if(xmlhttp.status==200)
				{
					if(get_Value("result",xmlhttp.responseText)=="1")
					{
						continue;
					}
					else
					{
						break;
					};
				};
			
			};
		};
		alert("已全部删除");
		show_grade_Query();
	};
	
};

function show_grade_Query()
{
	var xmlhttp;
	var select_form;
	var del_form;
	var class_no;
	var i;
	var line_array;
	var table_form;
	var table_;
	var table_line;
	var table_row;
	
	if(logined==1)
	{
		select_form=document.forms["class_select_form"];
		class_no=form_class_list[select_form["year_list"].value].year_grades[select_form["grade_list"].value].grade_class[select_form["class_list"].value].class_no;
		xmlhttp=xmlhttp_Create();
		xmlhttp.open("post",RESPONDER_NAME_+"?op=classst",false);
		xmlhttp.setRequestHeader("Content-Type", POST_HEAD_);
		xmlhttp.send(class_no);
		
		if(xmlhttp.readyState==4)
		{
			if(xmlhttp.status==200)
			{
				/*alert(xmlhttp.responseText);*/
				if(get_Value("result",xmlhttp.responseText)=="1")
				{
					/*get ready for the table*/
					table_html=document.getElementById("entry");
					table_html.innerHTML="";
					del_form=document.createElement("form");
					del_form.setAttribute("name","st_select");
					table_=document.createElement("table");
					table_.setAttribute("border","1");
					table_.setAttribute("borderColor","black");
					table_.setAttribute("Width","80%");
					
					table_line=table_.insertRow(0);
					(table_line.insertCell(0)).appendChild(document.createTextNode("选择"));
					(table_line.insertCell(1)).appendChild(document.createTextNode("学号"));
					(table_line.insertCell(2)).appendChild(document.createTextNode("姓名"));
					for(i=1;get_Name_byIndex(i,xmlhttp.responseText).length>0;++i)
					{
						line_array=get_Value_byIndex(i,xmlhttp.responseText).split(",");
						table_line=table_.insertRow(i);
						(table_line.insertCell(0)).innerHTML="<input type=\"checkbox\" name=\""+line_array[0]+"\" />";
						(table_line.insertCell(1)).appendChild(document.createTextNode(line_array[0]));
						(table_line.insertCell(2)).appendChild(document.createTextNode(line_array[1]));
					};
					del_form.appendChild(table_);
					table_html.appendChild(del_form);
										
					current_class=class_no;
				};
			};
		};
	}
	else
	{
		show_Login();
	};	
};

function show_score_Query()
{
	var xmlhttp;
	var select_form;
	var del_form;
	var class_no;
	var i;
	var line_array;
	var table_form;
	var table_;
	var table_line;
	var table_row;
	
	if(logined==1)
	{
		select_form=document.forms["class_select_form"];
		class_no=form_class_list[select_form["year_list"].value].year_grades[select_form["grade_list"].value].grade_class[select_form["class_list"].value].class_no;
		xmlhttp=xmlhttp_Create();
		xmlhttp.open("post",RESPONDER_NAME_+"?op=classscore",false);
		xmlhttp.setRequestHeader("Content-Type", POST_HEAD_);
		xmlhttp.send(class_no);
		
		if(xmlhttp.readyState==4)
		{
			if(xmlhttp.status==200)
			{
				
				if(get_Value("result",xmlhttp.responseText)=="1")
				{					
					table_html=document.getElementById("entry");
					table_html.innerHTML="";
					del_form=document.createElement("form");
					del_form.setAttribute("name","st_select");
					table_=document.createElement("table");
					table_.setAttribute("border","1");
					table_.setAttribute("borderColor","black");
					table_.setAttribute("Width","80%");
					
					table_line=table_.insertRow(0);
					(table_line.insertCell(0)).appendChild(document.createTextNode("年级"));
					(table_line.insertCell(1)).appendChild(document.createTextNode("班级"));
					(table_line.insertCell(2)).appendChild(document.createTextNode("姓名"));
					(table_line.insertCell(3)).appendChild(document.createTextNode("考试类型"));
					(table_line.insertCell(4)).appendChild(document.createTextNode("科目"));
					(table_line.insertCell(5)).appendChild(document.createTextNode("模块"));
					(table_line.insertCell(6)).appendChild(document.createTextNode("分数"));
					for(i=1;get_Name_byIndex(i,xmlhttp.responseText).length>0;++i)
					{
						line_array=get_Value_byIndex(i,xmlhttp.responseText).split(",");
						table_line=table_.insertRow(i);
						
						(table_line.insertCell(0)).appendChild(document.createTextNode(line_array[1]));
						(table_line.insertCell(1)).appendChild(document.createTextNode(line_array[3]));
						(table_line.insertCell(2)).appendChild(document.createTextNode(line_array[5]));
						(table_line.insertCell(3)).appendChild(document.createTextNode(line_array[6]));
						(table_line.insertCell(4)).appendChild(document.createTextNode(line_array[8]));
						(table_line.insertCell(5)).appendChild(document.createTextNode(line_array[9]));
						(table_line.insertCell(6)).appendChild(document.createTextNode(line_array[10]));
					};
					del_form.appendChild(table_);
					table_html.appendChild(del_form);
										
					current_class=class_no;
				};
			};
		};
	}
	else
	{
		show_Login();
	};
};

function show_new_year()
{
	var edit_form;
	
	edit_form=document.getElementById("edit");
	edit_form.innerHTML=NEW_YEAR_FORM_;
};

function show_new_grade()
{
	var edit_form;
	
	edit_form=document.getElementById("edit");
	edit_form.innerHTML=NEW_GRADE_FORM_;
};

function show_new_class()
{
	var edit_form;
	
	edit_form=document.getElementById("edit");
	edit_form.innerHTML=NEW_CLASS_FORM_;
};

/*on function*/
function on_year_change()
{	
	var select_form;
	var i;
	
	select_form=document.forms["class_select_form"];
	
	select_form["grade_list"].length=0;
	
	for(i=0;i<form_class_list[select_form["year_list"].value].year_grades.length;++i)
	{
		select_form["grade_list"].options[select_form["grade_list"].length]=new Option(level_Convert(form_class_list[select_form["year_list"].value].year_grades[i].grade_level),i);
	};
	
	on_grade_change();
};

function on_grade_change()
{
	var select_form;
	var i;
	
	select_form=document.forms["class_select_form"];
	
	select_form["class_list"].length=0;
	
	for(i=0;i<form_class_list[select_form["year_list"].value].year_grades[select_form["grade_list"].value].grade_class.length;++i)
	{
		select_form["class_list"].options[select_form["class_list"].length]=new Option(form_class_list[select_form["year_list"].value].year_grades[select_form["grade_list"].value].grade_class[i].class_num+"班",i);
	};		
};

function on_num_change()
{
	/*
	var add_form;
	
	add_form=document.forms["add_st"];
	if(add_form["st_no"].value="")return;
	/^([0-9]*)/.exec(add_form["st_no"]);
	if(add_form["st_no"].value!=RegExp.$1)add_form["st_no"].value=RegExp.$1;
	*/
};

function on_edit_cancel_click()
{
	var edit_form;
	
	edit_form=document.getElementById("edit");
	edit_form.innerHTML="";
};

function on_add_ok_click()
{
	var add_form;
	
	if(logined==1)
	{
		add_form=document.forms["add_st"];
		xmlhttp=xmlhttp_Create();
		xmlhttp.open("post",RESPONDER_NAME_+"?op=addst",false);
		xmlhttp.setRequestHeader("Content-Type", POST_HEAD_);
		xmlhttp.send("no="+add_form["st_no"].value+"&name="+add_form["st_name"].value+"&class="+current_class+";");
		
		if(xmlhttp.readyState==4)
		{
			if(xmlhttp.status==200)
			{
				if(get_Value("result",xmlhttp.responseText)=="1")
				{
					on_edit_cancel_click();
					show_grade_Query();
				}
				else
				{
					alert("操作失败");
				};				
			};
			
		};
		
	}
	else
	{
		show_Login();
	};
};

function on_new_year_click()
{
	xmlhttp=xmlhttp_Create();
	xmlhttp.open("post",RESPONDER_NAME_+"?op=addyear",false);
	xmlhttp.setRequestHeader("Content-Type", POST_HEAD_);
	xmlhttp.send("null");
	
	if(xmlhttp.readyState==4)
	{
		if(xmlhttp.status==200)
		{
			if(get_Value("result",xmlhttp.responseText)=="1")
			{
				on_edit_cancel_click();
				show_grade_Query();
			}
			else
			{
				alert("操作失败");
			};
		};
		
	};
};

function on_new_grade_click()
{
	xmlhttp=xmlhttp_Create();
	xmlhttp.open("post",RESPONDER_NAME_+"?op=addgrade",false);
	xmlhttp.setRequestHeader("Content-Type", POST_HEAD_);
	xmlhttp.send(current_class);
	
	if(xmlhttp.readyState==4)
	{
		if(xmlhttp.status==200)
		{
			if(get_Value("result",xmlhttp.responseText)=="1")
			{
				on_edit_cancel_click();
				show_grade_Query();
			}
			else
			{
				alert("操作失败");
			};				
		};
		
	};
};

function on_add_class_click()
{
	var add_form;
	
	add_form=document.forms["add_class"];
	xmlhttp=xmlhttp_Create();
	xmlhttp.open("post",RESPONDER_NAME_+"?op=addclass",false);
	xmlhttp.setRequestHeader("Content-Type", POST_HEAD_);
	xmlhttp.send("no="+current_class+"&num="+add_form["class_num"].value);
	
	if(xmlhttp.readyState==4)
	{
		if(xmlhttp.status==200)
		{
			if(get_Value("result",xmlhttp.responseText)=="1")
			{
				on_edit_cancel_click();
				show_grade_Query();
			}
			else
			{
				alert("操作失败");
			};				
		};
		
	};
};

function test()
{
	var select_form;
	select_form=document.forms["class_select_form"];
	
	alert(select_form["year_list"].value);
};
