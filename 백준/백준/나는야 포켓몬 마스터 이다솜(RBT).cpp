//레드블랙트리는 삭제부분이 엄청 힘들다
//시험에 쓸 수는 없는 구조
#include <cstdio>
#define BLACK 0
#define RED 1

typedef unsigned long long ull;
inline void strcpy(char *p, char *q) { while (*p++ = *q++); }
inline int strcmp(char *p, char *q)
{
	int i;
	for (i = 0; p[i] && q[i];i++) if (p[i] != q[i]) break;
	return p[i] - q[i];
}
inline ull gethash(char *p)
{
	ull hash = 5381;
	for (int i = 0; p[i]; i++) hash = hash * 33 + p[i];
	return hash;
}
struct RBT
{
	ull hash;
	char name[21];
	int idx, col, leaf;
	RBT *left, *right, *par;
	RBT(RBT *par, int col = BLACK) { this->col = col; name[0] = 0, leaf = 1, hash = 0, idx = 0; left = right = 0; this->par = par; }
	~RBT() { delete left, right; }
	void insert(char *p,int i)
	{
		if (leaf)
		{
			leaf = 0;
			left = new RBT(this), right = new RBT(this);
			hash = gethash(p); idx = i;
			strcpy(name, p);
			check_and_update();
			if(par!=0) col = RED;
			return;
		}
		if (strcmp(p, name) > 0) right->insert(p, i);
		else left->insert(p, i);
	}
	void check_and_update()
	{
		if (par == 0) return;
		if (par->col == BLACK) return;
		RBT *tmp;
		if (par->right == this) tmp = par->left;
		else tmp = par->right;
		if (tmp->col == RED) recolor();
		else reconstruct();
	}
	void recolor()
	{
		RBT *grand = par->par;
		grand->left->col = grand->right->col = BLACK;
		if (grand->par != 0)
		{
			grand->col = RED;
			grand->check_and_update();
		}
	}
	void reconstruct()
	{
		RBT *grand = par->par;
		RBT *parent = par;
		if (par == grand->right)
		{
			if (par->right == this)
			{
				par->LR();
				col = grand->col = RED;
				parent->col = BLACK;
			}
			else 
			{ 
				RR(); LR(); 
				col = BLACK;
				grand->col = parent->col = RED;
			}
			return;
		}
		if (par->left == this)
		{
			par->RR();
			parent->col = BLACK;
			col = grand->col = RED;
		}
		else 
		{ 
			LR(); RR(); 
			col = BLACK;
			parent->col = grand->col = RED;
		}
	}
	void LR()
	{
		RBT *tmp = par;
		par->right = left;
		left->par = par;

		par = par->par;
		if (par != 0)
		{
			if (tmp == par->right) par->right = this;
			else par->left = this;
		}

		tmp->par = this;
		left = tmp;
	}
	void RR()
	{
		RBT *tmp = par;
		tmp->left = right;
		right->par = tmp;
		par = par->par;
		if (par != 0)
		{
			if (tmp == par->right) par->right = this;
			else par->left = this;
		}
		tmp->par = this;
		right = tmp;
	}
	int find(char *p)
	{
		if (name[0] == 0) return 0;
		ull thash = gethash(p);
		if (thash == hash && strcmp(p, name) == 0) return idx;
		if (strcmp(p, name) > 0) right->find(p);
		else left->find(p);
	}
	void del(char *p)
	{
		ull hash = gethash(p);
		if (strcmp(p, name) > 0) right->del(p);
		else left->del(p);
		if (hash == this->hash && strcmp(p, name) == 0)
		{
			RBT *tmp = left;
			while (!leaf) tmp = tmp->right;
			tmp->par->right = new RBT(tmp->par);
			tmp->par = par;
			delete tmp->left, tmp->right;
			tmp->left = left, tmp->right = right;
			left->par = right->par = tmp;
			left = right = 0;
			if (col == RED)	{delete this; return;}
			if (tmp->col == RED)
			{
				tmp->col = BLACK;
				delete this; return;
			}
			else
			{

			}
		}
	}
};

RBT *root;
char name[100001][21];
int N, M;
int atoi(char *str);
int main()
{
	root = new RBT(0);
	scanf("%d %d", &N, &M);
	for (int i = 1; i <= N; i++)
	{
		scanf("%s", name[i]);
		root->insert(name[i], i);
		while (root->par) root = root->par;
	}
	for (int i = 0; i < M; i++)
	{
		int input;
		char in[21];
		scanf("%s", in);
		if ('0' <= in[0] && in[0] <= '9')
		{
			input = atoi(in);
			printf("%s\n", name[input]);
		}
		else
		{
			int idx = root->find(in);
			printf("%d\n", idx);
		}
	}
}
int atoi(char *p)
{
	int sum = 0;
	for (int i = 0; p[i]; i++) sum = sum * 10 + p[i] - '0';
	return sum;
}