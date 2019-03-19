#include <stdio.h>
int N;
long long M;
int maxs = 0x7f123456;
inline int max(int a, int b) { return a > b ? a : b; }
struct DATA
{
	int f, s;
	DATA() { f = s = 0; }
	void insert(int f, int s) { this->f = f, this->s = s; }
};
DATA food[100000 + 10];
struct TREE
{
	DATA *tree;
	DATA *arr;
	int size;
	TREE(int n)
	{
		int i;
		for (i = 1; i <= n; i <<= 1);
		tree = new DATA[i*2]();
		size = i;
		for (int j = i; j < N + i; j++)
		{
			scanf("%d %d", &tree[j].f, &tree[j].s);
		}
		arr = &tree[i - 1];
		for (int j = i - 1; j >= 1; j--)
		{
			tree[j].s = max(tree[j * 2].s, tree[j * 2 + 1].s);
		}
	}
	int query(int qs, int qe,int s,int e,int node)
	{
		int m = (s + e) / 2;
		if (qe < s || e < qs) return 0;
		if (qs <= s && e <= qe) return tree[node].s;
		return max(query(qs, qe, s, m, node * 2), query(qs, qe, m + 1, e, node * 2 + 1));
	}
};
void find_sol();
TREE *tree;
int main()
{
	scanf("%d %lld", &N, &M);
	tree = new TREE(N);
	find_sol();
	printf("%d", maxs);
	delete tree;
	return 0;
}
void find_sol()
{
	long long sum = 0;
	int sol, s = 1, e = 1;
	while (e<=N)
	{
		while (sum < M && e<=N) sum += tree->arr[e++].f;
		if (sum < M) break;
		while (sum >= M) sum -= tree->arr[s++].f;
		sol = tree->query(s - 1, e - 1,1,tree->size,1);
		if (sol < maxs) maxs = sol;
	}
}