#include<stdio.h>
#include<string.h>
void main()
{
	char a[10];
	gets(a);
	for (int i = 8;i >= 0;i = i - 2)
		printf("%c", a[i]);
	return;
}