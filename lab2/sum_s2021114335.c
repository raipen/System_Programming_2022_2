#include <stdio.h>

int sum(int s,int f){
	int result = 0;
	for(int i=s;i<=f;i++)
		result +=i;
	return result;
}

int main(void){
	printf("The sum of 1 to 500 is %d\n",sum(1,500));
	return 0;
}
