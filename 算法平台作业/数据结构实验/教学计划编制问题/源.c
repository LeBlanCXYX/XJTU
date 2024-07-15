#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxClass 100		//�γ�����������100
#define MaxSemester 12		//ѧ������������12

//   �ڽӱ��ʾ
typedef struct ARCNODE EdgeNode;		//�ڽӱ���
struct ARCNODE {
	int AdjVex;		//�ڽӵ���
	EdgeNode* Next;		//ָ����һ���ڱ߽ڵ��ָ����
};

typedef struct VNODE {		//�������
	char Date[3 + 1];		//�γ̱��        ��Ҫ����/0���ԣ�1
	int Credit;		//�ڵ�ѧ�֣�ÿ�ſ�ѧ�֣�
	EdgeNode* FirstEdge;		//ָ���ڽӱ��һ���ڱ߽ڵ��ָ����
	int InDegree;		//�γ����
}VexNode;

typedef struct MESSAGE {		//ÿѧ�ڵ�ѧ����Ϣ
	int SemesterNum;		//ѧ����
	int MaxCredit;		//ÿѧ��ѧ������
}Message;

typedef struct ALGRAPH {		//ͼ
	VexNode* Vertics;		//�ڽӱ���
	int VexNum;		//�ڵ���
	int ArcNum;		//����
	Message* ExtraInfo;		//ѧ����γ���Ϣ
}ALGraph;

int Locate(char* ch) {		//��C1C2C3�����ȱ�Ϊ1 2 3...
	return (2 == strlen(ch)) ? ch[1] - '1' : (ch[1] - '0') * 10 + ch[2] - '1';
}

void Creat_Graph1(ALGraph* G) {		//����ѧ������ ѧ������ �γ�����(��������)
	G->ExtraInfo = (Message*)malloc(sizeof(Message));		//��ʼ��ָ��
	printf("�����룺  ѧ������  ÿѧ��ѧ������  �γ�����\n");
	scanf("%d%d%d", &G->ExtraInfo->SemesterNum, &G->ExtraInfo->MaxCredit, &G->VexNum);
	if (G->VexNum > MaxClass) {
		printf("�������γ�����%d,���������\n", MaxClass);
		exit(1);
	}
	if (G->ExtraInfo->SemesterNum > MaxSemester) {
		printf("�������ѧ����%d,���������\n", MaxSemester);
		exit(1);
	}
}

void Creat_Graph2(ALGraph* G) {		//���ļ���ȡ�γ���Ϣ
	FILE* fp = fopen("D:\\123\\����.txt", "r");
	if (NULL == fp) {
		printf("�ļ�·�����󣡣���");
		exit(1);
	}
	G->Vertics = (VexNode*)malloc(sizeof(VexNode) * G->VexNum);
	for (int i = 0; i < G->VexNum; i++)
		G->Vertics[i].FirstEdge = NULL;		//��ʼ��
	for (int i = 0; i < G->VexNum; i++) {		//��ȡ�γ���Ϣ
		fscanf(fp, "%s%d", G->Vertics[i].Date, &G->Vertics[i].Credit);		//��ȡ�γ����ƺ�ѧ��
		while ('\n' != fgetc(fp)) {		//�������޿γ̽����ڽӱ���
			char str[4];
			int s;
			fscanf(fp, "%s", str);
			s = Locate(str);
			if (s < 0 || s > G->VexNum) {		//�жϿγ��Ƿ��д���
				printf("%s�������\n", G->Vertics[i].Date);
				exit(1);
			}
			EdgeNode* p = (EdgeNode*)malloc(sizeof(EdgeNode));		//�����ڽӱ���
			p->AdjVex = i;
			p->Next = G->Vertics[s].FirstEdge;
			G->Vertics[s].FirstEdge = p;
			G->ArcNum++;
		}
	}
	fclose(fp);
	for (int i = 0; i < G->VexNum; i++)		//�������
		G->Vertics[i].InDegree = 0;
	for (int i = 0; i < G->VexNum; i++) {
		for (EdgeNode* p = G->Vertics[i].FirstEdge; NULL != p; p = p->Next) {
			G->Vertics[p->AdjVex].InDegree++;
		}

	}
}
//InDegree�����������ջ��next
void Top_Sort(VexNode g[], int n, VexNode* temp)		//����������aov��������������������浽����temp��
{
	int i, j, k, top, m = 0;
	EdgeNode* p;
	top = -1;		//��ջ��ʼ����-1Ϊջβ
	for (i = 0; i < n; i++)			//�����Ϊ0�Ķ������ӳ���ջ
		if (g[i].InDegree == 0)
		{
			g[i].InDegree = top;
			top = i;
		}
	printf("aov����������Ϊ��");
	while (top != -1)		//����ջ�ǿ�ʱ
	{
		j = top;		//��ջ�������Ϊj
		top = g[top].InDegree;		//ջ��ָ��ָ�򵯳�ջ����һ�����Ϊ0�Ķ���
		printf("%s ", g[j].Date);//���������Ϣ
		temp[m] = g[j];		//��������Ϣ���򱣴浽����
		m++;		//��¼������Ķ������
		p = g[j].FirstEdge;
		while (p != NULL)		//ɾ������j�����г���
		{
			k = p->AdjVex;
			g[k].InDegree--;		//������j���ڽӱ߽ڵ�k��ȼ�1
			if (g[k].InDegree == 0)		//������k���Ϊ��������ջ
			{
				g[k].InDegree = top;
				top = k;
			}
			p = p->Next;		//������һ���ڽӱ߽ڵ�
		}
	}
	if (m < n)
		printf("AOV ���л�·����������");
}

