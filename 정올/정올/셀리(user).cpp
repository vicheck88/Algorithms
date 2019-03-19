struct DATA
{
	int r, c;
	DATA() { r = c = 0; }
	void assign(int row, int col) { r = row, c = col; }
} a[10000],b[10000],*now,*next;
int init_time = 10;
int curtime;
int nexttime;
int curnum, nextnum;
int map[3010][3010];
int size;
int dirow[] = { -1,-1,0,1,1,1,0,-1 };
int dicol[] = { 0,1,1,1,0,-1,-1,-1 };

void userInit(int boardSize) {
	size = boardSize;
	now = a, next = b;
	curnum = nextnum = 0;
	curtime += 10;
	init_time = curtime;
}

void toEgg(int r, int c) {
	if (map[r][c] == curtime) return;
	map[r][c] = curtime;
	now[curnum++].assign(r, c);
}

void toImago(int r, int c, int d, int L) {
	if (d == 0) map[r][c] = curtime - 2;
	else if (d == 1)
	{
		for (int i = c; i < c + L && i < size; i++) map[r][i] = curtime - 2;
	}
	else
	{
		for (int i = r; i < r + L && i < size; i++) map[i][c] = curtime - 2;
	}
}
bool checkEgg(int r, int c)
{
	int nr, nc, cnt = 0;
	for (int i = 0; i < 8; i++)
	{
		nr = r + dirow[i], nc = c + dicol[i];
		if (nr < 0 || nr >= size || nc < 0 || nc >= size) continue;
		if (map[nr][nc] == curtime) cnt++;
		if (cnt > 2) return false;
	}
	if (cnt) return true;
	return false;
}
int getEggCount(int tick) {
	nexttime = init_time + tick;
	int nr, nc, i, j;
	for (i = 0, j = 0; i < curnum; i++)
	{
		if (map[now[i].r][now[i].c] == curtime) now[j++] = now[i];
	}
	curnum = j;
	while (curtime < nexttime)
	{
		nextnum = 0;
		for (int i = 0; i < curnum; i++)
		{
			for (int k = 0; k < 8; k++)
			{
				nr = now[i].r + dirow[k];
				nc = now[i].c + dicol[k];
				if (nr < 0 || nr >= size || nc < 0 || nc >= size) continue;
				if (map[nr][nc]<init_time - 2 || map[nr][nc]>curtime - 2) continue;
				if (checkEgg(nr, nc))
				{
					next[nextnum++].assign(nr, nc);
					map[nr][nc] = curtime + 1;
				}
			}
		}
		DATA *tmp = now; now = next, next = tmp;
		curnum = nextnum;
		curtime++;
	}
	return curnum;
}