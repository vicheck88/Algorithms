#include <stdio.h>
#define MAXSIZE 500
#define DOWN 1
#define UP -1
char map[MAXSIZE + 1][MAXSIZE + 1];
int chk[MAXSIZE + 1][MAXSIZE + 1];
int R, C;
int dicol[] = { -1,1 };
int sr, sc, er, ec;
struct DATA
{
	int r, c, dir, cnt;
	DATA() { r = c = dir = cnt = 0; }
	void insert(int r, int c, int dir, int cnt)
	{
		this->r = r, this->c = c, this->dir = dir, this->cnt = cnt;
	}
	void clear() { r = c = dir = cnt = 0; }
	bool operator>(DATA &n)
	{
		if (cnt < n.cnt) return true;
		return false;
	}
};
void swap(DATA &a, DATA &b)
{
	DATA tmp = a; a = b, b = tmp;
}
struct HEAP
{
	DATA *heap;
	int last;
	const int first = { 1 };
	HEAP(int n) { last = 0; heap = new DATA[n+1](); }
	void enqueue(int r,int c,int dir,int cnt)
	{
		chk[r][c] = 1;
		//map[r][c] = 'p';
		heap[++last].insert(r, c, dir, cnt);
		int n = last;
		while (n/2 && heap[n]>heap[n/2])
		{
			swap(heap[n], heap[n / 2]);
			n /= 2;
		}
	}
	DATA dequeue()
	{
		DATA tmp = heap[first];
		swap(heap[first], heap[last]);
		heap[last--].clear();
		for (int i = first; heap[i * 2].dir;)
		{
			int idx;
			if (heap[i*2+1].dir && heap[i*2+1]>heap[i*2]) idx = i * 2 + 1;
			else idx = i * 2;
			if (heap[idx] > heap[i]) swap(heap[idx], heap[i]);
			else break;
			i = idx;
		}
		return tmp;
	}
	bool empty() { return last == 0; }
};
HEAP *heap;
void input();
int BFS();
int main()
{
	input();
	printf("%d", BFS());
	delete heap;
	return 0;
}
void input()
{
	scanf("%d %d", &R, &C);
	heap = new HEAP(R*C);
	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			scanf(" %c", &map[i][j]);
			if (map[i][j] == 'C')
			{
				sr = i, sc = j;
			}
			else if (map[i][j] == 'D')
			{
				er = i, ec = j;
			}
		}
	}
}
int go_down(int r, int c,int dir)
{
	while (0 <= r && r < R)
	{
		if (map[r + dir][c] != '#') r += dir;
		else break;
		if (map[r][c] == 'D') return -2;
	}
	if (r == -1 || r == R) return -1;
	return r;
}
int BFS()
{
	int dir = DOWN;
	int nr, nc;
	nr = go_down(sr, sc, dir);
	if (nr == -2) return 0;
	if (nr == -1) return -1;
	heap->enqueue(nr, sc, dir, 0);
	while (!heap->empty())
	{
		DATA tmp = heap->dequeue();
		if (tmp.r == er && tmp.c == ec) return tmp.cnt;
		//중력 변환
		dir = tmp.dir*-1;
		nr = go_down(tmp.r, tmp.c, dir);
		if (nr == -2) return tmp.cnt + 1;
		if (nr != -1 && chk[nr][tmp.c] == 0 && map[nr][tmp.c]=='.')
			heap->enqueue(nr, tmp.c, dir, tmp.cnt + 1);

		for (int k = 0; k < 2; k++)
		{
			nc = tmp.c + dicol[k];
			nr = tmp.r;
			if (0 > nc || nc >= C) continue;
			if (chk[nr][nc] || map[nr][nc] == '#') continue;
			if (map[nr][nc] == 'D') return tmp.cnt;
			nr = go_down(tmp.r, nc, tmp.dir);
			if (nr == -2) return tmp.cnt;
			if (nr < 0) continue;

			if (chk[nr][nc] || map[nr][nc] == '#') continue;
			heap->enqueue(nr, nc, tmp.dir, tmp.cnt);
		}
	}
	return -1;
}