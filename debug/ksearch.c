#include <stdio.h>
#include <cda/c_string.h>

int main()
{
	CHAR_ *string1;
	CHAR_ *string2;
	KMP_KEY search_key;

	SET_LOCALE_("");
	string1=string_Create(ENCODE_("莫楠万岁万岁万万岁"));
	string2=string_Create(ENCODE_("万岁"));

	string_Kprepare(&search_key,string2);
	PRINTF_(ENCODE_("%S\n"),string_Knsearch(string1,&search_key,4));
	PRINTF_(ENCODE_("%S\n"),search_key.key_string);
	PRINTF_(ENCODE_("length:%u,%u,%u\n"),search_key.key_length,search_key.prefix_array[0],search_key.prefix_array[1]);

	string_Kfree(&search_key);
	string_Drop(string1);
	string_Drop(string2);

	return 0;
};