#ifndef NULL
#define NULL 0
#endif 
const int hashsize = (1 << 21);
int nodecnt;
int total;
int song[10001][200];
int songlen[10001];
int mat[3][8] = {
	{128,128,128,128,-127,-127,-127,-127},
{128,128,-127,-127,128,128,-127,-127},
{128,-127,128,-127,128,-127,128,-127}
};
struct NODE
{
	int idx, num;
	NODE *next;
	NODE() { idx = num = 0; }
} node[3000000], *hash[hashsize];
inline int extract(int n)
{
	return (n >> 8) & 127;
}
void push(int key, int num, int idx)
{
	node[nodecnt].idx = idx;
	node[nodecnt].num = num;
	node[nodecnt].next = hash[key];
	hash[key] = &node[nodecnt++];
}

int getkey(int x,int y,int z)
{
	return (extract(x) << 14) | (extract(y) << 7) | extract(z);
}

void initUser(int soundNum) {
	total = soundNum;
	nodecnt = 0;
	for (int i = 0; i < hashsize; i++) hash[i] = 0;
}

void registerSound(int soundID, int soundLen, int soundArr[200]) {
	for (int i = 0; i < soundLen; i++) song[soundID][i] = soundArr[i];
	for (int i = 0; i <= soundLen - 8; i++)
	{
		push(getkey(song[soundID][i], song[soundID][i+1], song[soundID][i+2]), soundID, i);
	}
}
bool match(NODE *cur, int *newData)
{
	int idx = cur->idx, num = cur->num;
	for (int i = 0; i < 8; i++)
	{
		if (newData[i] - song[num][idx + i] < -128 || newData[i] - song[num][idx + i]>127) return false;
	}
	return true;
}
int  plagiarismCheck(int newData[8])
{
	for (int i = 0; i < 8; i++)
	{
		int key = (extract(newData[0]+mat[0][i]) << 14) | 
			(extract(newData[1] + mat[1][i]) << 7) | extract(newData[2] + mat[2][i]);
		for (NODE *cur = hash[key]; cur; cur = cur->next)
		{
			if (match(cur, newData))
			{
				return cur->num;
			}
		}
	}
}