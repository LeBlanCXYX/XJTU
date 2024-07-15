#include<iostream>
using namespace std;
int judge[20][20];
int arr[1000][20];
int bestcolumn = 0;                 // record the best ans
int a[20], b[20];                   // record the chosen column of A or B
int ansA[20], ansB[20];
void dfs(int column, int nowcoA, int nowcoB)
{
    if (20 - column + nowcoB + nowcoA < bestcolumn)
        return;                     // last column is less than bestcolumn
    if (column == 20 && nowcoA + nowcoB >= bestcolumn && nowcoB != 0 && nowcoA != 0)
    {
        int i;
        int a1 = 0, a2 = 0, b1 = 0, b2 = 0;
        for (i = 0;i < 20;i++)
        {
            if (ansA[i] == 1)a1++;
            if (a[i] == 1)a2++;
            if (ansB[i] == 1)b1++;
            if (b[i] == 1)b2++;
        }
        if (nowcoA + nowcoB > bestcolumn || (nowcoA + nowcoB == bestcolumn && a2 > b2 && a1 - b1 >= a2 - b2))
        {

            int j, ok = 1;
            for (i = 0; i < 1000; i++)
            {
                int A = 0; int B = 0;
                for (j = 0; j < 20; j++)
                {
                    if (a[j] == 1 && arr[i][j] == 1)
                        A = 1;
                    if (b[j] == 1 && arr[i][j] == 1)
                        B = 1;
                }
                //cout<<A<<"   "<<B<<endl;
                if (A == 1 && B == 1)        // only when A = B = 1, break
                {
                    ok = 0; break;
                }
            }
            if (ok)
            {
                bestcolumn = nowcoB + nowcoA;
                for (int m = 0; m < 20; m++)
                {
                    ansA[m] = a[m];
                    ansB[m] = b[m];
                }
            }
            return;
        }
    }
    if (column == 20) return;
    int okB = 1, okA = 1;
    if (nowcoB != 0)
    {
        for (int i = 0; i < 20; i++)
        {
            if (b[i] == 1 && judge[column][i] == 0)
            {
                okA = 0; break;
            }
        }
    }
    if (okA)
    {
        a[column] = 1;                  // only add this column to A
        dfs(column + 1, nowcoA + 1, nowcoB);
        a[column] = 0;
    }

    if (nowcoA != 0)
    {
        for (int i = 0; i < 20; i++)
        {
            if (a[i] == 1 && judge[column][i] == 0)
            {
                okB = 0; break;
            }
        }
    }
    if (okB)
    {
        b[column] = 1;                  // only add this column to B
        dfs(column + 1, nowcoA, nowcoB + 1);
        b[column] = 0;
    }
    dfs(column + 1, nowcoA, nowcoB);    // neither
}

int main()
{
    int i, j, k;
    for (i = 0; i < 1000; i++)
        for (j = 0; j < 20; j++)
            cin >> arr[i][j];
    for (i = 0; i < 20; i++)
        for (j = i + 1; j < 20; j++)
        {
            int ok = 0;
            for (k = 0; k < 1000; k++)
            {
                if (arr[k][i] == 1 && arr[k][j] == 1)
                {
                    ok = 1; break;
                }
            }
            if (!ok)
            {
                judge[i][j] = 1;
                judge[j][i] = 1;
            }
        }
    dfs(0, 0, 0);
    for (i = 0; i < 20; i++)
    {
        if (ansA[i]) cout << i << " ";
    }
    cout << endl;
    for (i = 0; i < 20; i++)
    {
        if (ansB[i]) cout << i << ' ';
    }
    int flag = 1;
    for (i = 0;i < 1000;i++)
    {
        if (arr[i][10] == 1)
        {
            flag = 0;
        }
    }
    if (flag == 0)cout << '*';
    return 0;
}
