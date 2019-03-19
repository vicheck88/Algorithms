#include <stdio.h>
typedef unsigned long long ull;
enum {right,down,left,up};
int N, M, T;
int totCnt;
ull puzzle[2510][5];
ull pattern = 2222222222222222;
char str[20];
int chk[2510];
int map[51][51];
void puzzleInput();
void setTopLeftCnt();
void setPuzzle();
int main()
{
	int samecnt = 0;
	scanf("%d %d %d", &N, &M, &T);
	totCnt = N * N + T;
	puzzleInput();
	setTopLeftCnt();
	setPuzzle();
	int r, c;
	scanf("%d %d", &r, &c);
	printf("%d\n", map[r][c]);
	for (int i = 0; i < totCnt; i++)
	{
		if (chk[i]) continue;
		printf("%d ", i);
	}
	return 0;
}

void puzzleInput()
{
	int n;
	for (int i = 0; i < totCnt; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			scanf("%s", str);
			for (int k = 0; str[k]; k++)
			{
				if (str[k] == 'M') n = 2;
				else if (str[k] == '0') n = 1;
				else n = 0;
				puzzle[i][j] = puzzle[i][j] * 10 + n;
			}
		}
	}
}
void setTopLeftCnt()
{
	ull pat = pattern;
	for (int i = 0; i < 16 - M; i++)
	{
		pat /= 10;
	}
	for (int i = 0; i < totCnt; i++)
	{
		for (int j = 0; j < totCnt; j++)
		{
			if (puzzle[i][up] + puzzle[j][down] == pat) puzzle[i][4]++;
			if (puzzle[i][left] + puzzle[j][right] == pat) puzzle[i][4]++;
		}
	}
}
int DFS(int n);
void setPuzzle()
{
	int cnt = 0;
	for (;;)
	{
		for (int i = 0; i < totCnt; i++)
		{
			if (puzzle[i][4] == cnt)
			{
				chk[i] = 1;
				map[0][0] = i;
				if (DFS(1)) return;
				chk[i] = 0;
			}
		}
		cnt++;
	}
}
int DFS(int n)
{
	ull pat = pattern;
	for (int i = 0; i < 16 - M; i++)
	{
		pat /= 10;
	}
	if (n >= N * N) return 1;
	int r = n / N, c = n % N;
	for (int i = 0; i < totCnt; i++)
	{
		if (chk[i]) continue;
		if (r && puzzle[map[r - 1][c]][down] + puzzle[i][up] != pat) continue;
		if (c && puzzle[map[r][c - 1]][right] + puzzle[i][left] != pat) continue;
		chk[i] = 1;
		map[r][c] = i;
		if (DFS(n + 1)) return 1;
		chk[i] = 0;
		map[r][c] = 0;
	}
	return 0;
}