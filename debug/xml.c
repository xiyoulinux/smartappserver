#include <stdio.h>
#include <xmlo/xmlo.h>

int main()
{
	XML_DOC my_xml;
	XML_NODE my_node;
	size_t n;
	size_t m;
	FILE *file;
	
	SET_LOCALE_("");
	PRINTF_(ENCODE_("NOW START\n"));
	my_xml.root=(XML_NODE*)malloc(sizeof(XML_NODE));
	if(xml_Open(&my_xml,ENCODE_("testxml"),STORE_ALL)==NULL)printf("Open fail\n");
	PRINTF_(ENCODE_("the child have:%lu\n"),array_Head(my_xml.root->childs)->array_length);
	for(n=0;n<array_Head(my_xml.root->childs)->array_length;++n)
	{
		PRINTF_(ENCODE_("the number is:%lu,the name is:%S\n"),n,my_xml.root->childs[n].name);
		if(my_xml.root->childs[n].childs!=NULL)
		{
			for(m=0;m<array_Head(my_xml.root->childs[n].childs)->array_length;++m)
			{
				PRINTF_(ENCODE_("the number is:%lu,the name is:%S\n"),m,my_xml.root->childs[n].childs[m].name);
			};
		};
	};

	PRINTF_(ENCODE_("%S\n"),xml_Parmbyname(ENCODE_("Address"),&my_xml.root->childs[0].childs[0]));
	xml_Parmappend(&my_xml.root->childs[0].childs[0],ENCODE_("fuck"),ENCODE_("you"));

	file=fopen("xmloutput","w");
	PRINTF_(ENCODE_("\n"));
	xml_Save(file,&my_xml);
		
	return 0;
};