void Sort2(VexNode* t, Message* s, int VexNum)		//���γ̾����ܼ�����ǰ��ѧ������������ѧ�ƻ�
{
	FILE* fp = fopen("D:\\123\\���.txt", "w");
	int c = 0;		//��������γ���Ϣ
	for (int i = 0; i < s->SemesterNum; i++)
	{
		int b = 0;		//�ۼ�ÿѧ��ѧ��
		printf("\n��%d��ѧ�ڵĿγ�Ϊ��", i + 1);
		fprintf(fp, "\n��%d��ѧ�ڵĿγ�Ϊ��", i + 1);
		while (b + t[c].Credit <= s->MaxCredit)		//�ж��Ƿ񳬹����ѧ��
		{
			if (c == VexNum)break;
			printf("%s  ", t[c].Date);		//����γ�
			fprintf(fp, "%s ", t[c].Date);
			b = b + t[c].Credit;		//ѧ���ۼ�
			c++;		//ָ����һ�γ�
		}
	}
}

void Sort1(VexNode* t, Message* s, int VexNum)		//����ѧ�ڸ�����������������ѧ�ƻ�
{
	FILE* fp = fopen("D:\\123\\���.txt", "w");
	int c = 0;		//��������γ���Ϣ
	for (int i = 0; i < s->SemesterNum; i++)
	{
		int b = 0;		//�ۼ�ÿѧ��ѧ��
		printf("\n��%d��ѧ�ڵĿγ�Ϊ��", i + 1);
		fprintf(fp, "\n��%d��ѧ�ڵĿγ�Ϊ��", i + 1);
		for (int j = 0; j < VexNum / s->SemesterNum; j++)
		{
			if (b + t[c].Credit <= s->MaxCredit)		//�ж��Ƿ񳬹����ѧ��
			{
				if (c == VexNum)break;
				printf("%s  ", t[c].Date);		//����γ�
				fprintf(fp, "%s ", t[c].Date);
				b = b + t[c].Credit;		//ѧ���ۼ�
				c++;		//ָ����һ�γ�
			}
		}
		if (i < VexNum % s->SemesterNum)		//����ƽ�������Ŀγ�
		{
			if (c == VexNum)break;
			printf("%s  ", t[c].Date);		//����γ�
			fprintf(fp, "%s ", t[c].Date);
			b = b + t[c].Credit;		//ѧ���ۼ�
			c++;		//ָ����һ�γ�
		}
	}
}

int main() {
	ALGraph G;
	int i;
	Creat_Graph1(&G);		//����ѧ������ ѧ������ �γ�����(��������)
	Creat_Graph2(&G);		//���ļ���ȡ�γ���Ϣ
	VexNode s[99];		//���������������ķ���
	Top_Sort(G.Vertics, G.VexNum, s);		//�ǳ�������������򣡣�����
	printf("\n�������ѧ�ƻ��������ͣ�\n1.��ѧ�ڸ�������\n2.�γ̾����ܼ�����ǰ��ѧ��\n");
	scanf("%d", &i);
	(i == 1) ? Sort1(s, G.ExtraInfo, G.VexNum) : Sort2(s, G.ExtraInfo, G.VexNum);//���������������������ǰ��ѧ������������ѧ�ƻ�
	return 0;
}
