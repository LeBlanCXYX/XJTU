#include <stdio.h>
#include <stdlib.h>
#define length 8

int count = 0;  // 计数 
int ms[length][length] = { 0 };

// 判断将皇后放在(a, b)位置上是否合法 
int isTrue(int a, int b)
{
    int t;
    // 向上判断 
    for (t = a - 1; t >= 0; t--)
    {
        if (ms[t][b] == 1)
            return 0;
    }

    // 向左判断左
    for (t = b - 1; t >= 0; t--) {
        if (ms[a][t] == 1)
            return 0;
    }

    int m, n;
    // 向左上判断 
    for (m = a - 1, n = b - 1; m >= 0 && n >= 0; m--, n--)
    {
        if (ms[m][n] == 1)
            return 0;
    }
    // 向右上判 
    for (m = a - 1, n = b + 1; m >= 0 && n < length; m--, n++)
    {
        if (ms[m][n] == 1)
            return 0;
    }
    return 1;
}

// 打印八皇后 
void print() {
    count++;
    printf("%d:\n", count);
    int a, b;
    for (a = 0; a < length; a++) // 行 
    {
        for (b = 0; b < length; b++) // 列 
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

// 递归调用 m表示层数 
void fun(int m) {
    if (m == length)
    {
        print();    // 输出 
        return;     // 递归出口
    }
    int i;
    for (i = 0; i < length; i++)
    {
        if (isTrue(m, i))    // 回溯算法 
        {
            ms[m][i] = 1;   // 10表示此位置有皇后
            fun(m + 1);       // 递归调用 
            ms[m][i] = 0;   // 0表示此位置无皇后 
        }
    }
}

int main(void)
{
    fun(0);
    return EXIT_SUCCESS;
}
