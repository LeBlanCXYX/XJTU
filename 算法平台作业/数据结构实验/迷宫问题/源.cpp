#include <cstdio>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <queue>
using namespace std;

int m, n, a, b, flag = 0;//m行 n列 起点(a,b) 
struct point {
    int x;  //行坐标
    int y;  //列坐标
    string str;//记录路径
};
string graph[30];
int vis[30][50];//用于标记
int dx[4] = { 1,0,0,-1 };
int dy[4] = { 0,-1,1,0 };
string dir = "dlru";//要打印的方向

void BFS()
{
    queue<point>q;
    q.push({ 0,0,"" });
    vis[0][0] = 1;
    while (!q.empty())//队列不为空的情况下
    {
        point now = q.front();//第一个元素出队
        q.pop();//删除第一个元素释放空间
        if (now.x == m - 1 && now.y == n - 1)//如果是出口
        {
            flag = 1;
            for (int i = 0;i <= now.str.length();i++)
                switch (now.str[i]) {
                case 'd':
                    cout << "down ";
                    break;
                case 'l':
                    cout << "left ";
                    break;
                case 'r':
                    cout << "right ";
                    break;
                case 'u':
                    cout << "up ";
                    break;
                }
        }
        for (int i = 0;i < 4;i++)
        {
            int nx = now.x + dx[i];
            int ny = now.y + dy[i];
            if (nx < 0 || nx == m || ny == n || ny < 0 || vis[nx][ny] == 1 || graph[nx][ny] == '1')
                continue;//地图越界或者之前被访问过或着是障碍，重新选方向
            q.push({ nx,ny,now.str + dir[i] });//将新元素入队
            vis[nx][ny] = 1;
        }
    }
}

int main()
{
    freopen("D:\\in.txt", "r", stdin);//以E盘下的in.txt为输入文件 
    freopen("D:\\out.txt", "w", stdout);//以E盘下的out.txt为输出文件 
    cin >> m >> n;
    for (int i = 0;i < m;i++)
        cin >> graph[i];
    BFS();
    if (flag == 0)cout << "无通路";
    fclose(stdin);
    fclose(stdout);
    return 0;
}
