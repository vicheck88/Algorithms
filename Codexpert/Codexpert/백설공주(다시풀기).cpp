#include <stdio.h>

int N, C, M;
inline void swap(int &a, int &b) { int tmp = a; a = b, b = tmp; }
inline int max(int a, int b) { return a > b ? a : b; }
struct VECTOR
{
	int idx;
	int lim;
	const int cap = { 300 };
	int *arr;
	VECTOR() { lim = cap, idx = 0; arr = new int[lim](); }
	~VECTOR() { delete[] arr; }
	void push_back(int n)
	{
		if (idx + 1 >= lim)
		{
			lim += cap;
			int *tmp = arr;
			arr = new int[lim]();
			for (int i = 1; i <= idx; i++) arr[i] = tmp[i];
			delete[] tmp;
		}
		arr[++idx] = n;
	}
	int upperbound(int r)
	{
		int s = 1, e = idx;
		int m;
		int ans = 0;
		while (s <= e)
		{
			m = (s + e) / 2;
			if (arr[m] == r) return m;
			else if (arr[m] < r)
			{
				ans = m; s = m + 1;
			}
			else e = m - 1;
		}
		return ans;
	}
};
VECTOR vec[10000 + 10];

struct DATA
{
	int num, cnt;
	DATA(int n = 0, int c = 0) { num = n, cnt = c; }
};
struct TREE
{
	int size;
	DATA *dat;
	DATA merge(DATA a, DATA b)
	{
		if (a.num == b.num) return DATA(a.num, a.cnt + b.cnt);
		else if (a.cnt > b.cnt) return DATA(a.num, a.cnt - b.cnt);
		else if (b.cnt>a.cnt) return DATA(b.num, b.cnt - a.cnt);
		else return DATA(max(a.num, b.num), 0);
	}
	TREE(int N)
	{
		int i;
		for (i = 1; i <= N; i <<= 1);
		dat = new DATA[i * 2]();
		insert(1, N, 1);
	}
	void insert(int s, int e, int node)
	{
		if (s == e)
		{
			scanf("%d", &dat[node].num);
			dat[node].cnt = 1;
			vec[dat[node].num].push_back(s);
			return;
		}
		int m = (s + e) / 2;
		insert(s, m, node * 2); insert(m + 1, e, node * 2 + 1);
		dat[node] = merge(dat[node * 2], dat[node * 2 + 1]);
	}
	~TREE() { delete[] dat; }
	DATA query(int qs, int qe,int s,int e,int node)
	{
		if (qe < s || e < qs) return DATA(0, 0);
		if (qs <= s && e <= qe) return dat[node];
		int m = (s + e) / 2;
		return merge(query(qs,qe,s,m,node*2), query(qs, qe,m+1,e,node*2+1));
	}
};
TREE *tree;
int main()
{
	scanf("%d %d", &N, &C);
	tree = new TREE(N);
	scanf("%d", &M);
	int qs, qe;
	for (int i = 0; i < M; i++)
	{
		scanf("%d %d", &qs, &qe);
		DATA tmp = tree->query(qs, qe, 1, N, 1);
		int max = (qe - qs + 1) / 2;
		//int upper = vec[tmp.num].upperbound(qe);
		//int lower = vec[tmp.num].upperbound(qs - 1);
		int sol = vec[tmp.num].upperbound(qe) - vec[tmp.num].upperbound(qs - 1);
		if (sol > max) printf("yes %d\n", tmp.num);
		else printf("no\n");
	}
	delete tree;
	return 0;
}