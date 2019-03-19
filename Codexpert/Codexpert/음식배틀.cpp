#include <stdio.h>
#define INF 0x7f123456
int N, D;
struct DATA
{
	int taste[2]; //0:±èÁØÇö, 1: ¹®¼¼À±
	int idx;
	int dist;
	int order;
	DATA * prev;
	DATA(int kim = 0, int moon = 0, int idx=0, int order=0, int dist = 0, DATA *prev = 0)
	{
		taste[0] = kim, taste[1] = moon, this->dist = dist, this->prev = prev;
		this->idx = idx;
		this->order = order;
	}
	bool operator>(DATA &n)
	{
		if (dist < n.dist) return true;
		return false;
	}
};

DATA data[2][100000 + 10]; //0:±èÁØÇö, 1:¹®¼¼À±
DATA tmp[100000 + 10];
int arr[2][100000 + 10];
inline void swap(DATA &a, DATA &b) { DATA tmp = a; a = b; b = tmp; }

struct QUEUE
{
	int wp, rp;
	DATA queue[200000 + 10];
	QUEUE()
	{
		wp = rp = 0;
		for (int i = 0; i < 200000 + 10; i++) queue[i] = DATA();
	}
	bool empty() { return wp==rp; }
	void enqueue(DATA dat)
	{
		if (arr[dat.order][dat.idx]<INF) return;
		arr[dat.order][dat.idx] = dat.dist;
		queue[wp++] = dat;
	}
	DATA dequeue()
	{
		return queue[rp++];
	}
	int upperbound(int n,int ord)
	{
		int ans = 0;
		int s = 1, e = N, m;
		while (s <= e)
		{
			m = (s + e) / 2;
			if (data[ord][m].taste[ord^1] > n) e = m - 1;
			else
			{
				ans = m;
				s = m + 1;
			}
		}
		return ans;
	}
};
QUEUE queue;
void input();
void BFS();
int main()
{
	input();
	
	BFS();
	for (int i = 1; i <= N; i++)
	{
		if(arr[0][i]<INF) printf("%d\n", arr[0][i]);
		else printf("-1\n");
	}
}
void sort(DATA *n,int s,int e,int ord);
void input()
{
	int kim, moon;
	scanf("%d %d", &N, &D);
	for (int i = 0; i <= N; i++)
	{
		arr[0][i] = arr[1][i] = INF;
	}
	for (int i = 1; i <= 2*N; i++)
	{
		scanf("%d %d", &kim, &moon);
		if (i <= N)
		{
			data[0][i] = DATA(kim, moon, i);
			if (data[0][i].taste[1] == 0)
			{
				data[0][i].dist = 1;
				queue.enqueue(data[0][i]);
			}
		}
		else
		{
			data[1][i - N] = DATA(kim, moon, i - N, 1);
			if (data[1][i - N].taste[0] == 0)
			{
				data[1][i - N].dist = 1;
				queue.enqueue(data[1][i - N]);
			}
		}
	}
	sort(data[0],1,N,1); sort(data[1],1,N,0);
}
void sort(DATA *n,int s,int e,int ord)
{
	if (s >= e) return;
	int i = s, m = (s + e) / 2, j = m + 1, k = s;
	sort(n, s, m,ord); sort(n, m + 1, e,ord);
	while (i <= m && j <= e)
	{
		if (n[i].taste[ord] < n[j].taste[ord])	tmp[k++] = n[i++];
		else tmp[k++] = n[j++];
	}
	while (i <= m) tmp[k++] = n[i++];
	while (j <= e) tmp[k++] = n[j++];
	for (int i = s; i <= e; i++) n[i] = tmp[i];
}
void BFS()
{
	int ord,d,upper,lower;
	while (!queue.empty())
	{
		DATA tmp = queue.dequeue();
		ord = tmp.order;
		d = tmp.dist;
		upper = queue.upperbound(tmp.taste[ord], ord ^ 1);
		lower = queue.upperbound(tmp.taste[ord] - D-1, ord ^ 1)+1;
		for (int i = lower; i <= upper; i++)
		{
			DATA n = data[ord ^ 1][i];
			n.dist = d + 1;
			queue.enqueue(n);
		}
	}
}