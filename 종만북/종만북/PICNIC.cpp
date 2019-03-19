#include <stdio.h>
int T;
int N, M;
int map[10][10];
int chk[10];
int cnt;
void init();
void input();
void solve(int n);
int main()
{
	scanf("%d", &T);
	while (T--)
	{
		init();
		input();
		solve(0);
		printf("%d\n", cnt);
	}
}
void init()
{
	for (int i = 0; i < 10; i++)
	{
		chk[i] = 0;
		for (int j = 0; j < 10; j++) map[i][j] = 0; 
	}
	cnt = 0;
}
void input()
{
	int ni, nj;
	scanf("%d %d", &N, &M);
	for (int i = 0; i < M; i++)
	{
		scanf("%d %d", &ni, &nj);
		map[ni][nj] = map[nj][ni] = 1;
	}
}
void solve(int n)
{
	int j;
	chk[n] = 1;
	for (int i = n + 1; i < N; i++)
	{
		if (chk[i]) continue;
		if (!map[i][n] && !map[n][i]) continue;
		chk[i] = 1;
		j = n + 1;
		while (chk[j] && j < N) j++;
		if (j == N)
		{
			cnt++; chk[i] = 0; chk[n] = 0; return;
		}
		solve(j);
		chk[i] = 0;
	}
	chk[n] = 0;
}