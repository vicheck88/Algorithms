#include <stdio.h>
int T;
int N, M;
inline void strcpy(char *p, const char *q)
{
	if (q == 0) *p = 0;
	else while (*p++ = *q++);
}
struct DATA
{
	int id, sr, sc, width, height;
	char str[1600];
	DATA(int id = 0, int sr=0, int sc=0, int width = 0, int height = 0, char *str=0)
	{
		this->id = id, this->width = width, this->height = height;
		this->sr = sr, this->sc = sc;
		strcpy(this->str, str);
	}
};
DATA data[10001];
struct NODE
{
	int id;
	char col;
	NODE *next;
	NODE(int id = 0, char col = 0, NODE *n=0)
	{
		this->id = id, this->col = col;
		if (n)
		{
			next = n->next;
			n->next = this;
		}
		else next = 0;
}
	~NODE() { delete next; };
	void deleteNode(int id)
	{
		NODE *cur = this;
		if (!cur) return;
		for (; cur->next; cur=cur->next)
		{
			if (cur->next->id != id) continue;
			NODE *tmp = cur->next;
			cur->next = tmp->next;
			tmp->next = 0;
			delete tmp;
			return;
		}
	}
};
NODE *map[1000][1000];
void insert(DATA d)
{
	char *str = d.str;
	for (int i = d.sr; i < d.sr + d.height; i++)
	{
		for (int j = d.sc; j < d.sc + d.width; j++)
		{
			if (!map[i][j]) map[i][j] = new NODE();
			new NODE(d.id, *str++, map[i][j]);
		}
	}
}
void deleteNode(int id)
{
	int sr = data[id].sr, sc = data[id].sc;
	int w = data[id].width, h = data[id].height;
	for (int i = sr; i <= sr + h; i++)
	{
		for (int j = sc; j <= sc + w; j++)
		{
			map[i][j]->deleteNode(id);
		}
	}
}
void print(int r, int c)
{
	for (int i = r; i < r + 4; i++)
	{
		for (int j = c; j < c + 4; j++)
		{
			if (map[i][j] == 0 || map[i][j]->next==0) printf("0");
			else printf("%c", map[i][j]->next->col);
		}
	}
	printf("\n");
}
void init()
{
	for (int i = 0; i <= 10000; i++)
	{
		data[i] = DATA();
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (map[i][j]) delete map[i][j];
			map[i][j] = 0;
		}
	}
}
int main()
{
	char str[1601];
	int id;
	int sr, sc, w, h;
	int mode;
	scanf("%d", &T);
	while (T--)
	{
		init();
		scanf("%d %d", &N, &M);
		for (int i = 0; i < M; i++)
		{
			scanf("%d", &mode);
			switch(mode)
			{
			case 1:
				scanf("%d %d %d %d %d %s", &id, &sr, &sc, &h, &w, &str);
				data[id] = DATA(id, sr, sc, w, h, str);
				insert(data[id]); break;
			case 2:
				scanf("%d", &id);
				deleteNode(id); data[id] = DATA(); break;
			case 3:
				scanf("%d", &id);
				deleteNode(id);
				insert(data[id]); break;
			case 4:
				scanf("%d %d %d", &id, &sr, &sc);
				deleteNode(id);
				data[id].sr = sr, data[id].sc = sc;
				insert(data[id]); break;
			case 5:
				scanf("%d %d", &sr, &sc);
				print(sr, sc); break;
			}
		}
	}
	return 0;
}