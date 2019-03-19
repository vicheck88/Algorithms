#define SIZE 4096
#define NULL -1
int order[16][16];
int chk[256];

unsigned char tmp[SIZE][SIZE];
inline void init()
{
	for (int i = 0; i < 256; i++)
	{
		order[0][i] = NULL;
		chk[i] = NULL;
	}
}
inline void mapcopy(unsigned char A[SIZE][SIZE], unsigned char B[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++) 
			A[i][j] = B[i][j];
	}
}
inline bool chkZero(int n, int r1, int r2, int c1, int c2)
{
	int r = n >> 4, c = n & 15;
	for (int i = r1; i < r2; i++)
	{
		for (int j = c1; j < c2; j++)
		{
			if (tmp[(r << 8) + i][(c << 8) + j]) 
				return false;
		}
	}
	return true;
}
inline bool isCandForFirst(int n)
{
	int r = n >> 4, c = n & 15;
	if (!chkZero(n, 0, 256, 0, 16)) return false;
	if (!chkZero(n, 0, 16, 16, 256)) return false;
	return true;
}

bool check(int n, int ord)
{
	if (!(ord >> 4) && !chkZero(n, 0, 16, 0, 256)) return false;
	if (!(ord & 15) && !chkZero(n, 0, 256, 0, 16)) return false;
	if ((ord >> 4) == 15 && !chkZero(n, 240, 256, 0, 256)) return false;
	if ((ord & 15) == 15 && !chkZero(n, 0, 256, 240, 256)) return false;

	int prev;
	int r = (n >> 4) << 8, c = (n & 15) << 8;
	int pr, pc;
	if (ord >> 4)
	{
		prev = order[-1][ord];
		for (int i = 0; i < 256; i++)
		{
			if (tmp[r][c + i] && !chkSquare(prev, r, c + i)) return false;
		}
	}
	if (ord & 15)
	{
		prev = order[0][ord - 1];
		for (int i = 0; i < 256; i++)
		{
			if (tmp[r + i][c] && !chkSquare(prev, r + i, c)) return false;
		}
	}
	return true;
}

bool DFS(int ord)
{
	if (ord == 256) return true;
	for (int i = 0; i < 256; i++)
	{
		if (chk[i] > -1) continue;
		if (!check(i, ord)) continue;
		chk[i] = ord;
		order[ord >> 4][ord & 15] = i;
		if (DFS(ord + 1)) return true;
		chk[i] = order[ord >> 4][ord & 15] = NULL;
	}
	return false;
}
void solve(unsigned char map[SIZE][SIZE])
{
	int r, c;
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			r = order[i][j] >> 4, c = order[i][j] & 15;
			for (int a = 0; a < 256; a++)
			{
				for (int b = 0; b < 256; b++)
				{
					map[(i << 8) + a][(j << 8) + b] = tmp[(r << 8) + a][(c << 8) + b];
				}
			}
		}
	}
}

void recover(unsigned char map[SIZE][SIZE])
{
	init();
	mapcopy(tmp,map);
	for (int i = 0; i < 256; i++)
	{
		if (isCandForFirst(i))
		{
			chk[i] = 0;
			order[0][0] = i;
			if (DFS(1)) break;
			chk[i] = NULL;
			order[0][0] = NULL;
		}
	}
	solve(map);
}