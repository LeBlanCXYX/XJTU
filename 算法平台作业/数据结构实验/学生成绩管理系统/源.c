#include <stdio.h>
#include <stdlib.h>
#define length 8

int count = 0;  // ���� 
int ms[length][length] = { 0 };

// �жϽ��ʺ����(a, b)λ�����Ƿ�Ϸ� 
int isTrue(int a, int b)
{
    int t;
    // �����ж� 
    for (t = a - 1; t >= 0; t--)
    {
        if (ms[t][b] == 1)
            return 0;
    }

    // �����ж���
    for (t = b - 1; t >= 0; t--) {
        if (ms[a][t] == 1)
            return 0;
    }

    int m, n;
    // �������ж� 
    for (m = a - 1, n = b - 1; m >= 0 && n >= 0; m--, n--)
    {
        if (ms[m][n] == 1)
            return 0;
    }
    // �������� 
    for (m = a - 1, n = b + 1; m >= 0 && n < length; m--, n++)
    {
        if (ms[m][n] == 1)
            return 0;
    }
    return 1;
}

// ��ӡ�˻ʺ� 
void print() {
    count++;
    printf("%d:\n", count);
    int a, b;
    for (a = 0; a < length; a++) // �� 
    {
        for (b = 0; b < length; b++) // �� 
        {
            if (ms[a][b] == 0)
            {
                printf("1 ");
            }
            else if (ms[a][b] == 1)
            {
                printf("0 ");
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

// �ݹ���� m��ʾ���� 
void fun(int m) {
    if (m == length)
    {
        print();    // ��� 
        return;     // �ݹ����
    }
    int i;
    for (i = 0; i < length; i++)
    {
        if (isTrue(m, i))    // �����㷨 
        {
            ms[m][i] = 1;   // 10��ʾ��λ���лʺ�
            fun(m + 1);       // �ݹ���� 
            ms[m][i] = 0;   // 0��ʾ��λ���޻ʺ� 
        }
    }
}

int main(void)
{
    fun(0);
    return EXIT_SUCCESS;
}
