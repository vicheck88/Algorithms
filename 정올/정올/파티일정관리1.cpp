#include <stdio.h>
int N;
inline int days(int year, int month) {
	int d[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (year % 400 == 0 || (year % 4 == 0 && year % 100)) d[2] = 29; /// À±³âÀÌ¶ó¸é
	return d[month];
}
struct DATE
{
	int year, month, day;
	DATE(int y = 0, int m = 0, int d = 0) { year = y, month = m, day = d; }
};
struct NODE
{
	DATE date;
	int id;
	NODE *next, *prev, *link;
	NODE() { id = 0; date = DATE(); next = prev = link = 0; }
	NODE(int id, DATE date, NODE *n)
	{
		this->id = id, this->date = date;
		next = n, prev = n->prev;
		prev->next = this;
		next->prev = this;
		link = 0;
	}
	~NODE() { delete next; }
	void selfdel()
	{
		next->prev = prev;
		prev->next = next;
		prev = next = 0;
		delete this;
	}
};
struct LIST
{
	int num;
	bool modeflag;
	NODE *head, *tail;
	LIST()
	{
		num = 0;
		head = new NODE(), tail = new NODE();
		head->next = tail, tail->prev = head;
		modeflag = 0;
	}
	void insert(int id,DATE d)
	{
		new NODE(id, d, tail);
		num++;
	}
	void deleteDate(NODE *n)
	{
		n->selfdel();
		num--;
	}
};
LIST *ID[10001];
LIST *CAL[101][13][32];

inline DATE convert(int date)
{
	int y = date / 10000;
	int m = date % 10000 / 100;
	int d = date % 100;
	return DATE(y, m, d);
}
void insertDate(int id, int Case, int cnt, int date)
{
	DATE dat = convert(date);
	int y = dat.year - 2018;
	int m = dat.month;
	int d = dat.day;
	int daylim = days(y+2018, m);
	if (!ID[id]) ID[id] = new LIST();
	while (cnt--)
	{
		if (!CAL[y][m][d]) CAL[y][m][d] = new LIST();
		ID[id]->insert(id, dat);
		CAL[y][m][d]->insert(id, dat);

		ID[id]->tail->prev->link = CAL[y][m][d]->tail->prev;
		CAL[y][m][d]->tail->prev->link = ID[id]->tail->prev;

		switch (Case)
		{
		case 8:m++; break;
		case 9:y++; break;
		default:d += Case; break;
		}
		if (d > daylim)
		{
			m++; d -= daylim;
			if (m <= 12) daylim = days(y+2018, m);
		}
		if (m > 12) { m = 1; y++; daylim = 31; }
		if (y + 2018 > 2117) break;
		dat.year = y + 2018, dat.month = m, dat.day = d;
	}
}
void deleteDate(int date)
{
	DATE d = convert(date);
	LIST *list = CAL[d.year - 2018][d.month][d.day];
	if (!list) return;
	for (NODE *cur = list->head->next; cur != list->tail;)
	{
		NODE *tmp = cur;
		ID[tmp->id]->num--;
		ID[tmp->id]->modeflag = true;
		tmp->link->selfdel();
		cur = cur->next;
		list->deleteDate(tmp);
	}
}
void deleteID(int id)
{
	ID[id]->num = 0;
	for (NODE *cur = ID[id]->head->next; cur != ID[id]->tail;)
	{
		NODE *tmp = cur;
		LIST *list = CAL[tmp->date.year - 2018][tmp->date.month][tmp->date.day];
		list->deleteDate(tmp->link);
		cur = cur->next;
		tmp->selfdel();
	}
}

void deleteProcess(int mode, int d)
{
	if (mode == 0) deleteDate(d);
	else if (mode == 2) deleteID(d);
	else
	{
		DATE date = convert(d);
		LIST *list = CAL[date.year-2018][date.month][date.day];
		if (!list) return;
		for (NODE *cur = list->head->next; cur != list->tail;)
		{
			NODE *tmp = cur;
			cur = cur->next;
			//if (!ID[tmp->id]->modeflag) deleteID(tmp->id);
			deleteID(tmp->id);
		}
	}
}
void init()
{
	for (int i = 0; i < 10001; i++)
	{
		if (ID[i])
		{
			delete ID[i];
			ID[i] = 0;
		}
	}
	for (int i = 0; i < 101; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			for (int k = 0; k < 32; k++)
			{
				if (CAL[i][j][k])
				{
					delete CAL[i][j][k];
					CAL[i][j][k] = 0;
				}
			}
		}
	}
}
int getReservedDate(int f, int e)
{
	int ans = 0;
	DATE from = convert(f), to = convert(e);
	int fy = from.year, fm = from.month, fd = from.day;
	int ty = to.year, tm = to.month, td = to.day;
	int daylim = days(fy, fm);
	for (;;)
	{
		
		if (CAL[fy - 2018][fm][fd]) ans += CAL[fy - 2018][fm][fd]->num;

		fd++;
		if (fd > daylim)
		{
			fd = 1; fm++;
			if (fm > 12)
			{
				fm = 1; fy++;
			}
			daylim = days(fy, fm);
		}
		if (fy * 10000 + fm * 100 + fd > ty * 10000 + tm * 100 + td) break;
	}
	return ans;
}

int main()
{
	init();
	scanf("%d", &N);
	int mode;
	int id, Case, cnt, date;
	int date1, date2;
	for (int i = 0; i < N; i++)
	{
		scanf("%d", &mode);
		switch (mode)
		{
		case 1: scanf("%d %d %d %d", &id, &Case, &cnt, &date); 
			if (Case == 0) cnt = 1;
			insertDate(id, Case, cnt, date); break;
		case 2: scanf("%d", &Case);
			switch(Case)
			{
			case 0:
			case 1: scanf("%d", &date); deleteProcess(Case, date); break;
			case 2: scanf("%d", &id); deleteProcess(Case, id); break;
			} 
			break;
		case 3: scanf("%d %d", &date1, &date2); 
			printf("%d\n", getReservedDate(date1, date2)); break;
		}
	}
	return 0;
}