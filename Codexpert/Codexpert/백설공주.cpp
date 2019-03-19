#include <stdio.h>
#include <malloc.h>
int N, C, M;
inline void swap(int &a, int &b) { int tmp = a; a = b, b = tmp; }
inline int max(int a, int b) { return a > b ? a : b; }
int arr[300000 + 10];
struct DATA
{
	int num, cnt;
	DATA(int n = 0, int c = 0) { num = n, cnt = c; }
};
struct TREE
{
	int s, e, m;
	DATA dat;
	TREE *left, *right;
	DATA merge(DATA a, DATA b)
	{
		if (a.num == b.num) return DATA(a.num, a.cnt + b.cnt);
		else if (a.cnt > b.cnt) return DATA(a.num, a.cnt - b.cnt);
		else if (b.cnt>a.cnt) return DATA(b.num, b.cnt - a.cnt);
		else return DATA(max(a.num, b.num), 0);
	}
	TREE(int s, int e)
	{
		this->s = s, this->e = e, m = (s + e) / 2;
		int num;
		left = right = 0;
		if (s == e)
		{
			scanf("%d", &num);
			arr[s] = num;
			dat = DATA(num, 1);
			return;
		}
		left = new TREE(s, m); right = new TREE(m + 1, e);
		dat = merge(left->dat, right->dat);
	}
	~TREE() { delete left, right; }
	DATA query(int qs, int qe)
	{
		if (qe < s || e < qs) return DATA(0, 0);
		if (qs <= s && e <= qe) return dat;
		return merge(left->query(qs, qe), right->query(qs, qe));
	}
};
TREE *tree;
int main()
{
	scanf("%d %d", &N, &C);
	tree = new TREE(1, N);
	scanf("%d", &M);
	int qs, qe;
	for (int i = 0; i < M; i++)
	{
		scanf("%d %d", &qs, &qe);
		DATA tmp = tree->query(qs, qe);
		int max = (qe - qs + 1) / 2;
		int num = 0;
		for (int k = qs; k <= qe; k++)
		{
			if (arr[k] == tmp.num) num++;
		}
		if (num > max) printf("yes %d\n", tmp.num);
		else printf("no\n");
	}
}