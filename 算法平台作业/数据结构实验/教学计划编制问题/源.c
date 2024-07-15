#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxClass 100		//课程总数不超过100
#define MaxSemester 12		//学期总数不超过12

//   邻接表表示
typedef struct ARCNODE EdgeNode;		//邻接表结点
struct ARCNODE {
	int AdjVex;		//邻接点域
	EdgeNode* Next;		//指向下一个邻边节点的指针域
};

typedef struct VNODE {		//顶点表结点
	char Date[3 + 1];		//课程编号        还要储存/0所以＋1
	int Credit;		//节点学分（每门课学分）
	EdgeNode* FirstEdge;		//指向邻接表第一个邻边节点的指针域
	int InDegree;		//课程入度
}VexNode;

typedef struct MESSAGE {		//每学期的学期信息
	int SemesterNum;		//学期数
	int MaxCredit;		//每学期学分上限
}Message;

typedef struct ALGRAPH {		//图
	VexNode* Vertics;		//邻接表域
	int VexNum;		//节点数
	int ArcNum;		//边数
	Message* ExtraInfo;		//学期与课程信息
}ALGraph;

int Locate(char* ch) {		//将C1C2C3……等变为1 2 3...
	return (2 == strlen(ch)) ? ch[1] - '1' : (ch[1] - '0') * 10 + ch[2] - '1';
}

void Creat_Graph1(ALGraph* G) {		//输入学期总数 学分上限 课程总数(顶点数量)
	G->ExtraInfo = (Message*)malloc(sizeof(Message));		//初始化指针
	printf("请输入：  学期总数  每学期学分上限  课程总数\n");
	scanf("%d%d%d", &G->ExtraInfo->SemesterNum, &G->ExtraInfo->MaxCredit, &G->VexNum);
	if (G->VexNum > MaxClass) {
		printf("超出最大课程总数%d,请更改数据\n", MaxClass);
		exit(1);
	}
	if (G->ExtraInfo->SemesterNum > MaxSemester) {
		printf("超出最大学期数%d,请更改数据\n", MaxSemester);
		exit(1);
	}
}

void Creat_Graph2(ALGraph* G) {		//从文件读取课程信息
	FILE* fp = fopen("D:\\123\\数据.txt", "r");
	if (NULL == fp) {
		printf("文件路径有误！！！");
		exit(1);
	}
	G->Vertics = (VexNode*)malloc(sizeof(VexNode) * G->VexNum);
	for (int i = 0; i < G->VexNum; i++)
		G->Vertics[i].FirstEdge = NULL;		//初始化
	for (int i = 0; i < G->VexNum; i++) {		//读取课程信息
		fscanf(fp, "%s%d", G->Vertics[i].Date, &G->Vertics[i].Credit);		//读取课程名称和学分
		while ('\n' != fgetc(fp)) {		//根据先修课程建立邻接表结点
			char str[4];
			int s;
			fscanf(fp, "%s", str);
			s = Locate(str);
			if (s < 0 || s > G->VexNum) {		//判断课程是否有错误
				printf("%s输入错误！\n", G->Vertics[i].Date);
				exit(1);
			}
			EdgeNode* p = (EdgeNode*)malloc(sizeof(EdgeNode));		//更新邻接表结点
			p->AdjVex = i;
			p->Next = G->Vertics[s].FirstEdge;
			G->Vertics[s].FirstEdge = p;
			G->ArcNum++;
		}
	}
	fclose(fp);
	for (int i = 0; i < G->VexNum; i++)		//更新入度
		G->Vertics[i].InDegree = 0;
	for (int i = 0; i < G->VexNum; i++) {
		for (EdgeNode* p = G->Vertics[i].FirstEdge; NULL != p; p = p->Next) {
			G->Vertics[p->AdjVex].InDegree++;
		}

	}
}
//InDegree又是入度又是栈的next
void Top_Sort(VexNode g[], int n, VexNode* temp)		//用有入度域的aov网进行拓扑排序，输出并存到数组temp中
{
	int i, j, k, top, m = 0;
	EdgeNode* p;
	top = -1;		//链栈初始化，-1为栈尾
	for (i = 0; i < n; i++)			//将入度为0的顶点链接成链栈
		if (g[i].InDegree == 0)
		{
			g[i].InDegree = top;
			top = i;
		}
	printf("aov拓扑排序结果为：");
	while (top != -1)		//当链栈非空时
	{
		j = top;		//将栈顶顶点记为j
		top = g[top].InDegree;		//栈顶指针指向弹出栈后下一个入度为0的顶点
		printf("%s ", g[j].Date);//输出顶点信息
		temp[m] = g[j];		//将顶点信息有序保存到数组
		m++;		//记录已输出的顶点个数
		p = g[j].FirstEdge;
		while (p != NULL)		//删除顶点j的所有出边
		{
			k = p->AdjVex;
			g[k].InDegree--;		//将顶点j的邻接边节点k入度减1
			if (g[k].InDegree == 0)		//若顶点k入度为零则入链栈
			{
				g[k].InDegree = top;
				top = k;
			}
			p = p->Next;		//查找下一个邻接边节点
		}
	}
	if (m < n)
		printf("AOV 网有回路！！！！！");
}

