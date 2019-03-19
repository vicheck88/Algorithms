#include <stdio.h>
int R, C;
int valid;
int cnt;
int T;
int block_type[4][3][2]=
{
	{ { 0,0 },{0,1},{1,1} },
	{ { 0,0 },{1,0},{1,1} },
	{ { 0,0 },{1,-1},{1,0} },
	{ { 0,0 },{0,1},{1,0} }
};
int map[25][25];
bool chk_set_board(int r, int c, int type,int num)
{
	if (num != '.')
	{
		for (int i = 0; i < 3; i++)
		{
			int nr = r + block_type[type][i][0];
			int nc = c + block_type[type][i][1];
			if (nr < 0 || nr >= R || nc < 0 || nc >= C) return false;
			if (map[nr][nc] != '.') return false;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		int nr = r + block_type[type][i][0];
		int nc = c + block_type[type][i][1];
		map[nr][nc] = num;
	}
	return true;
}


void cover()
{
	int i, j;
	for (i = 0; i < R; i++)
	{
		for (j = 0; j < C; j++)
		{
			if (map[i][j] == '.') goto next;
		}
	}
	next:
	if (i == R && j == C)
	{
		valid++;
		return;
	}
	for (int k = 0; k < 4; k++)
	{
		if(!chk_set_board(i, j, k, 1)) continue;
		cover();
		chk_set_board(i, j, k, '.');
	}
}

int main()
{
	scanf("%d", &T);
	while (T--)
	{
		valid = 0; cnt = 0;
		scanf("%d %d", &R, &C);
		for (int i = 0; i < R; i++)
		{
			for (int j = 0; j < C; j++)
			{
				scanf(" %c", &map[i][j]);
				if (map[i][j] == '.') cnt++;
			}
		}
		if (cnt % 3)
		{
			printf("0\n");
			goto skip;
		}
		cover();
		printf("%d\n", valid);
	skip:;
	}
}