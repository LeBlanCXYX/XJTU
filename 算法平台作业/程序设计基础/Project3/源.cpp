#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
void MinCost(int L, int n, int* p);
int main()
{
	int L, n, i;
	cin >> L >> n;
	int* p = new int[n + 2];
	p[0] = 0;
	p[n + 1] = L;
	for (i = 1;i < n + 1;i++)
		cin >> p[i];
	MinCost(L, n, p);
	return 0;
}
void MinCost(int L, int n, int* p)
{
	int i;
	int j;
	int step;
	int k;
	int t = 2, m = 0;
	for (i = 1;i <= n;i++)
		for (j = 1;j <= n;j++)
		{
			if (p[j] > p[j + 1])
			{
				int m = p[j];
				p[j] = p[j + 1];
				p[j + 1] = m;
			}
		}
	vector<vector<int> >cost(n + 2, vector<int>(n + 2,0));
	for (i = 1;i <= n+1;i++)
	{
		for (j = 0;j <= n;j++)
		{
			cost[i][j] = 0;
		}
	}
	for (i = 1;i <= n;i++)
	{
		cost[i][i] = p[i + 1] - p[i - 1];
	}
	for (step = 1;step < n;step++)
	{
		for (j=step+1;j<= n;j++)
		{
			i=j-step;
			m = cost[i + 1][j];
			for (k = i;k <= j;k++)
			{
				if (cost[i][k - 1] + cost[k + 1][j] < m)
				{
					m = cost[i][k - 1] + cost[k + 1][j];
				}
				cost[i][j] = p[j + 1] - p[i - 1] + m;
			}
		}
	}
	cout << cost[1][n];
	return;
}