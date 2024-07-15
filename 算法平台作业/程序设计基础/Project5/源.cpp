#include<queue>
#include<iostream>
#include<vector>
using namespace std;
typedef struct
{
	int id;
	vector<int>son;
	int parent;
	int ownprice;//ownprice为从父节点到该处需要的代价 
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
			int k, j, ds1;//j为新增的点的序号，ds为新增点与父节点之间的带权边长 
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
		//此处为构造函数，初始化构建树。
	}
	void solution()
	{
		vector<int>marked(n1, 0);//用来做标记节点是否被访问;
		vector<int>markedson(n1, 0);//儿子节点已经访问了多少个 
		vector<Node>S;//用来存放被删除的节点;
		int ee = 0;//用来计数访问过了已经访问过了多少个节点
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
	int n, d;               //n为顶点个数，d为路径长度
	cin >> n >> d;
	dTree dt(n, d);    //构建与初始化树
	dt.solution();      //通过solution函数输出结果
	return 0;
}