void Sort2(VexNode* t, Message* s, int VexNum)		//按课程尽可能集中在前几学期输出并保存教学计划
{
	FILE* fp = fopen("D:\\123\\结果.txt", "w");
	int c = 0;		//用于输出课程信息
	for (int i = 0; i < s->SemesterNum; i++)
	{
		int b = 0;		//累计每学期学分
		printf("\n第%d个学期的课程为：", i + 1);
		fprintf(fp, "\n第%d个学期的课程为：", i + 1);
		while (b + t[c].Credit <= s->MaxCredit)		//判断是否超过最大学分
		{
			if (c == VexNum)break;
			printf("%s  ", t[c].Date);		//输出课程
			fprintf(fp, "%s ", t[c].Date);
			b = b + t[c].Credit;		//学分累计
			c++;		//指向下一课程
		}
	}
}

void Sort1(VexNode* t, Message* s, int VexNum)		//按各学期负担均匀输出并保存教学计划
{
	FILE* fp = fopen("D:\\123\\结果.txt", "w");
	int c = 0;		//用于输出课程信息
	for (int i = 0; i < s->SemesterNum; i++)
	{
		int b = 0;		//累计每学期学分
		printf("\n第%d个学期的课程为：", i + 1);
		fprintf(fp, "\n第%d个学期的课程为：", i + 1);
		for (int j = 0; j < VexNum / s->SemesterNum; j++)
		{
			if (b + t[c].Credit <= s->MaxCredit)		//判断是否超过最大学分
			{
				if (c == VexNum)break;
				printf("%s  ", t[c].Date);		//输出课程
				fprintf(fp, "%s ", t[c].Date);
				b = b + t[c].Credit;		//学分累计
				c++;		//指向下一课程
			}
		}
		if (i < VexNum % s->SemesterNum)		//加入平均后多余的课程
		{
			if (c == VexNum)break;
			printf("%s  ", t[c].Date);		//输出课程
			fprintf(fp, "%s ", t[c].Date);
			b = b + t[c].Credit;		//学分累计
			c++;		//指向下一课程
		}
	}
}

int main() {
	ALGraph G;
	int i;
	Creat_Graph1(&G);		//输入学期总数 学分上限 课程总数(顶点数量)
	Creat_Graph2(&G);		//从文件读取课程信息
	VexNode s[99];		//用于拓扑排序结果的分类
	Top_Sort(G.Vertics, G.VexNum, s);		//非常奇妙的拓扑排序！！！！
	printf("\n请输入教学计划编制类型：\n1.各学期负担均匀\n2.课程尽可能集中在前几学期\n");
	scanf("%d", &i);
	(i == 1) ? Sort1(s, G.ExtraInfo, G.VexNum) : Sort2(s, G.ExtraInfo, G.VexNum);//按各负担均匀输出或集中在前几学期输出并保存教学计划
	return 0;
}
