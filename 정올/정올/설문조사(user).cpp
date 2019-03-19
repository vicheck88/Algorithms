int celubnum=1;
int querycnt=1;
int blinded[10005];
int list[50005];
struct CELNODE
{
	int id;
	CELNODE *prev, *next;
	CELNODE(int id = 0, CELNODE *p = 0)
	{
		this->id = id;
		if (p)
		{
			prev = p;
			next = p->next;
			p->next = this;
			if(next) next->prev = this;
		}
		else prev = next = 0;
	}
	~CELNODE() { delete next; }
	CELNODE *insert(int id)
	{
		return new CELNODE(id, this);
	}
	void selfdel()
	{
		prev->next = next;
		if(next) next->prev = prev;
		next = prev = 0;
		delete this;
	}

} celub[10005];
struct TRIE
{
	int celubnum;
	TRIE *next[26];
	TRIE()
	{
		celubnum = 0;
		for (int i = 0; i < 26; i++) next[i] = 0;
	}
	~TRIE()
	{
		for (int i = 0; i < 26; i++) if (next[i]) delete next[i];
	}
	int insert(char *p)
	{
		if (*p == 0)
		{
			if (celubnum) return celubnum;
			else return celubnum = ::celubnum++;
		}
		int idx = *p - 'a';
		if (!next[idx]) next[idx] = new TRIE();
		return next[idx]->insert(p + 1);
	}
	int search(char *p)
	{
		if (*p == 0) return celubnum;
		int idx = *p - 'a';
		if (!next[idx]) return 0;
		return next[idx]->search(p + 1);
	}

} *root;
struct STUNODE
{
	int celubcnt;
	int blinded;
	int celubarr[4];
	CELNODE *celub[4];
	STUNODE()
	{
		celubcnt = blinded = 0;
		for (int i = 0; i < 4; i++)
		{
			celub[i] = 0, celubarr[i] = 0;
		}
	}
	void insert(int cnt, char *p,int id)
	{
		int celubnum = root->insert(p);
		celub[cnt] = ::celub[celubnum].insert(id);
		celubarr[cnt] = root->insert(p);
	}
	void update()
	{
		blinded = 0;
		for (int i = 0; i < celubcnt; i++)
		{
			if (::blinded[celubarr[i]]) blinded++;
		}
	}
	void cancel()
	{	
		for (int i = 0; i < celubcnt; i++)
		{
			celub[i]->selfdel();
			celubarr[i] = 0;
			celub[i] = 0;
		}
		celubcnt = blinded = 0;
	}
} student[50005];


void initUser()
{
	celubnum = 1;
	for (int i = 0; i < 10005; i++)
	{
		blinded[i] = 0;
		delete celub[i].next;
		celub[i].next = 0;
	}
	delete root;
	root = new TRIE();
}

/// �������� ����� ����

void addSurveyResult(int stdID, int cnt, char celebID[][11])
{
	if (student[stdID].celubcnt) student[stdID] = STUNODE();
	for (int i = 0; i < cnt; i++)
	{
		student[stdID].insert(i, celebID[i], stdID);
	}
	student[stdID].celubcnt = cnt;
	student[stdID].update();
}

/// �������� ��� ����� ���

void cancelSurveyResult(int stdID)
{
	student[stdID].cancel();
}

/// Ư�� ���� �ι��� ���� ��� ��ȣ�� ����� ���ܽ�Ű��

void blindCelebID(char celebID[])
{
	int idx = root->search(celebID);
	blinded[idx] = 1;
	for (CELNODE *cur = celub[idx].next; cur; cur = cur->next)
	{
		student[cur->id].update();
	}
}

/// Ư�� ���� �ι��� ���� ��� ��ȣ�� ����� ���ܵǾ� �ִٸ� �ٽ� ����

void recoverCelebID(char celebID[])
{
	int idx = root->search(celebID);
	blinded[idx] = 0;
	for (CELNODE *cur = celub[idx].next; cur; cur = cur->next)
	{
		student[cur->id].update();
	}
}

/// Ư���ι��� ��ȣ�ϴ� �л����� �����Ѵ�.
/// ��, blind ó���� �ι��� �������� ���� �л��� ����� �ȴ�.
/// mode == 0 : ���� �ι��� ��ȣ�ϴ� �л� ��
/// mode == 1 : �� �ι� ��θ� ��ȣ�ϴ� �л� ��
/// mode == 2 : �� �� �ϳ� �̻��� ��ȣ�ϴ� �л� ��

int query(int mode, char celebID[][11])
{
	querycnt++;
	int num = 0;
	if (!mode)
	{
		int idx = root->search(celebID[0]);
		if (!blinded[idx])
		{
			for (CELNODE *cur = celub[idx].next; cur; cur = cur->next)
			{
				if (student[cur->id].blinded) continue;
				num++;
			}
		}
	}
	else
	{
		int num1 = 0, num2 = 0;
		int idx1 = root->search(celebID[0]), idx2 = root->search(celebID[1]);
		if (idx1 && !blinded[idx1])
		{
			for (CELNODE *cur = celub[idx1].next; cur; cur = cur->next)
			{
				if (student[cur->id].blinded) continue;
				list[cur->id] = querycnt;
				num2++;
			}
		}
		if (idx2 && !blinded[idx2])
		{
			for (CELNODE *cur = celub[idx2].next; cur; cur = cur->next)
			{
				if (student[cur->id].blinded) continue;
				if (list[cur->id] == querycnt) num1++;
				else num2++;
			}
		}
		if (mode == 1) num = num1;
		else num = num2;
	}
	return num;
}