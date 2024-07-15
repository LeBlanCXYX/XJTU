#include<stdio.h>
#include<stdlib.h>
int ans[100];
typedef struct Node
{
	int data;
	int bf;
	int height;
	struct Node* lchild;
	struct Node* rchild;

}node, * NODE;
void updatebf(NODE* a);
void updatehe(NODE* a);
void Lotate(NODE* a)
{
	NODE b;
	b = (*a)->lchild;
	(*a)->lchild = b->rchild;
	b->rchild = *a;
	*a = b;
	updatehe(&(*a)->rchild);
	updatebf(&(*a)->rchild);
	updatehe(a);
	updatebf(a);
	return;
}
void Rotate(NODE* a)
{
	NODE b;
	b = (*a)->rchild;
	(*a)->rchild = b->lchild;
	b->lchild = *a;
	*a = b;
	updatehe(&(*a)->lchild);
	updatebf(&(*a)->lchild);
	updatehe(a);
	updatebf(a);
	return;
}
void insertAVL(NODE* temp, int x)
{
	if (!*temp)
	{
		*temp = (NODE)malloc(sizeof(node));
		(*temp)->data = x;
		(*temp)->lchild = (*temp)->rchild = NULL;
		(*temp)->height = (*temp)->bf = 0;
	}
	else if (x < (*temp)->data)
	{
		insertAVL(&(*temp)->lchild, x);
		updatehe(temp);
		updatebf(temp);
	}
	else if (x > (*temp)->data)
	{
		insertAVL(&(*temp)->rchild, x);
		updatehe(temp);
		updatebf(temp);
	}
	if ((*temp)->bf == 2 && (*temp)->lchild->bf == 1)Lotate(temp);
	else if ((*temp)->bf == -2 && (*temp)->rchild->bf == -1)Rotate(temp);
	else if ((*temp)->bf == 2 && (*temp)->lchild->bf == -1) { Rotate(&(*temp)->lchild);Lotate(temp); }
	else if ((*temp)->bf == -2 && (*temp)->rchild->bf == 1) { Lotate(&(*temp)->rchild);Rotate(temp); }
}
void find(NODE* a, int pos)
{
	if (!(*a))return;
	ans[pos] = (*a)->data;
	find(&(*a)->lchild, 2 * pos + 1);
	find(&(*a)->rchild, 2 * pos + 2);
}
void updatehe(NODE* a)
{
	if ((*a)->lchild == NULL && (*a)->rchild == NULL)(*a)->height = 0;
	else if ((*a)->lchild != NULL || (*a)->rchild == NULL)(*a)->height = (*a)->lchild->height + 1;
	else if ((*a)->rchild != NULL || (*a)->lchild == NULL)(*a)->height = (*a)->rchild->height + 1;
	else
	{
		if ((*a)->lchild->height > (*a)->rchild->height)
			(*a)->height = (*a)->lchild->height + 1;
		else
			(*a)->height = (*a)->rchild->height + 1;
	}
}
void updatebf(NODE* a)
{
	if ((*a)->lchild == NULL && (*a)->rchild == NULL)(*a)->bf = 0;
	else if ((*a)->lchild != NULL && (*a)->rchild == NULL)(*a)->bf = 1 + (*a)->lchild->height;
	else if ((*a)->lchild == NULL && (*a)->rchild != NULL)(*a)->bf = -1 - (*a)->rchild->height;
	else (*a)->bf = (*a)->lchild->height - (*a)->rchild->height;
	return;
}
int main()
{
	NODE root = NULL;
	int array[5];
	for (int i = 0;i < 5;i++)
		scanf("%d", &array[i]);
	for (int i = 0;i < 5;i++)
		insertAVL(&root, array[i]);
	find(&root, 0);
	printf("%d", ans[0]);
	for (int i = 1;i < 20;i++)
		if (ans[i])printf(" %d", ans[i]);
	return 0;
}
