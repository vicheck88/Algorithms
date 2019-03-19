#ifndef NULL
#define NULL 0
#endif // NULL
typedef unsigned int UI;
extern unsigned int hexToDeci(unsigned char*s, unsigned int len);
extern unsigned int getBlockID(unsigned char*str, int stIdx, int length);
const int LM = (int)1e6 + 5;
struct EXCHANGE
{
	int ID;
	int volume;
	EXCHANGE() { ID = volume = 0; }
};
struct BLOCK
{
	BLOCK *next;
	UI parID, ID;
	int transN;
	EXCHANGE *exchange;
	int valid;
	BLOCK()
	{
		valid = parID = ID = transN = 0;
		exchange = 0;
		next = 0;
		valid = 1;
	}
	BLOCK(BLOCK *b)
	{
		parID = b->parID, ID = b->ID, valid = b->valid;
		transN = b->transN;
		exchange = new EXCHANGE[b->transN];
		for (int i = 0; i < transN; i++)
		{
			exchange[i].ID = b->exchange[i].ID;
			exchange[i].volume = b->exchange[i].volume;
		}
		next = 0;
	}
};

struct LIST
{
	BLOCK *head, *tail;
	LIST() { head = tail = new BLOCK(); }
	~LIST()
	{
		BLOCK *cur = head;
		for (; cur!=tail;)
		{
			BLOCK *tmp = cur;
			cur = cur->next;
			if (tmp->exchange) delete[] tmp->exchange;
			delete tmp;
		}
		delete[] tail->exchange;
		delete tail;
	}
	void insert(BLOCK *n)
	{
		tail->next = n; 
		tail = n;
	}
	bool find(BLOCK *n)
	{
		BLOCK *cur = head->next;
		for (; cur; cur = cur->next)
		{
			if (cur->ID != n->ID) continue;
			cur->valid++;
			break;
		}
		if (!cur) return true;
		return false;
	}
};
LIST *block_list[20000];
BLOCK Block[15000+10];
BLOCK tmp[15000+10];
int block_cnt;
int lim;

void Block_init();
void Make_list(unsigned char *image);
void sort(int s, int e);
int upperbound(UI id,int s,int e);


void processBlockImage(int serverCnt, unsigned char image[][LM]) {
	Block_init();
	lim = serverCnt / 2;
	for (int i = 0; i < serverCnt; i++) Make_list(image[i]);
	sort(0, block_cnt - 1);
}
int getSumVolume(unsigned int ID, int exchangeID) {
	int ans=0;
	UI hash = ID % 20000;
	if (ID == 0)
	{
		for (int i = 0; Block[i].parID == 0; i++)
		{
			ans+=getSumVolume(Block[i].ID, exchangeID);
		}
		return ans;
	}
	if (block_list[hash] == 0) return ans;
	BLOCK *cur = block_list[hash]->head;
	for (; cur; cur = cur->next)
	{
		if (cur->ID != ID) continue;
		if (cur->valid <= lim) return 0;
		for (int i = 0; i < cur->transN; i++)
		{
			if (cur->exchange[i].ID != exchangeID) continue;
			ans += cur->exchange[i].volume;
		}
		break;
	}
	int u = upperbound(ID,0,block_cnt-1)-1 ;
	int l = upperbound(ID - 1,0,u);
	if (Block[l].parID != ID) return ans;
	for (int i = l; i <= u; i++)
		ans += getSumVolume(Block[i].ID, exchangeID);
	return ans;
}
void Block_init()
{
	for (int i = 0; i < 20000; i++)
	{
		if (block_list[i])
		{
			delete block_list[i];
			block_list[i] = 0;
		}
	}
	for (int i = 0; i < block_cnt; i++)
	{
		if (Block[i].exchange) delete[] Block[i].exchange;
		Block[i] = BLOCK();
	}
	block_cnt = 0;
}

void Make_list(unsigned char *list)
{
	int cnt = 8, blockinit;
	BLOCK *block;
	while (list[cnt])
	{
		block = new BLOCK();
		blockinit = cnt;
		block->parID = hexToDeci(list + cnt, 8);
		cnt += 12;
		block->transN = (int)hexToDeci(list + cnt, 2);
		block->exchange = new EXCHANGE[block->transN];
		cnt += 2;
		for (int i = 0; i < block->transN; i++)
		{
			block->exchange[i].ID = (int)hexToDeci(list + cnt, 2);
			cnt += 2;
			block->exchange[i].volume = (int)hexToDeci(list + cnt, 4);
			cnt += 4;
		}
		int len = cnt - blockinit;
		block->ID = getBlockID(list, blockinit, len);
		UI hash = block->ID % 20000;
		if (!block_list[hash]) block_list[hash] = new LIST();
		if (block_list[hash]->find(block))
		{
			Block[block_cnt++] = block;
			block_list[hash]->insert(block);
		}
		else
		{
			delete[] block->exchange;
			delete block;
		}
	}
}
void sort(int s, int e)
{
	if (s >= e) return;
	int i = s, k = s, m = (s + e) / 2, j = m + 1;
	sort(s, m); sort(m + 1, e);
	while (i <= m && j <= e)
	{
		if (Block[i].parID < Block[j].parID) tmp[k++] = Block[i++];
		else tmp[k++] = Block[j++];
	}
	while (i <= m) tmp[k++] = Block[i++];
	while (j <= e) tmp[k++] = Block[j++];
	for (int i = s; i <= e; i++) Block[i] = tmp[i];
}
int upperbound(UI id, int s, int e)
{
	int m;
	int ans = block_cnt;
	while (s <= e)
	{
		m = (s + e) / 2;
		if (Block[m].parID <= id) s = m + 1;
		else
		{
			ans = m; e = m - 1;
		}
	}
	return ans;
}