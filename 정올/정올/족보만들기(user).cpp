
inline void newstrcpy(char *p, const char *q)
{
	while (*p++ = *q++);
}
inline int newstrlen(const char *p)
{
	int cnt;
	for (cnt = 0; p[cnt]; cnt++);
	return cnt;
}
inline int newstrcmp(const char *p, const char *q)
{
	int cnt;
	for (cnt = 0; p[cnt] && q[cnt] && p[cnt] == q[cnt]; cnt++);
	return p[cnt] - q[cnt];
}
inline int newstrncmp(const char *p, const char *q, int len)
{
	int i;
	for (i = 0; p[i] && q[i] && p[i] == q[i] && i < len; i++);
	if (i == len) return 0;
	return p[i] - q[i];
}

struct SPLAY
{
	char name[13];
	SPLAY *root, *left, *right;
	SPLAY *dir;
	SPLAY(char *name, SPLAY *root = 0)
	{
		this->root = root;
		newstrcpy(this->name, name);
		left = right = 0;
		dir = 0;
	}
	SPLAY(SPLAY *root = 0)
	{
		this->root = root;
		name[0] = 0;
		left = right = 0;
		dir = 0;
	}

	SPLAY *rotateRight()
	{
		SPLAY *child = left;
		left = child->right;
		child->right = this;
		return child;
	}
	SPLAY *rotateLeft()
	{
		SPLAY *child = right;
		right = child->left;
		child->left = this;
		return child;
	}
};
SPLAY *splay(char *p, SPLAY *root)
{
	if (!root || newstrcmp(p, root->name) == 0) return root;
	SPLAY head = SPLAY();
	SPLAY *rightMin = &head;
	SPLAY *leftMax = &head;
	for (;;)
	{
		if (newstrcmp(p, root->name) == 0) break;
		else if (newstrcmp(p, root->name) < 0)
		{
			if (!root->left) break;
			else
			{
				if (newstrcmp(p, root->left->name) < 0)
				{
					root = root->rotateRight();
					if (!root->left) break;
				}
				rightMin->left = root;
				root = root->left;
				rightMin = rightMin->left;
				rightMin->left = 0;
			}
		}
		else
		{
			if (!root->right) break;
			else
			{
				if (newstrcmp(p, root->right->name) > 0)
				{
					root = root->rotateLeft();
					if (!root->right) break;
				}
				leftMax->right = root;
				root = root->right;
				leftMax = leftMax->right;
				leftMax->right = 0;
			}
		}
	}
	rightMin->left = root->right;
	leftMax->right = root->left;
	root->right = head.left;
	root->left = head.right;
	return root;
}
SPLAY *insert(char *p, SPLAY *cur)
{
	if (cur->dir) cur->dir = splay(p, cur->dir);
	SPLAY *tmp = new SPLAY(p, cur);
	if (!cur->dir) cur->dir = tmp;
	else if (newstrcmp(p, cur->dir->name) < 0)
	{
		tmp->left = cur->dir->left;
		cur->dir->left = 0;
		tmp->right = cur->dir;
		cur->dir = tmp;
	}
	else if (newstrcmp(p, cur->dir->name) > 0)
	{
		tmp->right = cur->dir->right;
		cur->dir->right = 0;
		tmp->left = cur->dir;
		cur->dir = tmp;
	}
	else delete tmp;
	return cur->dir;
}
SPLAY *leftMost(SPLAY *root)
{
	SPLAY *tmp = root;
	while (tmp->left) tmp = tmp->left;
	return tmp;
}
SPLAY *rightMost(SPLAY *root)
{
	SPLAY *tmp = root;
	while (tmp->right) tmp = tmp->right;
	return tmp;
}
SPLAY *searchLowerbound(char *p, SPLAY *root)
{
	SPLAY *tmp, *node;
	node = leftMost(root);
	tmp = root;
	int len = newstrlen(p);
	while (tmp)
	{
		if (newstrncmp(p, tmp->name, len) > 0)
		{
			node = tmp;
			tmp = tmp->right;
		}
		else tmp = tmp->left;
	}
	return node;
}
SPLAY *searchUpperbound(char *p, SPLAY *root)
{
	SPLAY *tmp, *node;
	node = rightMost(root);
	tmp = root;
	int len = newstrlen(p);
	while (tmp)
	{
		if (newstrncmp(p, tmp->name, len) < 0)
		{
			node = tmp;
			if (tmp->left) tmp = tmp->left;
			else break;
		}
		else
		{
			if (tmp->right) tmp = tmp->right;
			else break;
		}
	}
	return node;
}
SPLAY *setRange(char *p, SPLAY *root)
{
	SPLAY *lower = searchLowerbound(p, root);
	SPLAY *upper = searchUpperbound(p, root);
	root = splay(upper->name, root);
	root->left = splay(lower->name, root->left);
	return root;
}
SPLAY *searchNode(char *p, SPLAY *root)
{
	SPLAY *tmp, *node;
	tmp = root; node = 0;
	int len = newstrlen(p);
	while (tmp)
	{
		if (newstrncmp(p, tmp->name, len) > 0) tmp = tmp->right;
		else
		{
			node = tmp;
			tmp = tmp->left;
		}
	}
	return node;
}

