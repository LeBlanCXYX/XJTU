#include<stdio.h>
int main()
{
	int T;//TΪ��������
	scanf("%d", &T);
	FILE* fp;
	fp = fopen("a.txt", "r");
	int i = 0;
	int array1[20], array2[20][2];//array2Ϊ��ά���飬0�����洢װ������������ţ�1λ����װ�����Ĵ�С
	while(fscanf(fp,"%d",&array1[i]) != EOF)
	{
		i++;
	}//iΪ�ܵı�������
	fclose(fp);
	int sum = 0;//sumΪ
	int t1= 0,t2=0;//TΪ�󱳰�������iΪС����������sumΪ�󱳰����Ѿ�װ��С�������ܴ�С��t1��ʾ������С�����������ţ�t2��ʾС�����ڱ������˳�����
	int flag = 1;//��flag������Ƿ�ﵽ����
	while (flag)
	{
		while (t1 < i)
		{
			if (sum + array1[t1] < T)
			{
				array2[t2][0] = t1;
				array2[t2][1] = array1[t1];
				sum += array1[t1];
				t1++;t2++;
			}
			else if (sum + array1[t1] == T)
			{
				for (int i = 0;i < t2;i++)
					printf("%d ", array2[i][1]);
				printf("%d", array1[t1]);
				t1++;
			}
			else
				t1++;
		}
		flag = t2-1;
		for (int j = t2 - 1;j > 0;j--)
			if (array2[j][0] == array2[j - 1][0] + 1)
				flag--; 
		t1 = array2[t2 - 1][0] + 1;
		array2[t2 - 1][0] = 0;array2[t2 - 1][1] = 0;
		t2--;
	}
}