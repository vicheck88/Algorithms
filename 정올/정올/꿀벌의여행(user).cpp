#define MAXBEE     7
#define MAXSIZE  100
struct COORD {
	int y, x;
};
/// *** submit area start***
int N;
int map[MAXSIZE][MAXSIZE];
int pattern[MAXSIZE][MAXSIZE];
int chk[MAXBEE];
int finalChk[MAXBEE];
int beeSr, beeSc;
int beeEr, beeEc;
int minLength=0x7fffffff;
COORD patternloc[MAXBEE+1];
COORD bee[MAXBEE+1];
int dir[MAXBEE];
extern bool moveBee(int direction[MAXBEE]);
inline int abs(int x) { return x > 0 ? x : -x; }
void init(int B, COORD bee[MAXBEE]) {
	N = B;
	minLength = 0x7fffffff;
	beeSr = beeSc = 10000;
	for (int i = 0; i < B; i++)
	{
		::bee[i+1] = bee[i];
		map[bee[i].y][bee[i].x] = 1;
		if (beeEr < bee[i].y) beeEr = bee[i].y;
		if (beeEc < bee[i].x) beeEc = bee[i].x;
		if (beeSr > bee[i].y) beeSr = bee[i].y;
		if (beeSc > bee[i].x) beeSc = bee[i].x;
	}
}
void DFS(int sr, int sc, int H, int W, int n,int length,COORD *result);
bool move(int len);
COORD make_pattern(int W, int H, int pattern[MAXSIZE][MAXSIZE]) {
	COORD  result;
	for (int i = 0; i < MAXSIZE; i++)
	{
		for (int j = 0; j < MAXSIZE; j++)
		{
			map[i][j] = 0;
		}
	}
	int cnt = 0;
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			if (!pattern[i][j]) continue;
			patternloc[++cnt].y = i, patternloc[cnt].x = j;
		}
	}
	int sr = (beeSr + beeEr > H) ? (beeSr + beeEr - H) / 2 : 0;
	int sc = (beeSc + beeEc > W) ? (beeSc + beeEc - W) / 2 : 0;
	if (sr + H >= MAXSIZE) sr = MAXSIZE - H;
	if (sc + W >= MAXSIZE) sc = MAXSIZE - W;
	DFS(sr, sc, H, W, 1,0,&result);
	sr = (beeSr > H) ? beeSr - H : 0;
	sc = (beeSc > W) ? beeSc - W : 0;
	for (int i = sr; i < sr + H; i++)
	{
		if (i + H >= MAXSIZE) break;
		for (int j = sc; j < sc + W; j++)
		{
			if (j + W >= MAXSIZE) break;
			DFS(i, j, H, W, 1, 0,&result);
		}
	}
	int ans = 0;
	while (move(++ans)) moveBee(dir);
	beeSr = result.y, beeSc = result.x;
	beeEr = beeSr + H - 1, beeEc = beeSc + W - 1;
	return result;
}
void DFS(int sr, int sc, int H, int W, int n,int length,COORD *result)
{
	if (length >= minLength) return;
	if (n > N)
	{
		result->y = sr, result->x = sc;
		minLength = length;
		for (int i = 1; i <= N; i++)
		{
			finalChk[i] = chk[i];
		}
		return;
	}
	for (int i = 1; i <= N; i++)
	{
		if (chk[i]) continue;
		chk[i] = n;
		int len = abs(bee[i].y - patternloc[n].y + sr) + abs(bee[i].x - patternloc[n].x + sc);
		if (len > length) length = len;
		DFS(sr, sc, H, W, n + 1, length, result);
		chk[i] = 0;
	}
}
bool move(int len)
{
	const int dr[4] = { 0, 1, 0, -1 };
	const int dc[4] = { 1, 0, -1, 0 };
	bool flag = false;
	for (int i = 1; i <= N; i++)
	{
		int ni, nj;
		int goalr = patternloc[finalChk[i]].y;
		int goalc = patternloc[finalChk[i]].x;
		for (int j = 0; j < 4; j++)
		{
			ni = bee[i].y + dr[i];
			nj = bee[i].x + dc[i];
			if (abs(ni - goalr) + abs(nj - goalc) >= abs(bee[i].y - goalr) + abs(bee[i].x - goalc)) continue;
			if (map[ni][nj] == len) continue;
			map[ni][nj] = len;
			bee[i].y = ni, bee[i].x = nj;
			dir[i - 1] = j;
			flag = true;
			break;
		}
	}
	return flag;
}