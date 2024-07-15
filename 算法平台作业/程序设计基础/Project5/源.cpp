#include<queue>
#include<iostream>
#include<vector>
using namespace std;
typedef struct
{
	int id;
	vector<int>son;
	int parent;
	int ownprice;//ownpriceΪ�Ӹ��ڵ㵽�ô���Ҫ�Ĵ��� 
	int sonprice;
}Node;
vector<Node>a;
class dTree {
	int n1;
	int d1;
public:
	dTree(int n, int d) {
		int i;
		Node xx;
		for (i = 0;i < n;i++)
		{
			a.push_back(xx);
			a[i].id = i;
			a[i].sonprice = 0;
		}
		a[0].ownprice = 0;
		n1 = n;
		d1 = d;
		queue<int>q;
		q.push(0);
		while (!q.empty())
		{
			int t = q.front();
			q.pop();
			int k, j, ds1;//jΪ�����ĵ����ţ�dsΪ�������븸�ڵ�֮��Ĵ�Ȩ�߳� 
			cin >> k;
			for (i = 0;i < k;i++)
			{
				cin >> j >> ds1;
				a[t].son.push_back(a[j].id);
				a[j].parent = a[t].id;
				a[j].ownprice = ds1;
				q.push(j);
			}
		}
		//�˴�Ϊ���캯������ʼ����������
	}
	void solution()
	{
		vector<int>marked(n1, 0);//��������ǽڵ��Ƿ񱻷���;
		vector<int>markedson(n1, 0);//���ӽڵ��Ѿ������˶��ٸ� 
		vector<Node>S;//������ű�ɾ���Ľڵ�;
		int ee = 0;//�����������ʹ����Ѿ����ʹ��˶��ٸ��ڵ�
		while (ee < n1)
		{
			int i, sum = 0;
			for (i = 0;i < n1;i++)
				if (a[i].son.size() == markedson[i] && marked[i] == 0)
				{
					if (a[i].sonprice < d1)
					{
						if (i != 0)
						{
							if (a[a[i].parent].sonprice < a[i].sonprice + a[i].ownprice)
								a[a[i].parent].sonprice = a[i].sonprice + a[i].ownprice;
						}
					}
					else
					{
						S.push_back(a[i]);
					}
					ee++;
					marked[i] = 1;
					if (i != 0)
						markedson[a[i].parent] += 1;
				}
		}
		cout << S.size();
	}
};
int main()
{
	int n, d;               //nΪ���������dΪ·������
	cin >> n >> d;
	dTree dt(n, d);    //�������ʼ����
	dt.solution();      //ͨ��solution����������
	return 0;
}