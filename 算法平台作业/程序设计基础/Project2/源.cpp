#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
void WeightMedian(int length, vector<int>num, vector<double>weight, int index)
{
	if (length == 1)
		cout << num[0];
	double sumsmall = 0;
	int k = index + (length - 1) / 2;
	int mid = num[k];
	int i, t = 0;
	for (i = index;i < index + length;i++)
	{
		int m;double n;
		if (num[i] < mid)
		{
			m = num[index + t];
			num[index + t] = num[i];
			num[i] = m;
			n = weight[index + t];
			weight[index + t] = weight[i];
			weight[i] = n;
			if (index + t == k)
				k = i;
			t++;
		}

	}
	int m = num[k];num[k] = num[index + t];num[index + t] = m;
	double n = weight[k];weight[k] = weight[index + t];weight[index + t] = n;
	k = index + t;
	mid = num[k];
	for (int i = index;i < k;i++)
		sumsmall += weight[i];
	if (sumsmall < 0.5 && (1 - weight[k] - sumsmall) < 0.5)
	{
		if (sumsmall <= 0.5)
		{
			cout << num[k];
			return;
		}
	}
	else if (sumsmall > 0.5)
	{
		weight[k] = 1 - sumsmall;
		WeightMedian(t + 1, num, weight, index);
	}
	else if (1 - sumsmall - weight[k] > 0.5)
	{
		weight[k] = sumsmall + weight[k];
		WeightMedian(length - t, num, weight, k);
	}
}
int main() {
    int length;
    cin >> length;
    vector<int>num;
    int k;
    vector<double>weight;
    double m;
    for (int i = 0; i < length; i++) {
        cin >> k;
        num.push_back(k);
    }
    for (int i = 0; i < length; i++) {
        cin >> m;
        weight.push_back(m);
    }
    WeightMedian(length, num, weight, 0);
    return 0;
}