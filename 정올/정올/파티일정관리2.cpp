#include <stdio.h>
//#include <stdlib.h>
//int idchk[10001];
int days(int year, int month)
{
	int day[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (year % 400 == 0 || (year % 4 == 0 && year % 100)) day[2] = 29;
	return day[month];
}
int date[100][13];
void convertdate()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 1; j <= 12; j++)
		{
			date[i][j] = date[i][j - 1] + days(i + 2018, j);
		}
		if (i != 99) date[i + 1][0] = date[i][12];
	}
}
int datesum[12 * 31 * 100];
struct NODE
{
	int id, datenum;
	NODE *prev, *next, *link;
	NODE(int i = 0, int d = 0, NODE *p = 0, NODE *n = 0)
	{
		id = i, datenum = d;
		if (p)
		{
			prev = p;
			p->next = this;
		}
		else prev = 0;
		if (n)
		{
			next = n;
			n->prev = this;
		}
		else next = 0;
		link = 0;
	}
	~NODE() { delete next; }
	void suicide()
	{
		prev->next = next;
		if (next) next->prev = prev;
		next = 0;
		delete this;
	}
};
struct LIST
{
	NODE *sp;
	LIST() { sp = new NODE(); }
	~LIST() { delete sp; }
	void insert(int id, int datenum)
	{
		new NODE(id, datenum, sp, sp->next);
	}
	void del(NODE *n)
	{
		datesum[n->datenum]--;
		n->link->suicide();
		n->suicide();
	}
	bool empty() { return sp->next == 0; }
};
LIST *ID[10000 + 10];
LIST *CAL[40000];
void init()
{
	for (int i = 0; i < 10000 + 10; i++)
	{
		if (ID[i])
		{
			delete ID[i];
			ID[i] = 0;
		}
	}
	for (int i = 0; i < 12 * 31 * 100; i++)
	{
		if (CAL[i])
		{
			delete CAL[i];
			CAL[i] = 0;
		}
		datesum[i] = 0;
	}
	//for (int i = 0; i <= 10000; i++) idchk[i] = 0;
}
void addDate(int id, int Case, int cnt, int year, int month, int day)
{
	if (!ID[id]) ID[id] = new LIST();
	int datenum = date[year - 2018][month - 1] + day;
	while (cnt--)
	{
		if (datenum > date[99][12]) return;
		if (!CAL[datenum]) CAL[datenum] = new LIST();

		ID[id]->insert(id, datenum);
		CAL[datenum]->insert(id, datenum);
		datesum[datenum]++;

		ID[id]->sp->next->link = CAL[datenum]->sp->next;
		CAL[datenum]->sp->next->link = ID[id]->sp->next;
		switch (Case)
		{
		case 8:
			if (++month > 12)
			{
				year++; month = 1;
			}
			if (year > 2117) return;
			datenum = date[year - 2018][month - 1] + day;
			break;
		case 9:
			if (++year > 2117) return;
			datenum = date[year - 2018][month - 1] + day;
			break;
		default:
			datenum += Case;
		}
	}
}
void deleteDate(int datenum)
{
	if (datenum > date[99][12]) return;
	if (!CAL[datenum] || CAL[datenum]->empty()) return;
	NODE *cur = CAL[datenum]->sp->next;
	for (; cur;)
	{
		NODE *tmp = cur;
		cur = cur->next;
		CAL[datenum]->del(tmp);
	}
}
void deleteId(int id)
{
	if (!ID[id] || ID[id]->empty()) return;
	NODE *cur = ID[id]->sp->next;
	for (; cur; )
	{
		NODE *tmp = cur;
		cur = cur->next;
		ID[id]->del(tmp);
	}
}
void deleteDateId(int datenum)
{
	if (datenum > date[99][12]) return;
	if (!CAL[datenum] || CAL[datenum]->empty()) return;
	NODE *cur = CAL[datenum]->sp->next;
	for (; cur;)
	{
		NODE *tmp = cur;
		cur = cur->next;
		deleteId(tmp->id);
	}
}
int countDate(int datenum)
{
	int sum = 0;
	if (datenum > date[99][12]) return 0;
	int to = ((datenum + 29) > date[99][12]) ? date[99][12] : datenum + 29;
	for (int i = datenum; i <= to; i++) sum += datesum[i];
	return sum;
}
int main()
{
	int T, N;
	int mode, id;
	int year, month, day;
	int Case, cnt;
	int datenum;
	convertdate();
	scanf("%d", &T);
	while (T--)
	{
		init();
		scanf("%d", &N);
		//N = rand() % 10000 + 1;

		while (N--)
		{
			scanf("%d", &mode);
			//mode = rand() % 3 + 1;
			switch (mode)
			{
			case 1:
				scanf("%d %d %d", &id, &Case, &cnt);
				/*id = rand() % 10000 + 1;
				while (idchk[id]) id = rand() % 10000 + 1;
				idchk[id] = 1;
				Case = rand() % 10 + 1;
				cnt = rand() % 10 + 1;*/

				scanf("%4d%2d%2d", &year, &month, &day);
				/*year = rand() % 100 + 2018;
				month = rand() % 12 + 1;
				day = rand() % days(year, month) + 1;*/

				if (!Case) cnt = 1;
				addDate(id, Case, cnt, year, month, day);
				break;
			case 2:
				scanf("%d", &Case);
				//Case = rand() % 3;

				switch (Case)
				{
				case 0:
					scanf("%4d%2d%2d", &year, &month, &day);
					/*year = rand() % 100 + 2018;
					month = rand() % 12 + 1;
					day = rand() % days(year, month) + 1;*/

					datenum = date[year - 2018][month - 1] + day;
					deleteDate(datenum); break;
				case 1:
					scanf("%4d%2d%2d", &year, &month, &day);
					/*year = rand() % 100 + 2018;
					month = rand() % 12 + 1;
					day = rand() % days(year, month) + 1;*/

					datenum = date[year - 2018][month - 1] + day;
					deleteDateId(datenum); break;
				case 2:
					scanf("%d", &id);
					/*id = rand() % 10000 + 1;
					if (idchk[id]==1) idchk[id] = 2;*/
					deleteId(id); break;
				} break;
			case 3:
				scanf("%4d%2d%2d", &year, &month, &day);
				/*year = rand() % 100 + 2018;
				month = rand() % 12 + 1;
				day = rand() % days(year, month) + 1;*/

				datenum = date[year - 2018][month - 1] + day;
				printf("%d\n", countDate(datenum)); break;
			}
		}
	}
	return 0;
}