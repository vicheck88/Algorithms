#include <cstdio>
struct NODE
{
	int n;
	int prev;
	NODE(int n = 0, int prev = 0) { this->n = n, this->prev = prev; }
};
struct STACK
{
	long long sum;
	int sp;
	int max;
	NODE *arr;
	STACK(int N=0) { arr = new NODE[N]; sp = 0, sum = 0, max = 0; }
	~STACK() { delete arr; }
	void push(int n, int prev=0) 
	{ 
		arr[sp++] = NODE(n, prev);
		if (n > max) max = n;
	}
	NODE &top() { return arr[sp-1]; }
	void pop() { sp--; }
	bool empty() { return sp==0; }
	void getsum(int n)
	{
		while (!empty() && top().n < n)
		{
			sum++; pop();
		}
		int prev = 0;
		if (!empty())
		{
			if (n == top().n) prev = top().prev + 1;
			sum += prev;
			if (max>n) sum++;
		}
		push(n,prev);
	}
};
STACK *stack;
int main()
{
	int N;
	int num;
	scanf("%d", &N);
	stack = new STACK(N);
	for (int i = 0; i < N; i++)
	{
		scanf("%d", &num);
		stack->getsum(num);
	}
	printf("%lld", stack->sum);
	delete stack;
	return 0;
}