#include <stdio.h>
#define INF 0x7f123456
int N;
int dirow[] = { -1,0,1,0 };
int dicol[] = { 0,1,0,-1 };
int map[101][101];
int chk[101][101];
struct DATA
{
	int r, c;
	DATA(int r=0,int c=0) { this->r = r, this->c = c; }
};
struct QUEUE
{
	int wp, rp;
	DATA queue[20000];
	QUEUE()
	{
		for (int i = 0; i < 20000; i++) queue[i] = DATA();
		wp = rp = 0;
	}
	void clear() { wp = rp = 0; }
	bool empty() { return wp == rp; }
	void enqueue(DATA r)
	{
		if (r.r < 0 || r.r >= N || r.c < 0 || r.c >= N) return;
		if (chk[r.r][r.c]) return;
		chk[r.r][r.c] = 1;
		queue[wp++] = r;
	}
	DATA &dequeue()
	{
		return queue[rp++];
	}
};
QUEUE queue;
void input();
int solve();
bool BFS(int cost);
int map_min = INF;
int map_max;
int main()
{
	input();
	printf("%d\n", solve());
	return 0;
}
void input()
{
	scanf("%d", &N);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			scanf("%d", &map[i][j]);
			if (map_max < map[i][j]) map_max = map[i][j];
			if (map_min > map[i][j]) map_min = map[i][j];
		}
	}
}
int solve()
{
	int ans = -1;
	int s = 0, e = map_max-map_min, m;
	while (s <= e)
	{
		m = (s + e) / 2;
		if (BFS(m))
		{
			ans = m;
			e = m - 1;
		}
		else s = m + 1;
	}
	return ans;
}
bool BFS(int cost)
{
	for (int k = map_min; k <= map_max; k++)
	{
		queue.clear();
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (map[i][j] >= k && map[i][j] <= k + cost) chk[i][j] = 0;
				else chk[i][j] = 1;
			}
		}
		if (chk[0][0]) continue;
		queue.enqueue(DATA(0, 0));
		while (!queue.empty())
		{
			DATA tmp = queue.dequeue();
			if (tmp.r == N - 1 && tmp.c == N - 1) return true;
			int ni, nj;
			for (int r = 0; r < 4; r++)
			{
				ni = tmp.r + dirow[r];
				nj = tmp.c + dicol[r];
				queue.enqueue(DATA(ni,nj));
			}
		}
	}
	return false;
}
