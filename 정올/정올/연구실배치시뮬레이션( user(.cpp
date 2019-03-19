enum DIR { left, right };
inline int max(int a, int b) { return a > b ? a : b; }
inline int abs(int a) { return a > 0 ? a : -a; }
int dirow[] = { -1,0,1,0 };
int dicol[] = { 0,1,0,-1 };
int N;
static int chk[4][52][52];
struct LAB
{
	int id;
	int sr, sc, er, ec;
	int evr, evc;
	LAB(int id = 0, int sr = 0, int sc = 0, int height = 0, int width = 0, int evr = 0, int evc = 0)
	{
		this->id = id;
		this->sr = sr, this->sc = sc;
		er = sr + height - 1, ec = sc + width - 1;
		this->evr = evr, this->evc = evc;
	}
	LAB operator+(LAB a)
	{
		LAB tmp = a;
		tmp.id += id;
		tmp.sr += sr, tmp.sc += sc;
		tmp.er += er, tmp.ec += ec;
		tmp.evr += evr, tmp.evc += evc;
		return tmp;
	}
};
LAB *list[1001];
struct MAP
{
	int map[52][52];
	MAP(int N)
	{
		for (int i = 0; i <= N + 1; i++)
			for (int j = 0; j <= N + 1; j++) map[i][j] = 0;
	}
	void insert(LAB lab)
	{
		for (int i = lab.sr; i <= lab.er; i++)
		{
			for (int j = lab.sc; j <= lab.ec; j++) map[i][j] = lab.id;
		}
		map[lab.sr - 1][lab.sc - 1] = map[lab.sr - 1][lab.ec + 1] =
			map[lab.er + 1][lab.sc - 1] = map[lab.er + 1][lab.ec + 1] = -1;
	}
};
MAP *map;

struct ROBOT
{
	int dir, dist;
	int r, c;
	int right_r, right_c;
	bool corner;
	ROBOT(int r = 0, int c = 0, int dist = 0, int dir = 0, bool corner = false)
	{
		this->r = r, this->c = c;
		this->dir = dir, this->dist = dist;
		right_r = r + dirow[(dir + 1) % 4], right_c = c + dicol[(dir + 1) % 4];
		this->corner = corner;
	}
};
struct QUEUE
{
	int wp, rp;
	ROBOT queue[20000];
	QUEUE() { wp = rp = 0; }
	void enqueue(ROBOT r, int n)
	{
		if (chk[r.dir][r.r][r.c] == n) return;
		if (r.r < 1 || r.r>N || r.c < 1 || r.c>N) return;
		if (map->map[r.r][r.c] > 0) return;

		if (map->map[r.right_r][r.right_c] <= 0)
		{
			if (r.corner) return;
			else
			{
				if (map->map[r.r][r.c] == -1) r.corner = true;
				else return;
			}
		}
		else r.corner = false;
		chk[r.dir][r.r][r.c] = n;
		queue[wp++] = r;
	}
	ROBOT &dequeue()
	{
		return queue[rp++];
	}
	bool empty() { return wp == rp; }
	void clear() { wp = rp = 0; }
};
QUEUE queue;
int ord;
void initUser(int tableSize) {
	N = tableSize;
	delete map;
	map = new MAP(tableSize);
	for (int i = 0; i <= 1000; i++)
	{
		if (!list[i]) continue;
		delete list[i];
		list[i] = 0;
	}
}
int BFS(LAB *s, LAB *e, int ord);
void makeLabRoom(int id, int sr, int sc, int h, int w, int vr, int vc) {
	list[id] = new LAB(id, sr, sc, h, w, vr, vc);
	map->insert(*list[id]);
}

int getShortestPath(int s, int e) {
	queue.clear();
	ord++;
	LAB *start = list[s];
	LAB *end = list[e];
	return BFS(start, end, ord);
}
int BFS(LAB *s, LAB *e, int ord)
{
	int r, c, i;
	for (i = 0; i < 4; i++)
	{
		r = s->evr + dirow[i];
		c = s->evc + dicol[i];
		if (map->map[r][c] > 0) continue;
		else break;
	}
	queue.enqueue(ROBOT(r, c, 1, (i + 1) % 4), ord);
	while (!queue.empty())
	{
		ROBOT tmp = queue.dequeue();
		if (tmp.right_r == e->evr && tmp.right_c == e->evc) return tmp.dist + 1;
		if (map->map[tmp.r][tmp.c] == -1)
		{
			for (int i = 0; i < 4; i++)
			{
				queue.enqueue(ROBOT(tmp.r, tmp.c, tmp.dist, i), ord);
			}
		}
		queue.enqueue(ROBOT(tmp.r + dirow[tmp.dir], tmp.c + dicol[tmp.dir], tmp.dist + 1, tmp.dir, tmp.corner), ord);
	}
	return -1;
}