#include <cstdio>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <queue>
using namespace std;

int m, n, a, b, flag = 0;//m�� n�� ���(a,b) 
struct point {
    int x;  //������
    int y;  //������
    string str;//��¼·��
};
string graph[30];
int vis[30][50];//���ڱ��
int dx[4] = { 1,0,0,-1 };
int dy[4] = { 0,-1,1,0 };
string dir = "dlru";//Ҫ��ӡ�ķ���

void BFS()
{
    queue<point>q;
    q.push({ 0,0,"" });
    vis[0][0] = 1;
    while (!q.empty())//���в�Ϊ�յ������
    {
        point now = q.front();//��һ��Ԫ�س���
        q.pop();//ɾ����һ��Ԫ���ͷſռ�
        if (now.x == m - 1 && now.y == n - 1)//����ǳ���
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
                continue;//��ͼԽ�����֮ǰ�����ʹ��������ϰ�������ѡ����
            q.push({ nx,ny,now.str + dir[i] });//����Ԫ�����
            vis[nx][ny] = 1;
        }
    }
}

int main()
{
    freopen("D:\\in.txt", "r", stdin);//��E���µ�in.txtΪ�����ļ� 
    freopen("D:\\out.txt", "w", stdout);//��E���µ�out.txtΪ����ļ� 
    cin >> m >> n;
    for (int i = 0;i < m;i++)
        cin >> graph[i];
    BFS();
    if (flag == 0)cout << "��ͨ·";
    fclose(stdin);
    fclose(stdout);
    return 0;
}
