#ifndef NULL
#define NULL  0
#endif
typedef unsigned long long ull;
inline int max(int a, int b) { return a > b ? a : b; }
#if 01
struct Member {
	int id, frequency;
};
#endif // 0

int memberCnt;
struct member :public Member
{
	member(int id=0, int fre=0) { this->id = id, this->frequency = fre; }
	member(Member &m)
	{
		this->id = m.id, this->frequency = m.frequency;
	}
	bool operator>(member &a)
	{
		if (frequency > a.frequency) return true;
		if (frequency == a.frequency && id > a.id) return true;
		return false;
	}
	bool operator>(Member &a)
	{
		if (frequency > a.frequency) return true;
		if (frequency == a.frequency && id > a.id) return true;
		return false;
	}
};
struct AVL
{
	member M;
	AVL *left, *right;
	int diff, height;
	int nodeNum;
	ull sum;
	AVL()
	{
		M = member();
		left = right = 0;
		diff = 0, height = -1;
		nodeNum = sum = 0;
	}
	~AVL() { delete left; delete  right; }
	AVL *insert(Member obj)
	{
		AVL *node;
		if (height == -1)
		{
			left = new AVL(); right = new AVL();
			M = member(obj);
			height = 0;
			node = this;
			nodeNum = 1; sum = M.frequency;
			memberCnt++;
		}
		else if (M > obj) left = left->insert(obj);
		else right = right->insert(obj);
		update();
		node = rebalance();
		return node;
	}
	void update()
	{
		if (left && right)
		{
			height = max(left->height, right->height) + 1;
			diff = left->height - right->height;
			nodeNum = left->nodeNum + right->nodeNum + 1;
			sum = left->sum + right->sum + M.frequency;
		}
		else
		{
			height = -1; diff = sum = nodeNum = 0;
		}
	}
	AVL *rebalance()
	{
		if (-2 < diff && diff < 2) return this;
		AVL *node;
		if (diff >= 2)  // left > right
		{
			if (left->left->height >= left->right->height) node = rotateLeft();
			else
			{
				left = left->rotateRight();
				node = rotateLeft();
			}
		}
		else
		{
			if (right->right->height >= right->left->height) node = rotateRight();
			else
			{
				right = right->rotateLeft();
				node = rotateRight();
			}
		}
		update();
		return node;
	}
	AVL *rotateLeft()
	{
		AVL *child = left;
		left = child->right;
		child->right = this;
		update();
		child->update();
		return child;
	}
	AVL *rotateRight()
	{
		AVL *child = right;
		right = child->left;
		child->left = this;
		update();
		child->update();
		return child;
	}
	AVL *deleteNode(int order,int *value)
	{
		AVL *node;
		if (order == left->nodeNum + 1)
		{
			if (left->height == -1 || right->height == -1)
				node = deleteLeafNode(value);
			else
			{
				AVL *replaceNode = findReplaceNode();
				member tmp = M;
				M = replaceNode->M, replaceNode->M = tmp;
				left=left->deleteNode(left->nodeNum,value);
			}
		}
		else if (order < left->nodeNum + 1) left = left->deleteNode(order,value);
		else right = right->deleteNode(order - left->nodeNum -1,value);
		update();
		node = rebalance();
		return node;
	}
	AVL *findReplaceNode()
	{
		AVL *cand = left;
		while (cand->right->height > -1) cand = cand->right;
		return cand;
	}
	AVL *deleteLeafNode(int *value)
	{
		AVL *node;
		if (left->height == -1 && right->height == -1)
		{
			delete left, right;
			left = right = 0;
			*value += M.id;
			M = member();
			node = this;
		}
		else
		{
			AVL *tmp = 0;
			if (left->height == -1)
			{
				tmp = right; delete left;
			}
			else
			{
				tmp = left; delete right;
			}
			right = tmp->right, left = tmp->left;
			*value += M.id;
			M = member(tmp->M);
			tmp->right = tmp->left = 0;
			delete tmp;
			node = this;
		}
		memberCnt--;
		return node;
	}
	int searchFirstNSum(int order,int sum,int odd,int *medianNum)
	{
		if (left->nodeNum + 1 == order)
		{
			if (odd) *medianNum = M.frequency;
			return left->sum + sum + (1^odd) * M.frequency;
		}
		else if (order < left->nodeNum + 1)
		{
			return left->searchFirstNSum(order, sum, odd,medianNum);
		}
		else
		{
			return right->searchFirstNSum(order - left->nodeNum - 1, 
				sum + left->sum + M.frequency, odd,medianNum);
		}
	}

#if 0
	void display(int level)
	{
		if (right->height > -1) right->display(level + 1);
		for (int i = 0; i < level; i++) printf("        ");
		printf("%d\n", M.frequency);
		if (left->height > -1) left->display(level + 1);
	}
#endif // 0

};
AVL *root;


#if 0
void print()
{
	printf("-----------------------------------\n");
	root->display(0);
}
#endif // 0

void userInit(int memberCount, Member members[]) {
	delete root;
	memberCnt = 0;
	root = new AVL();
	for (int i = 0; i < memberCount; i++)
	{
		root = root->insert(members[i]);
		//print();
	}
}
void addMember(Member obj) {
	root = root->insert(obj);
	//print();
}

int removeMembers(int mode) {
	int odd = memberCnt & 1;
	int even = 1 - odd;
	int median = memberCnt / 2 + odd;
	int ord;
	int totValue = 0;
	switch (mode)
	{
	case 0: ord = 1; break;
	case 1: ord = median; break;
	case 2: ord = memberCnt; break;
	}
	root = root->deleteNode(ord,&totValue);
	//print();
	if (even && mode == 1)
	{
		root = root->deleteNode(ord,&totValue);
		//print();
	}
	return totValue;
}

void getSum(int sum[]) {
	int total = root->sum;
	int odd = memberCnt & 1;
	int median = memberCnt / 2 + odd;
	int medianNum = 0;
	sum[0] = root->searchFirstNSum(median,0,odd,&medianNum);
	sum[1] = total - sum[0] - medianNum;
}