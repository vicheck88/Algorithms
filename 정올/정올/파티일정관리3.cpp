#include <stdio.h>
//#include <stdlib.h>
int idchk[10001];
int T, N;
int nodecnt;
int seg[1600000];
int cumday[1000][12];
inline int days(int y, int m) {
	int d[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (y % 400 == 0 || (y % 4 == 0 && y % 100)) d[2] = 29; /// ¿±≥‚¿Ã∂Û∏È
	return d[m];
}
inline void getdays()
{
	int i, j;
	for (i = 0; i < 1000; i++)
	{
		for (j = 1; j < 12; j++)
		{
			cumday[i][j] = cumday[i][j - 1] + days(i + 2020, j);
		}
		if(i<999) cumday[i][j] = cumday[i][j-1] + 31;
	}
}
inline int convert(int y, int m, int d) { return cumday[y - 2020][m - 1] + d; }
int lastday;
struct SEG
{
	void insert(int s, int e, int node, int k)
	{
		if (s == e)
		{
			seg[node]++;
			return;
		}
		int m = (s + e) / 2;
		if (k <= m) insert(s, m, node * 2, k);
		else insert(m + 1, e, node * 2 + 1, k);
		seg[node] = seg[node * 2] + seg[node * 2 + 1];
	}
	void del(int s, int e, int node, int k)
	{
		if (s == e)
		{
			seg[node]--;
			return;
		}
		int m = (s + e) / 2;
		if (k <= m) del(s, m, node * 2, k);
		else del(m + 1, e, node * 2 + 1, k);
		seg[node] = seg[node * 2] + seg[node * 2 + 1];
	}
	int query(int s, int e, int node, int qs, int qe)
	{
		if (qe < s || e < qs) return 0;
		if (qs <= s && e <= qe) return seg[node];
		int m = (s + e) / 2;
		return query(s, m, node * 2, qs, qe) + query(m + 1, e, node * 2 + 1, qs, qe);
	}
} segment;
struct NODE
{
	int id, day;
	NODE *dayPrev, *dayNext;
	NODE *idPrev, *idNext;
	NODE(int id = 0, int day = 0)
	{
		this->id = id, this->day = day;
		dayPrev = dayNext = idPrev = idNext = 0;
	}
	NODE *suicide(int mode)
	{
		segment.del(1,lastday,1,day);
		if(idPrev) idPrev->idNext = idNext;
		if(idNext) idNext->idPrev = idPrev;
		if(dayPrev) dayPrev->dayNext = dayNext;
		if(dayNext) dayNext->dayPrev = dayPrev;
		if (mode) return idNext;
		else return dayNext;
	}
};
NODE node[1000000+10];
struct LIST
{
	NODE *head;
	LIST() { head = new NODE(); }
	void deleteAll()
	{
		NODE *cur = head;
		while (cur)
		{
			cur = cur->suicide(1);
		}
	}
};
LIST ID[10001];
LIST CAL[400000];

void init()
{
	for (int i = 0; i <= 10000; i++)
	{
		ID[i].head->idNext = 0;
		//idchk[i] = 0;
	}
	for (int i = 0; i <= lastday; i++)
	{
		CAL[i].head->dayNext = 0;
	}
	for (int i = 0; i <= lastday * 4; i++) seg[i] = 0;
	nodecnt = 0;
}
inline void Insert(int id, int date)
{
	segment.insert(1, lastday, 1, date);
	NODE *ref = &node[nodecnt];
	node[nodecnt++] = NODE(id, date);
	NODE *idp = ID[id].head;
	NODE *dayp = CAL[date].head;
	ref->idPrev = idp;
	ref->idNext = idp->idNext;
	if (ref->idNext) ref->idNext->idPrev = ref;
	if (ref->idPrev) ref->idPrev->idNext = ref;

	ref->dayPrev = dayp;
	ref->dayNext = dayp->dayNext;
	if (ref->dayNext) ref->dayNext->dayPrev = ref;
	if (ref->dayPrev) ref->dayPrev->dayNext = ref;
}
void insertDate(int id, int Case, int cnt, int year, int month, int day)
{
	int conv = convert(year, month, day);
	while (cnt--)
	{
		if (year > 3019) break;
		if (conv > lastday) break;
		Insert(id, conv);
		switch (Case)
		{
		case 8: if (++month > 12) { if (++year > 3019) break; else month = 1; }
				conv = convert(year, month, day); break;
		case 9: if (++year > 3019) break;
			conv = convert(year, month, day); break;
		case 10: year += 10; if (year > 3019) break;
			conv = convert(year, month, day); break;
		default: conv += Case;
		}
	}
}
void deleteDate(int date)
{
	if (date > lastday) return;
	NODE *cur = CAL[date].head->dayNext;
	while (cur)
	{
		cur = cur->suicide(0);
	}
	CAL[date].head->dayNext = 0;
}
void deleteId(int id)
{
	NODE *cur = ID[id].head->idNext;
	while (cur)
	{
		cur = cur->suicide(1);
	}
	ID[id].head->idNext = 0;
}
void deleteIdDate(int date)
{
	if (date > lastday) return;
	NODE *cur = CAL[date].head->dayNext;
	int id;
	while (cur)
	{
		id = cur->id;
		cur = cur->suicide(0);
		deleteId(id);
	}
	CAL[date].head->dayNext = 0;
}
#if 01
int main()
{
	getdays();
	lastday = cumday[999][11] + 31;
	int id, mode, year, month, day, Case, cnt;
	int from, to;
	scanf("%d", &T);
	while (T--)
	{
		init();
		scanf("%d", &N);
		while (N--)
		{
			scanf("%d", &mode);
			switch (mode)
			{
			case 1: scanf("%d %d %d", &id, &Case, &cnt);
				scanf("%4d%2d%2d", &year, &month, &day);
				insertDate(id, Case, cnt, year, month, day);
				break;
			case 2: scanf("%d", &Case);
				switch (Case)
				{
				case 2: scanf("%d", &id);
					deleteId(id); break;
				default: scanf("%4d%2d%2d", &year, &month, &day); break;
				}
				if (Case == 1) deleteIdDate(convert(year, month, day));
				else if (!Case) deleteDate(convert(year, month, day));
				break;
			case 3: scanf("%4d%2d%2d", &year, &month, &day);
				from = convert(year, month, day);
				scanf("%4d%2d%2d", &year, &month, &day);
				to = convert(year, month, day);
				printf("%d\n", segment.query(1, lastday, 1, from, to));
				break;
			}
		}
	}
	return 0;
}
#endif // 0

#if 0
int main()
{

	srand(0);
	getdays();
	lastday = cumday[999][11] + 31;
	int id, mode, year, month, day, Case, cnt;
	int from, to;
	T = 10;
	while (T--)
	{
		init();
		N = rand() % 10000 + 1;
		while (N--)
		{
			mode = rand() % 3 + 1;
			cnt = rand() % 100 + 1;
			year = rand() % 1000 + 2020;
			//year = 3019;
			month = rand() % 12 + 1;
			day = days(year, month);
			switch (mode)
			{
			case 1:
				do
				{
					id = rand() % 10000 + 1;
				} while (idchk[id]);
				idchk[id] = 1;
				Case = rand() % 10 + 1;
				insertDate(id, Case, cnt, year, month, day);
				break;
			case 2: Case = rand() % 3;
				switch (Case)
				{
				case 2:
					do
					{
						id = rand() % 10000 + 1;
					} while (idchk[id] == 2);
					if (idchk[id] == 1) idchk[id] = 2;
					deleteId(id);
					break;
				default:
					id = convert(year, month, day);
					if (Case == 1) deleteIdDate(convert(year, month, day));
					else if (!Case) deleteDate(convert(year, month, day));
					break;
				} break;
			case 3:
				from = convert(year, month, day);
				year = rand() % 1000 + 2020;
				month = rand() % 12 + 1;
				day = days(year, month);
				to = convert(year, month, day);
				if (from > to) break;
				printf("%d\n", segment.query(1, lastday, 1, from, to));
				break;
			}
		}
	}
	return 0;
}

#endif // 0
