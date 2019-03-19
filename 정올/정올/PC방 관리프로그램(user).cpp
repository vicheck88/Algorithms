#ifndef NULL
#define NULL 0
#endif // NULL
//#include <stdio.h>
typedef enum { LEFT, RIGHT } dir;
inline int max(int a, int b) { return a > b ? a : b; }
inline void strcpy(char *p, const char *q)
{
	while (*p++ = *q++);
}
inline int strcmp(char *p, char *q)
{
	int i;
	for (i = 0; p[i] && q[i] && p[i] == q[i]; i++);
	return p[i] - q[i];
}
struct AVL
{
	static int num;
	static int logoutnum;
	static int baseTick;
	char id[12], pwd[12];
	bool logon;
	int tick;
	AVL *left, *right;
	int height, diff;

	AVL *prev, *next;
	AVL()
	{
		id[0] = pwd[0] = 0;
		logon = false;
		height = -1, diff = 0;
		left = right = 0;
		tick = baseTick;
		prev = next = 0;
	}
	~AVL() { delete left, right; }
	AVL *insert(char *id, char *pwd, AVL *tail=0)
	{
		AVL *node;
		int res = strcmp(this->id, id);
		if (height == -1)
		{
			strcpy(this->id, id); strcpy(this->pwd, pwd);
			logon = true;
			left = new AVL(); right = new AVL();
			node = this;
			
			next = tail;
			prev = tail->prev;
			tail->prev = this;
			prev->next = this;
		}
		else if (res == 0) node = NULL;
		else if (res > 0) left = left->insert(id, pwd, tail);
		else right = right->insert(id, pwd, tail);
		update_height();
		node = rebalance();
		return node;
	}
	AVL *find(char *id)
	{
		if (height == -1) return NULL;
		int res = strcmp(this->id, id);
		if (res == 0)   return this;
		else if (res > 0) left->find(id);
		else right->find(id);
	}
	void update_height()
	{
		height = max(left->height, right->height) + 1;
		diff = left->height - right->height;
	}
	AVL *rebalance()
	{
		AVL *node;
		if (-2 < diff && diff < 2) return this;
		if (diff > 0)
		{
			if (left->left->height >= left->right->height) node = rotate(LEFT);
			else
			{
				left = left->rotate(RIGHT);
				node = rotate(LEFT);
			}
		}
		else
		{
			if (right->right->height >= right->left->height) node = rotate(RIGHT);
			else
			{
				right = right->rotate(LEFT);
				node = rotate(RIGHT);
			}
		}
		return node;
	}
	AVL *rotate(dir d)
	{
		AVL *child;
		switch (d)
		{
		case LEFT:
			child = left;
			left = child->right;
			child->right = this;
			break;
		case RIGHT:
			child = right;
			right = child->left;
			child->left = this;
		}
		update_height();
		child->update_height();
		return child;
	}
	void reduceTick()
	{
		if (left->height > -1) left->reduceTick();
		if (logon)
		{
			tick--;
			if (!tick)
			{
				logon = false;
				AVL::num--;
				AVL::logoutnum++;
			}
		}
		if (right->height > -1) right->reduceTick();
	}
#if 0 //����׿� ����Լ�
	void display(int level = 0)
	{
		if (right->height > -1) right->display(level + 1);
		printf("\n");
		for (int i = 0; i < level; i++) printf("           ");
		printf("%s\n", id);
		if (left->height > -1) left->display(level + 1);
	}
#endif // 0

};
struct LIST
{
	AVL *head, *tail;
	LIST()
	{
		head = new AVL(), tail = new AVL();
		init();
	}
	void init()
	{
		head->next = tail, tail->prev = head;
	}
	void insert(AVL *n)
	{
		n->next = tail;
		n->prev = tail->prev;
		tail->prev = n;
		n->prev->next = n;
	}
	void del(AVL *n)
	{
		n->prev->next = n->next;
		n->next->prev = n->prev;
		n->next = n->prev = 0;
	}
	void reduceTick()
	{
		for (AVL *cur = head->next; cur != tail;)
		{
			AVL *tmp = cur;
			cur = cur->next;
			if (--tmp->tick) continue;
			tmp->logon = false;
			del(tmp);
			AVL::logoutnum++;
			AVL::num--;
		}
	}
#if 0 // ����׿� ����Լ�
	void display()
	{
		printf("-------------------------------\n");
		for (AVL *cur = head->next; cur != tail; cur = cur->next)
		{
			printf("id: %s ", cur->id);
			printf("tick: %d\n", cur->tick);
		}
	}
#endif // 0

};
int AVL::num = 0;
int AVL::logoutnum = 0;
int AVL::baseTick = 0;
/// ������� �ڵ带 �ʱ�ȭ �Ѵ�.
/// �� �׽�Ʈ ���̽����� �ѹ� ȣ�� �ȴ�.
AVL *root;
LIST *list;
void initUser(int baseTick)
{
	AVL::baseTick = baseTick;
	delete root;
	root = new AVL();
	if (!list) list = new LIST();
	else list->init();
	AVL::num = AVL::logoutnum = 0;
}
/// PC�濡 �� ���ο� �մ��� ȸ�������ϴ� ��� ����ϰ� �⺻�̿�ð��� �����Ѵ�.
/// ���� ���ӵǾ� �ִ� ȸ������ ��ȯ�Ѵ�.
/// ���� ȸ���� identity�� ���� ȸ���� �Էµ��� ������ �ִ� 50000�� ȣ�� �� �� �ִ�.
int joinGuest(char* identity, char  * password)
{
	root = root->insert(identity, password,list->tail);
	AVL::num++;
	//printf("-------------------------------------\n");
	//root->display();
	//list->display();
	return AVL::num;
}
/// id�� identity�� �մ��� �α� �ƿ��� ��û�� ��� ó���ϴ� �ڵ��̴�.
/// �̹� �α� �ƿ��� id�� �Էµ� �� �ִ�.
/// ���� ���ӵǾ� �ִ� ȸ������ ��ȯ�Ѵ�.
int logoutGuest(char*identity)
{
	AVL *tmp = root->find(identity);
	if (tmp && tmp->logon)
	{
		tmp->logon = false;
		list->del(tmp);
		AVL::num--;
		//list->display();
	}
	return AVL::num;
}
/// 1. ���� �α��ε� ȸ���̰�(identity�� ��ϵǾ� �ְ� �α��ε�) password�� �´´ٸ�
///    �մ��� �̿�ð��� baseTick���� �ʱ�ȭ�ϰ� ���� ���ӵǾ� �ִ� ȸ������ ��ȯ�Ѵ�.
/// 2. �̹� ȸ�������� �� ȸ���̰�(identity�� ��ϵ�) password�� �´´ٸ�
///    �մ��� �̿�ð��� baseTick���� �ʱ�ȭ�ϰ� ���� ���ӵǾ� �ִ� ȸ������ ��ȯ�Ѵ�.
/// 3. ���Ե� ȸ���� �ƴϰų� password�� ���� �ʴٸ� ���� ���ӵǾ� �ִ� ȸ������ ��ȯ�Ѵ�.
int contactGuest(char*identity, char*password)
{
	AVL *tmp = root->find(identity);
	if (tmp != NULL && strcmp(tmp->pwd, password) == 0)
	{
		tmp->tick = AVL::baseTick;
		if (tmp->logon == false)
		{
			tmp->logon = true; AVL::num++;
			list->insert(tmp);
		}
	}
	return AVL::num;
}
/// ��� �α��ε� �մ��� tick�� 1 ���δ�.
/// ���� �ð��� 0�� ����ڸ� �ϰ� �α׾ƿ� ��Ű�� �̶� �α׾ƿ��� �մ��� ���� ��ȯ�Ѵ�.
int reduceTick()
{
	AVL::logoutnum = 0;
	list->reduceTick();
	//list->display();
	return AVL::logoutnum;
}
/// ���� ���ӵǾ� �ִ� ȸ������ ��ȯ�Ѵ�.
/// �� �׽�Ʈ ���̽�����  1�� ȣ��ȴ�.
int remainGuest()
{
	return AVL::num;
}