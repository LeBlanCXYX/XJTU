#include<stdio.h>
int main()
{
	int T;//T为背包容量
	scanf("%d", &T);
	FILE* fp;
	fp = fopen("a.txt", "r");
	int i = 0;
	int array1[20], array2[20][2];//array2为二维数组，0用来存储装入包袱本身的序号，1位用来装背包的大小
	while(fscanf(fp,"%d",&array1[i]) != EOF)
	{
		i++;
	}//i为总的背包个数
	fclose(fp);
	int sum = 0;//sum为
	int t1= 0,t2=0;//T为大背包容量，i为小包袱个数，sum为大背包中已经装的小包袱的总大小，t1表示数到的小包袱本身的序号，t2表示小包袱在背包里的顺序序号
	int flag = 1;//用flag来检测是否达到最终
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