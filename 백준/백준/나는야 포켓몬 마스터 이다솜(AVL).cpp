//AVL 트리 연습용
//로테이션 부분에 많은 신경을 써야 함(까먹고 두면 이상한 곳으로 연결됨)
#include <cstdio>
typedef unsigned long long ull;
inline int max(int x, int y) { return x > y ? x : y; }
inline int abs(int x) { return x > 0 ? x : -x; }
inline void strcpy(char *p, char *q) { while (*p++ = *q++); }
inline int strcmp(char *p, char *q)
{
	int i;
	for (i = 0; p[i] && q[i]; i++)
	{
		if (p[i] != q[i]) break;
	}
	return p[i] - q[i];
}
inline ull gethash(char *p)
{
	ull hash = 5381;
	for (int i = 0; p[i]; i++)
	{
		hash = hash * 33 + p[i];
	}
	return hash;
}


struct AVL
{
	ull hash;
	char name[21];
	int height;
	int h_diff;
	int idx;
	AVL *par, *left, *right;
	AVL(AVL *t) { name[0]=0,idx = hash = h_diff = 0, height = -1, par = t, left = right = 0; }
	~AVL() { delete left, right; }
	void get_height()
	{
		height = max(left->height, right->height) + 1;
		h_diff = abs(left->height - right->height);
		if (h_diff > 1) update();
	}
	void insert(char *p,int i)
	{
		ull hash = gethash(p);
		if (height == -1)
		{
			this->hash = hash;
			strcpy(name, p);
			left = new AVL(this), right = new AVL(this);
			height = 0;
			idx = i;
			return;
		}
		if (strcmp(p, this->name) > 0) right->insert(p,i);
		else left->insert(p,i);
		get_height();
	}
	void del(char *p)
	{
		ull hash = gethash(p);
		if (height == -1) return;
		if (hash == this->hash && strcmp(p, this->name) == 0)
		{
			AVL *t = this;
			AVL *cand;
			if (!this->height)
			{
				delete left, right;
				height--;
				name[0] = hash = h_diff = 0;
				return;
			}
			if (left) {
				cand = left;
				while (!cand->height) cand = cand->right;
			}
			else
			{
				cand = right;
				while (!cand->height) cand = cand->left;
			}
			delete cand->left, cand->right;

			AVL *update_node = cand->par;
			cand->par->right = new AVL(cand->par);
			cand->par = par;

			if (par->right == t) par->right = cand;
			else par->left = cand;
			cand->right = right, cand->left = left;
			right->par = left->par = cand;
			right = left = 0; delete t;
			while (update_node->par != 0)
			{
				get_height(); update_node = update_node->par;
			}
		}
		if (strcmp(p, this->name) > 0) right->del(p);
		else left->del(p);
	}
	void update()
	{
		if (left->height > right->height)
		{
			if (left->left->height > left->right->height) left->LR();
			else{left->LR(),left->par->RR();}
		}
		else
		{
			if (right->right->height > right->left->height) right->RR();
			else{right->RR(), right->par->LR();	}
		}
	}
	int find(char *p)
	{
		ull hash = gethash(p);
		if (height == -1) return 0;
		if (hash == this->hash && strcmp(p, this->name) == 0) return idx;
		if (strcmp(p, this->name) > 0) right->find(p);
		else left->find(p);
	}

	void LR()
	{
		if (par == 0) return;
		AVL *tmp = par; //부모 임시저장

		tmp->left = right; //부모의 오른쪽을 자기의 왼쪽으로 바꾸기
		right->par = tmp; //자기 왼쪽의 부모를 자기의 부모로 바꾸기

		right = tmp; //자기 왼쪽 자식으로 부모를 넣기

		par = tmp->par;//부모의 부모를 자기의 부모로 바꾸기
		tmp->par = this;

		if (par != 0) {
			if (par->left == right) par->left = this; //부모의 자식을 자신으로 바꾸기
			else par->right = this;
		}

		tmp->height = max(tmp->right->height, tmp->left->height) + 1;
		tmp->h_diff = abs(tmp->left->height - tmp->right->height);
	}
	void RR()
	{
		if (par == 0) return;
		AVL *tmp=par;//부모 임시저장

		tmp->right = left; //부모의 오른쪽을 자기의 왼쪽으로 바꾸기
		left->par = tmp; //자기 왼쪽의 부모를 자기의 부모로 바꾸기

		left = par; //자기 왼쪽 자식으로 부모를 넣기

		par = tmp->par; //부모의 부모를 자기의 부모로 바꾸기
		tmp->par = this;
		if (par != 0)
		{
			if (par->left == left) par->left = this; //부모의 자식을 자신으로 바꾸기
			else par->right = this;
		}

		tmp->height = max(tmp->right->height, tmp->left->height) + 1;
		tmp->h_diff = abs(tmp->left->height - tmp->right->height);
	}
};
AVL *root;
char name[100001][21];
int N, M;
int atoi(char *str);
int main()
{
	root = new AVL(0);
	scanf("%d %d", &N, &M);
	for (int i = 1; i <= N; i++)
	{
		scanf("%s", name[i]);
		root->insert(name[i],i);
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
	int sum=0;
	for (int i = 0; p[i]; i++) sum = sum * 10 + p[i] - '0';
	return sum;
}