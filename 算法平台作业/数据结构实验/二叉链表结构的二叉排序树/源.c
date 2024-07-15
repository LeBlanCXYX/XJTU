#include<stdio.h>
#include<stdlib.h>
typedef struct Node
{
	int data;
	struct Node* lchild;
	struct Node* rchild;
}node,*NODE;
int array[50];
void insertAVL(NODE* root, int a)
{
	if (root == NULL)
	{
		root = (node*)malloc(sizeof(node));
		(*root)->lchild = NULL;
		(*root)->rchild = NULL;
		(*root)->data = a;
	}
	else if (a < (*root)->data)
		insertAVL(&(*root)->lchild, a);
	else if (a > (*root)->data)
		insertAVL(&(*root)->rchild, a);
	else if (a == (*root)->data)
		printf("二叉树中已有%d", a);
}
void midorder(NODE* temp)
{
	if (!temp)return;
	else {
		midorder(&(*temp)->lchild);
		printf("%d ", (*temp)->data);
		midorder(&(*temp)->rchild);
	}
}
int main()
{
	int t = 0;
	while (scanf("%d", &array[t])&&getchar()!='\n')t++;
	NODE* root = NULL;
	int i = 0;
	while (i < t) { insertAVL(root, array[i]);i++; }
	midorder(root);
}