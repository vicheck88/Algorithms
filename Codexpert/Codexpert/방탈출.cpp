#include <stdio.h>
int N;
int cur[100+10];
int lock[100+10];
int chk[100][10][10][10];
int distance(int x, int y);

struct DATA
{
	int arr[3];
	int start;
	int d;
	DATA(int start = 0, int d = 0, int a = 0, int b = 0, int c = 0)
	{
		this->start = start;
		this->d = d;
		arr[0] = a, arr[1] = b, arr[2] = c;
	}
	bool operator>(DATA &n)
	{
		if (d<n.d || (d == n.d && start>n.start)) return true;
		return false;
	}
};


DATA heap[100 * 10 * 10 * 10];
void swap(DATA &a, DATA &b) { DATA tmp = a; a = b, b = tmp; }
struct HEAP
{
	const int first = { 1 };
	int last;
	HEAP() { last = 0; }
	bool empty() { return last == 0; }
	void enqueue(int start, int d, int a, int b, int c)
	{
		int &ret = chk[start][a][b][c];
		if (ret && ret<=d) return;
		if(ret) printf("lock: %d %d %d, sum:%d to %d\n", a, b, c, ret, d);
		ret = d;
		heap[++last] = DATA(start, d, a, b, c);
		int n = last;
		while (n / 2)
		{
			if (heap[n] > heap[n / 2]) swap(heap[n], heap[n / 2]);
			else break;
			n /= 2;
		}
	}
	DATA dequeue()
	{
		DATA tmp = heap[first];
		swap(heap[first], heap[last]);
		heap[last] = DATA();
		last--;
		for (int i = 1; i * 2 <= last;)
		{
			int idx;
			if (i*2+1<=last && heap[i * 2 + 1] > heap[i * 2]) idx = i * 2 + 1;
			else idx = i * 2;
			if (heap[idx] > heap[i]) swap(heap[i], heap[idx]);
			else break;
			i = idx;
		}
		return tmp;
	}
};
HEAP H;
int BFS();
int main()
{
	scanf("%d", &N);
	for (int i = 0; i < N; i++) scanf("%d", &cur[i]);
	for (int i = 0; i < N; i++) scanf("%d", &lock[i]);
	printf("%d", BFS());
	return 0;
}
int circle(int x)
{
	return x >= 10 ? x - 10 : (x < 0 ? x + 10 : x);
}
int BFS()
{
	H.enqueue(0, 0, cur[0], cur[1], cur[2]);
	while (!H.empty())
	{
		DATA tmp = H.dequeue();
		if (tmp.arr[0] == lock[tmp.start])
		{
			if (tmp.start == N - 1) return tmp.d;
			H.enqueue(tmp.start + 1, tmp.d, tmp.arr[1], tmp.arr[2], cur[tmp.start + 3]);
		}
		for (int i = 1; i <= 3; i++)
		{
			H.enqueue(tmp.start, tmp.d + 1, circle(tmp.arr[0] + i), tmp.arr[1], tmp.arr[2]);
			if(tmp.start+1 < N) H.enqueue(tmp.start, tmp.d + 1, circle(tmp.arr[0] + i), circle(tmp.arr[1] + i), tmp.arr[2]);
			if(tmp.start+2 < N) H.enqueue(tmp.start, tmp.d + 1, circle(tmp.arr[0] + i), circle(tmp.arr[1] + i), circle(tmp.arr[2] + i));
		}
		for (int i = 1; i <= 3; i++)
		{
			H.enqueue(tmp.start, tmp.d + 1, circle(tmp.arr[0] - i), tmp.arr[1], tmp.arr[2]);
			if(tmp.start+1 < N) H.enqueue(tmp.start, tmp.d + 1, circle(tmp.arr[0] - i), circle(tmp.arr[1] - i), tmp.arr[2]);
			if(tmp.start+2 < N) H.enqueue(tmp.start, tmp.d + 1, circle(tmp.arr[0] - i), circle(tmp.arr[1] - i), circle(tmp.arr[2] - i));
		}
	}
	return -1;
}