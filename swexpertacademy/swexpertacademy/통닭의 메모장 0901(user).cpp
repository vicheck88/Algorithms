inline int mstrlen(char *p)
{
	int i;
	for (i = 0; p[i]; i++);
	return i;
}
inline void mstrcpy(char *p, char *q) { while (*p++ = *q++); }

struct VECT
{
	int num, lim;
	char *str;
	VECT() { num = 0, lim = 64, str = new char[lim](); }
	VECT(char *s)
	{
		lim = 64;
		num = mstrlen(s);
		while (num >= lim) lim *= 2;
		str = new char[lim]();
		mstrcpy(str, s);
	}
	~VECT() { delete[] str; }
	void insert(char p)
	{
		if (num + 2 >= lim)
		{
			lim *= 2;
			char *tmp = new char[lim]();
			mstrcpy(tmp, str);
			delete[] str;
			str = tmp;
		}
		str[num++] = p;
	}
	void insert(int pos, char *s)
	{
		int n = pos + mstrlen(s);
		if (n >= lim) 
		{
			while (n >= lim) lim *= 2;
			lim *= 2;
			char *tmp = new char[lim]();
			mstrcpy(tmp, str);
			delete[] str;
			str = tmp;
		}
		mstrcpy(str + pos, s);
		num = n;
	}
	void del() { str[--num] = 0; if (num * 4 < lim) resetting(); }
	void clear(int pos)
	{
		int i, j;
		for (i = pos, j = 0; str[i]; i++, j++) str[i] = 0;
		num -= j;
	}
	void resetting()
	{
		int orilim = lim;
		if (num == 0) lim = 3;
		else while (4 * num < lim) lim /= 2;
		if (orilim != lim)
		{
			char *tmp = new char[lim]();
			mstrcpy(tmp, str);
			delete[] str;
			str = tmp;
		}
	}
	void copy(int pos, char *s)
	{
		mstrcpy(s, str + pos);
		clear(pos);
	}
};
struct LINE
{
	VECT *str;
	LINE *prev, *next;
	LINE(LINE *p = 0, LINE *n = 0)
	{
		prev = p, next = n;
		if (p) p->next = this;
		if (n) n->prev = this;
		str = new VECT();
	}
	~LINE() { delete str; }
	void suicide()
	{
		if (prev) prev->next = next;
		if (next) next->prev = prev;
		next = 0;
		delete this;
	}
} *head, *row;
struct CUR
{
	int pos;
	char posback[10000];
	void insert(char p)
	{
		if(pos != row->str->num) row->str->copy(pos, posback);
		row->str->insert(p);
		pos++;
	}
	void clear()
	{
		for (int i = 0; posback[i]; i++) posback[i] = 0;
	}
	void backspace()
	{
		if (pos != row->str->num) row->str->copy(pos, posback);
		if (row->str->num > 0) row->str->del(), pos--;
		else if(row->prev)
		{
			row = row->prev;
			row->next->suicide();
			pos = row->str->num;
		}
	}
	void enter()
	{
		if (pos != row->str->num) row->str->copy(pos, posback);
		row = new LINE(row, row->next);
		pos = 0;
	}
	void beforemove()
	{
		row->str->clear(pos);
		if (posback[0]) row->str->insert(pos, posback);
		clear();
	}
	void moveleft(int n)
	{
		beforemove();
		if (pos >= n) pos -= n;
		else pos = 0;
	}
	void moveright(int n)
	{
		beforemove();
		if (row->str->num - pos >= n) pos += n;
		else pos = row->str->num;
	}
	void moveup(int n)
	{
		beforemove();
		while (n--)
		{
			if (row->prev) row = row->prev;
			else break;
		}
		if (pos > row->str->num) pos = row->str->num;
	}
	void movedown(int n)
	{
		beforemove();
		while (n--)
		{
			if (row->next) row = row->next;
			else break;
		}
		if (pos > row->str->num) pos = row->str->num;
	}
} cur;
void deleteall()
{
	LINE *row = head;
	for (; row;)
	{
		LINE *tmp = row;
		row = row->next;
		delete tmp;
	}
}
void init() {
	deleteall();
	head = new LINE();
	row = head;
	cur.clear(), cur.pos = 0;
}

void put_char(char c) {
	cur.insert(c);
}

void put_enter() {
	cur.enter();
}

void backspace() {
	cur.backspace();
}

void move_cursor_up(int n) {
	cur.moveup(n);
}

void move_cursor_down(int n) {
	cur.movedown(n);
}

void move_cursor_left(int n) {
	cur.moveleft(n);
}

void move_cursor_right(int n) {
	cur.moveright(n);
}

void close(char* d) {
	int i, sum = 0;
	LINE *r;
	cur.beforemove()
	for (r = head; r; r = r->next)
	{
		mstrcpy(d + sum, r->str->str);
		sum += r->str->num;
		if (r->next) d[sum++] = '\n';
	}
}