#include <stdio.h>
#define ROW 1
#define COL 2
#define LEFT 3
#define RIGHT 4
#define SIZE (40000)
#define PATTERN (0xfedcba9876543210)
#define MASK (0x000f)
typedef unsigned long long ull;
typedef unsigned short us;
inline int max(int a, int b) { return a > b ? a : b; }
inline int abs(int a) { return a > 0 ? a : -a; }

union CUBE
{
	ull cube;
	us sh[4];
};
struct RUBIK
{
	CUBE cube;
	int dist;
	int instruction[3];
	RUBIK *prev;
	RUBIK()
	{
		cube.cube = dist = 0;
		prev = 0;
		for (int i = 0; i < 3; i++) instruction[i] = 0;
	}
	void insert(int(*map)[4])
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				cube.sh[i] |= map[i][j] << 4 * j;
			}
		}
	}
	RUBIK rotate(int row, int ord, int num, us *ord_num)
	{
		RUBIK c = *this;
		c.dist = dist + 1;
		c.instruction[0] = row;
		c.instruction[1] = ord + 1;
		c.instruction[2] = num;
		c.prev = this;
		if (row == ROW)
		{
			c.cube.sh[ord] = (cube.sh[ord] << (4 * num)) | (cube.sh[ord] >> (16 - 4 * num));
		}
		else
		{
			us mask = 0xf << (4 * ord);
			int n;
			for (int i = 0; i < 4; i++)
			{
				n = (num + i) % 4;
				c.cube.sh[n] = (cube.sh[n] & (~mask)) | ord_num[i];
			}
		}
		return c;
	}
	int match_cnt()
	{
		int cnt = 0;
		ull mask = 0x000000000000000f;
		for (int i = 0; i < 16; i++)
		{
			if ((cube.cube&(mask<<4*i)) == (PATTERN&(mask << 4 * i))) 
				cnt++;
		}
		return cnt;
	}
	int is_matched() { return cube.cube == PATTERN; }
	bool operator>(RUBIK &r)
	{
		if (cube.cube > r.cube.cube) return true;
		return false;
	}
	bool operator==(RUBIK &r)
	{
		if (cube.cube == r.cube.cube) return true;
		return false;
	}
	void print()
	{
		if (prev->dist)prev->print();
		printf("%d %d %d\n", instruction[0], instruction[1], instruction[2]);
	}
};
struct QUEUE
{
	RUBIK queue[SIZE];
	int wp, rp;
	QUEUE()
	{
		wp = rp = 0;
		for (int i = 0; i < SIZE; i++) queue[i] = RUBIK();
	}
	bool empty() { return wp == rp; }
	bool enqueue(RUBIK r)
	{
		queue[wp++] = r;
		if (wp == SIZE) wp = 0;
		return false;
	}
	RUBIK *dequeue()
	{
		RUBIK *tmp = &queue[rp++];
		if (rp == SIZE) rp = 0;
		return tmp;
	}
};
QUEUE queue;
struct AVL
{
	AVL *par, *left, *right;
	RUBIK rubik;
	int height, diff;
	AVL(AVL *par = 0)
	{
		this->par = par, left = right = 0;
		rubik.cube.cube = 0;
		height = -1, diff = 0;
	}
	~AVL() { delete left, right; }
	bool insert(RUBIK r)
	{
		if (r.match_cnt() < 4 * (r.dist - 3)) return false;
		if (r.is_matched()) return true;
		if (height == -1)
		{
			rubik = r;
			left = new AVL(this);
			right = new AVL(this);
			height = 0;
			queue.enqueue(r);
			return false;
		}
		else if (rubik == r) return false;
		else if (rubik > r)  left->insert(r);
		else right->insert(r);
		update_height();
		return false;
	}

	void update_height()
	{
		height = max(left->height, right->height) + 1;
		diff = abs(left->height - right->height);
		if (diff >= 2) rebalance();
	}
	void rebalance()
	{
		if (left->height > right->height)
		{
			if (left->left->height > left->right->height) left->rotate(RIGHT);
			else
			{
				left->right->rotate(LEFT);
				left->rotate(RIGHT);
			}
		}
		else
		{
			if (right->right->height > right->left->height) right->rotate(LEFT);
			else
			{
				right->left->rotate(RIGHT);
				right->rotate(LEFT);
			}
		}
	}
	void rotate(int dir)
	{
		AVL *grand = par->par;
		AVL *parent = par;
		par = grand;
		if (grand)
		{
			if (grand->left == parent) grand->left = this;
			else grand->right = this;
		}
		switch (dir)
		{
		case LEFT:
			parent->right = left; left->par = parent;
			left = parent; break;
		case RIGHT:
			parent->left = right; right->par = parent;
			right = parent; break;
		}
		parent->par = this;
		parent->update_height();
		update_height();
	}
};
AVL *root;

RUBIK input_data(int(*map)[4]);
RUBIK BFS(RUBIK init);
int main()
{
	int map[4][4];
	root = new AVL();
	RUBIK init = input_data(map);
	RUBIK ans = BFS(init);
	printf("%d\n", ans.dist);
	ans.print();
	return 0;
}
RUBIK input_data(int(*map)[4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			scanf("%d", &map[i][j]);
			map[i][j]--;
		}
	}
	RUBIK init;
	init.insert(map);
	return init;
}
RUBIK BFS(RUBIK init)
{
	queue.enqueue(init);
	while (!queue.empty())
	{
		RUBIK *tmp = queue.dequeue();
		for (int j = 0; j < 4; j++)
		{
			us ord_tmp[4];
			us mask = 0xf << (j * 4);
			for (int r = 0; r < 4; r++)
			{
				ord_tmp[r] = (tmp->cube.sh[r]) & mask;
			}
			for (int k = 1; k < 4; k++)
			{
				for (int i = ROW; i <= COL; i++)
				{
					RUBIK cand = tmp->rotate(i, j, k, ord_tmp);
					if(root->insert(cand)) return cand;
					while (root->par) root = root->par;
				}
			}
		}

	}
	return RUBIK();
}