#include <stdio.h>
int N;
struct HEAP
{
	int heap[20000 + 10];
	int last;
	const int first = { 1 };
	HEAP()
	{
		last = 0;
		for (int i = 0; i < 20000 + 10; i++) heap[i] = 0;
	}
	void enqueue(int num,bool cmp(int,int))
	{
		heap[++last] = num;
		int n = last;
		while (n / 2)
		{
			if (cmp(heap[n],heap[n / 2])) swap(heap[n], heap[n / 2]);
			else break;
			n /= 2;
		}
	}
	int front()
	{
		return heap[first];
	}
	int dequeue(bool cmp(int,int))
	{
		swap(heap[first], heap[last]);
		int tmp = heap[last];
		heap[last--] = 0;
		for (int i = 1; i * 2 <= last;)
		{
			int idx;
			if (i*2+1<=last && cmp(heap[i * 2 + 1], heap[i * 2])) idx = i * 2 + 1;
			else idx = i * 2;
			if (cmp(heap[idx], heap[i])) swap(heap[idx], heap[i]);
			else break;
			i = idx;
		}
		return tmp;
	}
	void swap(int &a, int &b)
	{
		int tmp = a; a = b, b = tmp;
	}
};
struct MAX_HEAP :public HEAP
{
	void enqueue(int num)
	{
		HEAP::enqueue(num, [](int a, int b) {return a > b; });
	}
	int dequeue()
	{
		return HEAP::dequeue([](int a, int b) {return a > b; });
	}
};
struct MIN_HEAP :public HEAP
{
	void enqueue(int num)
	{
		HEAP::enqueue(num, [](int a, int b) {return a < b; });
	}
	int dequeue()
	{
		return HEAP::dequeue([](int a, int b) {return a < b; });
	}
};
MAX_HEAP max_heap;
MIN_HEAP min_heap;
int main()
{
	int N;
	int num[2];
	scanf("%d", &N);
	scanf("%d", &num[0]);
	min_heap.enqueue(num[0]);
	printf("%d\n", num[0]);
	for (int i = 0; i < (N - 1) / 2; i++)
	{
		scanf("%d %d", &num[0],&num[1]);
		for (int i = 0; i < 2; i++)	min_heap.enqueue(num[i]);
		while (max_heap.front() > min_heap.front())
		{
			max_heap.enqueue(min_heap.dequeue());
		}
		while (max_heap.last + 1 > min_heap.last)
		{
			min_heap.enqueue(max_heap.dequeue());
		}
		while (max_heap.last + 1 < min_heap.last)
		{
			max_heap.enqueue(min_heap.dequeue());
		}
		printf("%d\n", min_heap.front());
	}
}