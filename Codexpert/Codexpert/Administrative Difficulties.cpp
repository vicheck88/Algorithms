#include <stdio.h>
#define LEFT 1
#define RIGHT 2
typedef unsigned long long ull;
int T;
int N, M;

inline int get_hash(char *p)
{
	ull hash = 5381;
	for (int i = 0; p[i]; i++) hash = hash * 33 + p[i];
	return hash % 1000;
}
inline int strcmp(char *p, char *q)
{
	int i;
	for (i = 0; p[i] != 0 && q[i] != 0; i++)
	{
		if (p[i] != q[i]) break;
	}
	return p[i] - q[i];
}
inline void strcpy(char *p, char *q)
{
	while (*p++ = *q++);
}
inline int abs(int x) { return x > 0 ? x : -x; }
inline int max(int a, int b) { return a > b ? a : b; }

struct DATA
{
	int time;
	char name[41];
	char cmd;
	char car[41];
	int num;
	DATA(int time = 0, char *name = 0, char cmd = 0)
	{
		num = 0, car[0] = 0;
		this->time = time, this->cmd = cmd;
		if (name)strcpy(this->name, name);
		else this->name[0] = 0;
	}
};
DATA *data;
struct CAR
{
	int hash;
	char name[41];
	bool possible;
	int price, rent, oil;
	CAR *next;
	CAR(char *p = 0, int price = 0, int  rent = 0, int oil = 0, CAR *node = 0)
	{
		if (p) strcpy(name, p);
		this->price = price, this->rent = rent, this->oil = oil;
		this->possible = true;
		if (p) hash = get_hash(name);
		else hash = 0;
		if (node) node->next = this;
		next = 0;
	}
	~CAR() { delete next; }
};
struct LIST
{
	CAR *head, *tail;
	LIST() { head = tail = new CAR(); }
	~LIST() { delete head; }
	void insert(char *p, int price, int  rent, int oil)
	{
		tail = new CAR(p, price, rent, oil, tail);
	}
	CAR *lookup(char *p)
	{
		CAR *cur = head;
		for (; cur; cur = cur->next)
		{
			if (strcmp(cur->name, p) == 0) break;
		}
		if (cur) return cur;
		return 0;
	}
};
LIST *list[1000];
struct AVL
{
	AVL *par, *left, *right;
	int height, diff; //트리의 구성요소

	CAR *car;
	bool consistency;
	int total;
	char name[41];
	AVL(char *p = 0, AVL *par = 0, AVL *nil = 0)
	{
		car = 0;
		this->par = par, left = right = nil;
		height = -1, diff = 0;
		consistency = true;
		total = 0;
		if (p) strcpy(name, p);
		else name[0] = 0;
	}
	~AVL()
	{
		if (left && left->height > -1) delete left;
		if (right && right->height > -1) delete right;
	}

	CAR *find_car(char *car)
	{
		int hash = get_hash(car);
		return list[hash]->lookup(car);
	}
	void update(DATA &data, AVL *nil)
	{
		int res = strcmp(data.name, name);
		if (res == 0)
		{
			if (consistency == false) return;
			switch (data.cmd)
			{
			case 'p': borrow_car(data); break;
			case 'a': accident(data); break;
			case 'r': return_car(data); break;
			}
		}
		else if (res < 0)
		{
			if (left == nil) { left = new AVL(data.name, this, nil); }
			left->update(data, nil);
		}
		else
		{
			if (right == nil) { right = new AVL(data.name, this, nil); }
			right->update(data, nil);
		}
		update_height();
	}
	void update_height()
	{
		height = max(left->height, right->height) + 1;
		diff = abs(left->height - right->height);
		if (diff >= 2) rebalance();
	}

	void borrow_car(DATA &d)
	{
		//한 명의 첩보원은 최대 하나의 차만을 동시에 대여할 수 있다
		if (car && car!=find_car(d.car)) { consistency = false; return; }
		car = find_car(d.car);
		//if (car->borrow_car() == false) { consistency = false; car = 0; return; }
		total += car->rent;
	}
	void return_car(DATA &d)
	{
		//첩보원은 차를 반납하기 전에 대여를 먼저 한다
		if (car == 0) { consistency = false; return; }
		//if (car->return_car() == false) { consistency = false; return; }
		total += d.num * car->oil;
		car = 0;
	}
	void accident(DATA &d)
	{
		//사고는 첩보원이 차를 사용하던 기간에만 발생한다
		if (car == 0) { consistency = false; return; }
		int tmp = d.num*car->price;
		total += tmp / 100 + (tmp % 100 > 0);
	}

	void rebalance()
	{
		if (left->height > right->height)
		{
			if (left->left->height > left->right->height) left->roatate(RIGHT);
			else
			{
				left->right->roatate(LEFT);
				left->roatate(RIGHT);
			}
		}
		else
		{
			if (right->right->height > right->left->height) right->roatate(LEFT);
			else
			{
				right->left->roatate(RIGHT);
				right->roatate(LEFT);
			}
		}
	}
	void roatate(int dir)
	{
		AVL *grand = par->par;
		switch (dir)
		{
		case LEFT:
			par->right = left; left->par = par;
			left = par; left->par = this;
			left->update_height();
			break;
		case RIGHT:
			par->left = right; right->par = par;
			right = par; right->par = this;
			right->update_height();
			break;
		}
		if (grand)
		{
			if (grand->left == par) grand->left = this;
			else grand->right = this;
		}
		par = grand;
		update_height();
	}
	void print()
	{
		if (height == -1) return;
		left->print();
		//대여된 차는 반드시 반납된다
		if (name[0] == 0) return;
		if (car) consistency = false;
		printf("%s ", name);
		if (consistency == false) { printf("INCONSISTENT\n"); }
		else printf("%d\n", total);
		right->print();
	}
};
AVL *tree;
AVL *nil;
void init();
void car_input();
void person_input();
void solve();
int main()
{
	scanf("%d", &T);
	nil = new AVL();
	while (T--)
	{
		init();
		car_input();
		person_input();
		solve();
	}
	delete nil;
	return 0;
}
void init()
{
	delete tree;
	delete[] data;
	scanf("%d %d", &N, &M);
	data = new DATA[M + 10]();
	for (int i = 0; i < 1000; i++)
	{
		if (list[i])
		{
			delete list[i];
			list[i] = 0;
		}
	}
}
void car_input()
{
	char car[41];
	ull hash;
	int price, rent, oil;
	for (int i = 0; i < N; i++)
	{
		scanf("%s", car);
		hash = get_hash(car);
		if (list[hash] == 0) list[hash] = new LIST();
		scanf("%d %d %d", &price, &rent, &oil);
		list[hash]->insert(car, price, rent, oil);
	}
}
void person_input()
{
	char name[41], cmd;
	int time;
	for (int i = 0; i < M; i++)
	{
		scanf("%d %s %c", &time, name, &cmd);
		data[i] = DATA(time, name, cmd);
		switch (cmd)
		{
		case 'p':scanf("%s", data[i].car); break;
		default: scanf("%d", &data[i].num);
		}
	}
}
void solve()
{
	tree = new AVL(data[0].name, 0, nil);
	tree->update_height();
	for (int i = 0; i < M; i++)
	{
		tree->update(data[i], nil);
		while (tree->par)tree = tree->par;
	}
	tree->print();
}