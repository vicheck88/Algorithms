#include <cstdio>
struct NODE
{
	char ch;
	NODE *prev, *next;
	NODE(char ch = 0) { this->ch = ch; prev = next = 0; }
	NODE(char ch, NODE *n)
	{
		this->ch = ch;
		next = n;
		prev = n->prev;
		n->prev = this;
		prev->next = this;
	}
	~NODE() { delete next; }
	void selfdel()
	{
		prev->next = next;
		next->prev = prev;
		prev = next = 0;
		delete this;
	}
};
struct LIST
{
	NODE *head, *tail, *cur;
	LIST()
	{
		head = new NODE();
		tail = cur = new NODE();
		head->next = tail;
		tail->prev = head;
	}
	~LIST() { delete head; }
	void insert(char ch, NODE *n)
	{
		new NODE(ch, n);
	}
	void del()
	{
		if (cur == head->next) return;
		cur->prev->selfdel();
	}
	void left()
	{
		if (cur == head->next) return;
		cur = cur->prev;
	}
	void right()
	{
		if (cur == tail) return;
		cur = cur->next;
	}
};
LIST *list;
char ans[600000 + 10];
int main()
{
	int N;
	char cmd;
	char ch;
	delete list;
	list = new LIST();
	/*while (1)
	{
		scanf("%c", &ch);
		if (ch=='\n') break;
		list->insert(ch, list->tail);
	}*/
	scanf("%s", ans);
	for (int i = 0; ans[i]; i++)
	{
		list->insert(ans[i], list->tail);
	}
	scanf("%d", &N);
	list->cur = list->tail;
	for (int i = 0; i < N; i++)
	{
		scanf(" %c", &cmd);
		switch (cmd)
		{
		case 'P':scanf(" %c", &ch); list->insert(ch,list->cur); break;
		case 'D':list->right(); break;
		case 'L':list->left(); break;
		case 'B':list->del(); break;
		}
	}
	NODE *cur = list->head->next;
	for (int i = 0; cur != list->tail; cur = cur->next, i++) printf("%c", cur->ch);
	return 0;
}