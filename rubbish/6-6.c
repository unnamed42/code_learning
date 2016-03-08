#include <stdio.h>
int lenStr(char str[]){
	int i;
	for(i=0;'\0'!=str[i];i++)
		;
	return i;
}

void catStr(char str1[],char str2[]){
	int i,len1=lenStr(str1),len2=lenStr(str2);
	for(i=0;i<len2;i++)
		str1[len1+i]=str2[i];
}

void printStr(char str[]){
	int i;
	for(i=0;'\0'!=str[i];i++)
		printf("%c",str[i]);
}

int main(){
	char str1[20]="abcde",str2[10]="FGHIJdd";
	printf("length of str1 is %d\nlength of str2 is %d\n",lenStr(str1),lenStr(str2));
	printf("str1 is %s,str2 is %s",str1,str2);
	catStr(str1,str2);
	printf("\ncombined length is %d\nAfter combination:%s",lenStr(str1),str1);
	return 0;
}
