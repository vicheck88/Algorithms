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
#if 0 //디버그용 출력함수
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
#if 0 // 디버그용 출력함수
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
/// 사용자의 코드를 초기화 한다.
/// 매 테스트 케이스마다 한번 호출 된다.
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
/// PC방에 온 새로운 손님이 회원가입하는 경우 등록하고 기본이용시간을 제공한다.
/// 현재 접속되어 있는 회원수를 반환한다.
/// 기존 회원과 identity가 같은 회원이 입력되지 않으며 최대 50000번 호출 될 수 있다.
int joinGuest(char* identity, char  * password)
{
	root = root->insert(identity, password,list->tail);
	AVL::num++;
	//printf("-------------------------------------\n");
	//root->display();
	//list->display();
	return AVL::num;
}
/// id가 identity인 손님이 로그 아웃을 요청한 경우 처리하는 코드이다.
/// 이미 로그 아웃된 id가 입력될 수 있다.
/// 현재 접속되어 있는 회원수를 반환한다.
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
/// 1. 현재 로그인된 회원이고(identity가 등록되어 있고 로그인됨) password가 맞는다면
///    손님의 이용시간을 baseTick으로 초기화하고 현재 접속되어 있는 회원수를 반환한다.
/// 2. 이미 회원가입이 된 회원이고(identity가 등록됨) password가 맞는다면
///    손님의 이용시간을 baseTick으로 초기화하고 현재 접속되어 있는 회원수를 반환한다.
/// 3. 가입된 회원이 아니거나 password가 맞지 않다면 현재 접속되어 있는 회원수를 반환한다.
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
/// 모든 로그인된 손님의 tick을 1 줄인다.
/// 남은 시간이 0인 사용자를 일괄 로그아웃 시키고 이때 로그아웃된 손님의 수를 반환한다.
int reduceTick()
{
	AVL::logoutnum = 0;
	list->reduceTick();
	//list->display();
	return AVL::logoutnum;
}
/// 현재 접속되어 있는 회원수를 반환한다.
/// 매 테스트 케이스마다  1번 호출된다.
int remainGuest()
{
	return AVL::num;
}