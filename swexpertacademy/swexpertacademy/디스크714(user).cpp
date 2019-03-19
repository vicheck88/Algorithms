extern bool change_disk(int _disk_id);
extern bool read_disk(int sector_id, char* data);
extern bool write_disk(int sector_id, char* data);
typedef unsigned long long ull;
int nodecnt;
char tmpdata[1025];
char tmpd[4096];
inline void mstrcpy(char *p, char *q)
{
	for (int i = 0; i < 16; i++) p[i] = q[i];
	p[16] = 0;
}
inline int mstrcmp(char *p, char *q)
{
	int i;
	for (i = 0; p[i] == q[i] && i < 16; i++);
	if (i == 16) return 0;
	return p[i] - q[i];
}
struct NODE
{
	int s, e;
	NODE *next;
	NODE(int start = 0, int end = 0) { s = start, e = end; next = 0; }
} node[100000], *remaining, *tmps, *tmpe;
struct LIST
{
	char file[17];
	NODE *head;
	LIST() { file[0] = 0; }
} filelist[500];
int getHash(char *p)
{
	ull hash=5381;
	for (int i = 0; i < 16; i++)
	{
		hash = hash * 33 + p[i];
	}
	return (int)(hash % 500);
}
int findLoc(char *p)
{
	int step = 7;
	int h = getHash(p);
	while (filelist[h].file[0] && mstrcmp(filelist[h].file, p))
	{
		h += step;
		if (h >= 500) h -= 500;
	}
	if (filelist[h].file[0] == 0) mstrcpy(filelist[h].file, p);
	return h;
}
int getsize(NODE *n) { return n->e - n->s + 1; }
NODE *split(NODE *n, int size)
{
	if (getsize(n) == size) return n;
	NODE *ref = &node[nodecnt++];
	*ref = NODE(n->s + size, n->e);
	ref->next = n->next;
	n->next = ref;
	n->e = ref->s - 1;
	return n;
}
void assign(int size, NODE *n)
{
	int h;
	tmps = tmpe = n;
	if (n)
	{
		while ((h = getsize(tmpe)) < size)
		{
			tmpe = tmpe->next;
			size -= h;
		}
		tmpe = split(tmpe, size);
		if(n==remaining) remaining = tmpe->next;
	}
}

void free(int h, int offset, int size)
{
	NODE *cur;
	if (!offset) tmpe = filelist[h].head;
	else assign(offset, filelist[h].head->next);
	cur = tmpe;
	assign(size, cur->next);
	cur->next = tmpe->next;
	tmpe->next = remaining;
	remaining = tmps;
}
void init()
{
	nodecnt = 0;
	remaining = &node[nodecnt++];
	*remaining = NODE(0, (16 << 20) - 1);
	for (int i = 0; i < 500; i++)
	{
		filelist[i].file[0] = 0;
		filelist[i].head = &node[nodecnt++];
		*(filelist[i].head) = NODE();
	}
}
void insertNode(int h, int offset)
{
	NODE *cur1 = tmps, *cur2 = tmpe;
	assign(offset, filelist[h].head->next);
	if (tmpe && offset)
	{
		cur2->next = tmpe->next;
		tmpe->next = cur1;
	}
	else
	{
		filelist[h].head->next = cur1;
		cur2->next = tmps;
	}
}
void writeData(int s, int e, char *data, int *disk, int *sect)
{
	int i, k = 0;
	for (i = s; i <= e; i++, k++)
	{
		if (*sect != ((i >> 10) & 1023))
		{
			if (*disk > -1) write_disk(*sect, tmpdata);
			if (*disk != (i >> 20))
			{
				*disk = i >> 20;
				change_disk(i >> 20);
			}
			*sect = ((i >> 10) & 1023);
			read_disk(*sect, tmpdata);
		}
		tmpdata[i & 1023] = data[k];
	}
	write_disk(*sect, tmpdata);
}
void readData(int s, int e, char *data, int *disk, int *sect)
{
	int i, k = 0;
	for (i = s; i <= e; i++, k++)
	{
		if (*sect != ((i >> 10) & 1023))
		{
			if (*disk != (i >> 20))
			{
				*disk = (i >> 20);
				change_disk(i >> 20);
			}
			*sect = ((i >> 10) & 1023);
			read_disk(*sect, tmpdata);
		}
		data[k] = tmpdata[i & 1023];
	}
}
void read(int h, int offset, char *data, int size)
{
	NODE *cur=filelist[h].head->next;
	if (offset) assign(offset, cur), cur = tmpe->next;
	assign(size, cur);
	int s = 0;
	for (cur = tmps; s < size ; cur = cur->next)
	{
		int disk = -1, sect = -1;
		readData(cur->s, cur->e, data + s, &disk, &sect);
		readData(cur->s, cur->e, tmpd + s, &disk, &sect);
		s += getsize(cur);
	}
}
void write(char *data, int size)
{
	int s = 0;
	for (NODE *cur = tmps; s < size; cur = cur->next)
	{
		int disk = -1,  sect=-1;
		writeData(cur->s, cur->e, data + s, &disk, &sect);
		s += getsize(cur);
	}
}
void read_file(char* filename, int offset, char* data, int size)
{
	if (!size) return;
	int h = findLoc(filename);
	read(h, offset, data, size);
}
void insert_file(char* filename, int offset, char* data, int size)
{
	if (!size) return;
	int h = findLoc(filename);
	assign(size, remaining);
	write(data, size);
	insertNode(h, offset);
}
void delete_file(char* filename, int offset, int size)
{
	if (!size) return;
	int h = findLoc(filename);
	free(h, offset, size);
}