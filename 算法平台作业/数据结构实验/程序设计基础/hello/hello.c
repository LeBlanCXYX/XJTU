#include<stdio.h>
void main()
{
	char a[9];
	for (int i = 0;i <9;i++)
		scanf("%c", a[i]);
	for (int j = 8;j >= 0;j=j-2)
		printf("%c", a[j]);
	return;
}