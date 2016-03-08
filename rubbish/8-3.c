#include <stdio.h>
void upCopy(char * new,char * old){
	for(;'\0'!=*old;old++){
		if(*old<='Z'&&'A'<=*old){
			*new=*old;
			new++;
		}
	}	
}

int main(){
	char * p1,* p2="AbCdEFghi";
	upCopy(p1,p2);
	printf("%s,%s",p1,p2);	
	return 0;
}
