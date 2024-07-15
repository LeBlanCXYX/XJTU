#include<stdio.h>
#include<stdlib.h>
int main(){
	int n;
	int i, j, a, place, flag = 1, q;
	scanf("%d", &n);
	int *num=(int*)malloc((n + 1)*sizeof(int));
	for (i = 0;i < n;i++)
		scanf("%d", &num[i]);
	getchar();
	char b;
	scanf("%c", &b);
	printf("%c", b);
	if (b == 'I')
	{
		printf(" sadasd");
	}
	return 0;
}