#include "c_string.h"

C_ARRAY string_Create(CHAR_ const *tag_string)
{
	C_ARRAY res_string;
	
	res_string=array_Create(sizeof(CHAR_));
	res_string=string_Set(res_string,tag_string);
	
	return res_string;
};

KMP_KEY* string_Kprepare(KMP_KEY *tag_key,CHAR_ const *key_string)
{
	size_t i;
	size_t j;
	size_t *prefix_array;
	
	tag_key->key_string=NULL;
	tag_key->prefix_array=NULL;
	tag_key->key_length=STRLEN_(key_string);
	tag_key->key_string=malloc(sizeof(CHAR_)*(tag_key->key_length+1));
	if(tag_key->key_string==NULL)goto fail_return;
	memcpy(tag_key->key_string,key_string,(tag_key->key_length+1)*sizeof(CHAR_));

	tag_key->prefix_array=(size_t*)malloc(sizeof(size_t)*(tag_key->key_length));
	if(tag_key->prefix_array==NULL)goto fail_return;

	for(i=0;i<(tag_key->key_length);++i)tag_key->prefix_array[i]=0;
	i=0;
	for(j=1;j<(tag_key->key_length);++j)
	{
		for(;i>0&&key_string[i]!=key_string[j];)i=tag_key->prefix_array[i];
		if(key_string[i]==key_string[j])i++;
		tag_key->prefix_array[j]=i;
	};

	return tag_key;

	fail_return:
	if(tag_key->key_string!=NULL)free(tag_key->key_string);
	if(tag_key->prefix_array!=NULL)free(tag_key->prefix_array);
	return NULL;
};

CHAR_* string_Knsearch(CHAR_ const *tag_string,KMP_KEY const *key,size_t n)
{
	size_t i;
	size_t j;
	size_t k;

	for(i=k=j=0;(j<(STRLEN_(tag_string)))&&(k<n);++j,++k)
	{
		while((i>0)&&((key->key_string)[i]!=tag_string[j]))i=(key->prefix_array)[i];
		if((key->key_string)[i]==tag_string[j])++i;
		
		if(i>=(key->key_length))return (CHAR_* const)&(tag_string[j+1-key->key_length]);
	};
	return NULL;
};

void string_Kfree(KMP_KEY *key)
{
	free(key->key_string);
	free(key->prefix_array);
};

C_ARRAY string_Set(C_ARRAY tag_array,CHAR_ const *tag_string)
{
	CHAR_ const *op;

	tag_array=array_Resize(tag_array,0);
	for(op=tag_string;*op!=ENCODE_('\0');++op)tag_array=array_Append(tag_array,op);
	
	tag_array=array_Append(tag_array,op);
	
	return tag_array;
};

C_ARRAY string_Sprintf(C_ARRAY format,CHAR_ *set_string,...)
{
	C_STRING tag_string;
	
	return NULL;
};

char* string_Widetoansi(char **tag_string,wchar_t const *wstring)
{
	wchar_t const *s_wchar;
	char char_cache[WORD_SIZE_];
	char *op;
	size_t to_end;
	
	*tag_string=array_Create(sizeof(char));
	for(s_wchar=wstring;*s_wchar!=L'\0';++s_wchar)
	{
		to_end=wcrtomb(char_cache,*s_wchar,NULL);
		char_cache[to_end]='\0';
		for(op=char_cache;*op!='\0';++op)*tag_string=array_Append(*tag_string,op);
	};
	*op='\0';
	*tag_string=array_Append(*tag_string,op);
	return *tag_string;
};

wchar_t* string_Ansitowide(wchar_t **tag_wstring,char const *string)
{
	char const *s_char;
	wchar_t s_wchar=L'\0';
	
	*tag_wstring=array_Create(sizeof(wchar_t));
	for(s_char=string;*s_char!='\0';s_char+=mbrtowc(&s_wchar,s_char,WORD_SIZE_,NULL))*tag_wstring=(s_wchar==L'\0'?*tag_wstring:array_Append(*tag_wstring,&s_wchar));
	*tag_wstring=array_Append(*tag_wstring,&s_wchar);
	s_wchar=L'\0';
	*tag_wstring=array_Append(*tag_wstring,&s_wchar);
	
	return *tag_wstring;
};