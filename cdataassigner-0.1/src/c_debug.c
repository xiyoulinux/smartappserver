#include "c_debug.h"

BOOL_ debug_Output(char *message)
{
	FILE *fp;
	
	fp=fopen("/tmp/debug.out","a");
	if(fp==NULL)return FALSE_;
	
	if(fputs(message,fp))return FALSE_;
	
	fclose(fp);
	
	return TRUE_;
};
