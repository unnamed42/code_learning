#include <stdio.h>
void catStr(char* str1,char* str2){
	for(;*str1!='\0';str1++)
		;
	for(;*str2!='\0';str2++)
		*str1++=*str2;
}

int main(){
	char str1[20]="Hello ",str2[10]=",world!";
	printf("Before catStr:\nstr1=%s\nstr2=%s\n",str1,str2);
	catStr(str1,str2);
	printf("Before catStr:\nstr1=%s\nstr2=%s\n",str1,str2);
	return 0;
}