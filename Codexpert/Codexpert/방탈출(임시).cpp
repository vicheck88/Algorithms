#include <stdio.h>

#define MAX_N					100
#define INF						1000000  // seems to be OK if more than 10 * N (1000)
#define MAX(a,b)				((a>b) ? a : b)

int N;
char s[MAX_N + 1], t[MAX_N + 1], delta[MAX_N + 1];
int cache[MAX_N][10][10];

int move[10] = { 0, 1, 1, 1, 2, 2, 2, 1, 1, 1 };

void init_data(void)
{
	int k, a1, a2;

	for (k = 0; k < N; k++)
		delta[k] = (t[k] - s[k] + 10) % 10;

	for (k = 0; k < N; k++)
		for (a1 = 0; a1 < 10; a1++)
			for (a2 = 0; a2 < 10; a2++)
				cache[k][a1][a2] = -1;
}

int GetMinimum(int k, int a1, int a2)
{
	int t3, t2, t1;
	int next_a1, next_a2;
	int remaining = N - k + 1;
	int min = INF, tmin;

	if (cache[k][a1][a2] != -1)
		return cache[k][a1][a2];

	if (remaining == 1)
	{
		t1 = (delta[k] - a1 + 10) % 10;
		min = move[t1];
	}
	else if (remaining == 2)
	{
		for (t2 = 0; t2 < 10; t2++)
		{
			t1 = (20 + delta[k] - t2 - a1) % 10;
			next_a1 = (a2 + t2) % 10;

			if (move[t1] + move[t2] <= 2)
			{
				tmin = move[t1] + move[t2] + GetMinimum(k + 1, next_a1, 0);
				if (tmin < min) min = tmin;
			}
		}
	}
	else if (remaining >= 3)
	{
		for (t3 = 0; t3 < 10; t3++)
		{
			for (t2 = 0; t2 < 10; t2++)
			{
				t1 = (30 + delta[k] - t3 - t2 - a1) % 10;
				next_a1 = (a2 + t2 + t3) % 10;
				next_a2 = t3;

				if (move[t1] + move[t2] + move[t3] <= 2)
				{
					tmin = move[t1] + move[t2] + move[t3] + GetMinimum(k + 1, next_a1, next_a2);
					if (tmin < min) min = tmin;
				}
			}
		}
	}

	cache[k][a1][a2] = min;
	return min;
}

int main(void)
{
	int Answer;
	setbuf(stdout, NULL);

	scanf("%d", &N);
	scanf("%s %s", &s, &t);

	init_data();
	Answer = GetMinimum(0, 0, 0);
	printf("%d\n", Answer);

	return 0;
}