int countDir(char *p, SPLAY *root)
{
	int cnt = 0;
	if (!root) return 0;
	cnt += countDir(p, root->left) + countDir(p, root->right) +
		(newstrncmp(p, root->name, newstrlen(p)) == 0 ? 1 : 0);
	if (root->dir) cnt += countDir(p, root->dir);
	return cnt;
}

int deleteAll(SPLAY *root)
{
	if (!root) return 0;
	int n = 0;
	if (root->left) n += deleteAll(root->left);
	if (root->right) n += deleteAll(root->right);
	if (root->dir) n += deleteAll(root->dir);
	n++;
	delete root;
	return n;
}


SPLAY *root, *cur;
void init_user()
{
	if (!root) root = cur = new SPLAY();
	else if (root->dir)
	{
		deleteAll(root->dir);
		root->dir = 0;
		cur = root;
	}
}

void Add_child(char name[])
{
	cur->dir = insert(name, cur);
}

int Delete(char name[])
{
	if (!cur->dir) return 0;
	int cnt = 0;
	if (newstrcmp(name, "ALL") == 0)
	{
		cnt = deleteAll(cur->dir);
		cur->dir = 0;
	}
	else
	{
		int len = newstrlen(name);
		cur->dir = setRange(name, cur->dir);
		if (newstrncmp(cur->dir->name, name, len) == 0)
		{
			SPLAY *tmp = cur->dir;
			cur->dir = cur->dir->left;
			tmp->left = 0;
			cnt += deleteAll(tmp);
			if (cur->dir)
			{
				if (newstrncmp(cur->dir->name, name, len) == 0)
				{
					cnt += deleteAll(cur->dir);
					cur->dir = 0;
				}
				else
				{
					cnt += deleteAll(cur->dir->right);
					cur->dir->right = 0;
				}
			}
		}
		else
		{
			SPLAY *node = cur->dir->left;
			if (node)
			{
				if (newstrncmp(node->name, name, len) == 0)
				{
					cnt += deleteAll(node);
					cur->dir->left = 0;
				}
				else
				{
					cnt += deleteAll(node->right);
					node->right = 0;
				}
			}
		}
	}
	return cnt;
}

void Move(char name[])
{
	int a = newstrcmp(name, "root");
	int b = newstrcmp(name, "parent");
	int c = newstrcmp(name, "first");
	if (newstrcmp(name, "root") == 0) cur = root;
	else if (newstrcmp(name, "parent") == 0) { if (cur->root) cur = cur->root; }
	else if (newstrcmp(name, "first") == 0)
	{
		if (!cur->dir) return;
		cur = cur->dir;
		while (cur->left) cur = cur->left;
	}
	else
	{
		if (!cur->dir) return;
		SPLAY *node = searchNode(name, cur->dir);
		if (node)
		{
			cur->dir = splay(node->name, cur->dir);
			if (newstrncmp(name, node->name, newstrlen(name)) == 0) cur = cur->dir;
		}
	}
}

int Count(char name[])
{
	if (newstrcmp(name, "ALL") == 0) name[0] = 0;
	return countDir(name, cur->dir);
}