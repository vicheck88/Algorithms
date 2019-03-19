#include <stdio.h>
#define INF 0x7f123456;
typedef unsigned long long ull;
int N, M;
ull cost[20001];
struct DATA
{
	int node;
	int cost;
	DATA *next;
	DATA(int node = 0, int cost = 0, DATA *n = 0)
	{
		this->node = node, this->cost = cost;
		if (n) n->next = this;
		next = 0;
	}
};
struct LIST
{
	DATA *head, *tail;
	LIST() { head = tail = new DATA(); }
	void insert(int node, int cost)
	{
		tail = new DATA(node, cost, tail);
	}
};
LIST list[20001];
struct QUEUE
{
	int wp, rp;
	DATA queue[100000 + 10];
	QUEUE()
	{
		for (int i = 0; i < 100000 + 10; i++) queue[i] = DATA();
		wp = rp = 0;
	}
	bool empty() { return wp == rp; }
	void enqueue(DATA &d,ull c)
	{
		if (cost[d.node] <= c) return;
		cost[d.node] = c;
		queue[wp++] = d;
	}
	DATA dequeue() { return queue[rp++]; }
};
QUEUE queue;
void BFS();
int main()
{
	scanf("%d %d", &N, &M);
	for (int i = 1; i <= N; i++) cost[i] = INF;
	int from, to, c;
	for (int i = 0; i < M; i++)
	{
		scanf("%d %d %d", &from, &to, &c);
		list[from].insert(to, c);
	}
	BFS();
	printf("%llu", cost[N]);
}
void BFS()
{
	DATA *cur = list[1].head;
	cost[1] = 0;
	for (; cur; cur = cur->next) queue.enqueue(*cur, cur->cost);
	while (!queue.empty())
	{
		DATA tmp = queue.dequeue();
		ull c = cost[tmp.node];
		cur = list[tmp.node].head;
		for (; cur; cur = cur->next) 
			queue.enqueue(*cur, c + cur->cost);
	}
}