#ifndef NULL
#define NULL  0
#endif
typedef long long ll;
inline int max(int a, int b) { return a > b ? a : b; }
#if 0
struct Member {
	int id, frequency;
};
#endif // 0

int N;

struct AVL
{
	ll num;
	int nodenum;
	ll freqsum;
	int height;
	AVL *left, *right;
	AVL()
	{
		num = nodenum = 0;
		height = -1;
		left = right = 0;
		freqsum = 0;
	}
	~AVL() { delete left, right; }
	AVL *insert(long long n)
	{
		if (height == -1)
		{
			num = n;
			left = new AVL(), right = new AVL();
			freqsum += (ll)num / 100000;
		}
		else if (n < num) left = left->insert(n);
		else right = right->insert(n);
		updateHeight();
		updatenodenum();
		updateSum();
		AVL *node = rebalance(left->height - right->height);
		return node;
	}

	AVL *rebalance(int diff)
	{
		if (-2 < diff && diff < 2) return this;
		AVL *node;
		if (diff >= 2)
		{
			if (left->left->height >= left->right->height)
			{
				node = rotateRight();
			}
			else
			{
				left = left->rotateLeft();
				node = rotateRight();
			}
		}
		else
		{
			if (right->right->height >= right->left->height)
			{
				node = rotateLeft();
			}
			else
			{
				right = right->rotateRight();
				node = rotateLeft();
			}
		}
		return node;
	}
	AVL *rotateRight()
	{
		AVL *child = left;
		left = child->right;
		child->right = this;
		updateHeight();
		updatenodenum();
		updateSum();
		child->updateHeight();
		child->updatenodenum();
		child->updateSum();
		return child;
	}
	AVL *rotateLeft()
	{
		AVL *child = right;
		right = child->left;
		child->left = this;
		updateHeight();
		updatenodenum();
		updateSum();
		child->updateHeight();
		child->updatenodenum();
		child->updateSum();
		return child;
	}
	int findsum(int n, int sum)
	{
		if (left->nodenum + 1 == n) return sum + left->freqsum + num / 100000;
		else if (left->nodenum + 1 < n) return right->findsum(n - left->nodenum - 1, sum + left->freqsum + num / 100000);
		else return left->findsum(n, sum);
	}
	int findval(int n)
	{
		if (left->nodenum + 1 == n) return num / 100000;
		else if (left->nodenum + 1 < n) return right->findval(n - left->nodenum - 1);
		else return left->findval(n);
	}
	AVL *deleteNode(int n, int *ans)
	{
		AVL *node;
		if (left->nodenum + 1 == n)
		{
			if (left->height > -1 && right->height > -1)
			{
				AVL *tmp = getReplacement();
				swap(this->num, tmp->num);
				left = left->deleteNode(left->nodenum, ans);
			}
			else node = deleteLeaf(ans);
		}
		else if (left->nodenum + 1 < n) right = right->deleteNode(n - left->nodenum - 1, ans);
		else left = left->deleteNode(n, ans);
		if (height > -1)
		{
			updateHeight();
			updatenodenum();
			updateSum();
			node = rebalance(left->height - right->height);
		}
		return node;
	}
	AVL *deleteLeaf(int *ans)
	{
		*ans += num % 100000;
		if (!height)
		{
			num = 0;
			nodenum = 0;
			delete left, right;
			left = right = 0;
			height = -1;
			freqsum = 0;
		}
		else
		{
			AVL *tmp;
			if (left->height > -1)
			{
				tmp = left;
				delete right;
			}
			else
			{
				tmp = right;
				delete left;
			}

			left = tmp->left, right = tmp->right;
			num = tmp->num;
			nodenum = tmp->nodenum;
			freqsum = tmp->freqsum;
			tmp->left = 0, tmp->right = 0;
			delete tmp;
		}
		return this;
	}
	AVL *getReplacement()
	{
		AVL *tmp = left;
		while (tmp->right->height>-1) tmp = tmp->right;
		return tmp;
	}
	void updateHeight() { height = max(left->height, right->height) + 1; }
	void updatenodenum() { nodenum = left->nodenum + right->nodenum + 1; }
	void updateSum() { freqsum = left->freqsum + right->freqsum + num / 100000; }
	void swap(long long &a, long long &b) { long long tmp = a; a = b, b = tmp; }

#if 0
	void display(int level)
	{
		if (right->height > -1) right->display(level + 1);
		for (int i = 0; i < level; i++)
		{
			printf("          ");
		}
		printf("%lld\n", num);
		if (left->height>-1) left->display(level + 1);
	}
#endif // 0


} *root;

#if 0
int chk[100000 + 10];

int main()
{
	int T = 20;
	while (T--)
	{
		for (int i = 0; i < 100000 + 10; i++) chk[i] = 0;
		freqsum = 0;
		N = rand() % 100000;
		M = rand() % 100000 + 1;
		int mode, id, freq;
		int ans;
		root = new AVL();
		for (int i = 0; i < M; i++)
		{
			mode = rand() % 4;
			do { id = rand() % 100000; } while (chk[i]);
			freq = rand() % 20001;
			switch (mode)
			{
			case 0: //scanf("%d %d", &id, &freq);
				root = root->insert(freq * 100000 + id);
				//root->display(0); 
				break;
			case 1:
				if (root->nodenum < 1) break;
				root->deleteNode(1, &ans);
				printf("%d\n", ans % 100000);
				//root->display(0); 
				break;
			case 2:
				if (root->nodenum < 1) break;
				root->deleteNode(root->nodenum, &ans);
				printf("%d\n", ans % 100000);
				//root->display(0); 
				break;
			case 3:
				if (root->nodenum <= 2) printf("0\n");
				else printf("%d\n", freqsum -
					root->findNode(1) / 100000 - root->findNode(root->nodenum) / 100000);
			}
		}
	}
	return 0;
}
#endif // 0






/// === submit area start ===



void addMember(Member obj) {
	root = root->insert((long long)obj.frequency * 100000 + obj.id);
	//printf("------------------------------------\n");
	//root->display(0);
}



int removeMembers(int mode) {
	int ans = 0;
	switch (mode)
	{
	case 0: if (root->nodenum < 1) break;
		root = root->deleteNode(1, &ans); break;
	case 2: if (root->nodenum < 1) break;
		root = root->deleteNode(N, &ans); break;
	case 1:
		for (int i = 0; i <= ((N & 1) ^ 1); i++)
		{
			root = root->deleteNode(N / 2+(N&1), &ans);
		}
	}
	//printf("------------------------------------\n");
	//root->display(0);
	return ans;
}



void getSum(int sum[]) {
	sum[0] = root->findsum(N / 2, 0);
	sum[1] = root->freqsum - sum[0];
	if (N & 1) sum[1] -= root->findval(N / 2 + 1);
}



void userInit(int memberCount, Member members[]) {
	delete root;
	root = new AVL();
	N = memberCount;
	for (int i = 0; i < N; i++)
	{
		addMember(members[i]);
	}
}
