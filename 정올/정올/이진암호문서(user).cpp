#ifndef MAX
#define MAX  (256)
#endif
typedef unsigned long long ull;
int map[5][MAX][MAX];
int accMap[5][MAX][MAX];
int fullSizedMap[MAX+1][MAX+1];
ull compressMap[MAX / 8 + 1][MAX / 8 + 1];
int numOfOnes[1 << 16];
int fullSize;
int chkSize;
int mask = (1 << 16) - 1;
int cnt;
int order[5];
int chk[5];
int verified[3][MAX+1][MAX+1];
ull newCompressed[3][MAX / 8 + 1][MAX / 8 + 1];
int threshold;

extern void userInit(int docSize) {
	cnt = 0;
	fullSize = docSize;
	chkSize = fullSize / 2;
	for (int i = 0; i < (1 << 16); i++)
	{
		numOfOnes[i] = numOfOnes[i >> 1] + (i & 1);
	}
	threshold = chkSize * chkSize / 20;
	for (int i = 0; i < 5; i++) chk[i] = order[i] = 0;
}
bool chksum(int ord, int sr, int sc, int er, int ec, int sum)
{
	return sum == accMap[ord][er][ec] + accMap[ord][sr][sc] - accMap[ord][er][sc] - accMap[ord][sr][ec];
}
bool chkValidity(int ord, int sr, int sc, int er, int ec, int si, int sj)
{
	for (int i = 0; i <= er - sr; i++)
	{
		for (int j = 0; j <= ec - sc; j++)
		{
			if (map[order[4]][sr + i][sc + j] != map[ord][si + i][sj + j]) return false;
		}
	}
	return true;
}
bool isValid()
{
	int(*accSrc)[MAX] = accMap[order[4]];
	int(*src)[MAX] = map[order[4]];
	int er, ec;
	for (int i = 2; i < chkSize-2; i++)
	{
		for (int j = 2; j < chkSize-2; j++)
		{
			er = chkSize - i, ec = chkSize - j;
			int chkFirst = accSrc[er][ec];
			int chkSecond = accSrc[er][chkSize] - chkFirst;
			int chkThird = accSrc[chkSize][ec] - chkFirst;
			int chkFourth = accSrc[chkSize][chkSize] - chkFirst - chkSecond - chkThird;
			
			if (!chksum(order[0], i, j, chkSize, chkSize, chkFirst)) continue;
			if (!chksum(order[1], i, 0, chkSize, j, chkSecond)) continue;
			if (!chksum(order[2], 0, j, i, chkSize, chkThird)) continue;
			if (!chksum(order[3], 0, 0, i, j, chkFourth)) continue;

			if (!chkValidity(order[0], 1, 1, er, ec, i + 1, j + 1)) continue;
			if (!chkValidity(order[1], 1, ec+1, er, chkSize, i + 1, 1)) continue;
			if (!chkValidity(order[2], er+1, 1, chkSize, ec, 1, j + 1)) continue;
			if (!chkValidity(order[3], er+1, ec+1, chkSize, chkSize, 1, 1)) continue;
			return true;
		}
	}
	return false;
}
int DFS(int n)
{
	if (n >= 5)
	{
		if (isValid()) return 1;
		return 0;
	}
	for (int i = 0; i < 5; i++)
	{
		if (chk[i]) continue;
		chk[i] = 1;
		order[n] = i;
		if (DFS(n + 1)) return 1;
		chk[i] = 0;
	}
	return 0;
}
void paintMap()
{
	int startcol[4] = { 1,chkSize + 1,1,chkSize + 1 };
	int startrow[4] = { 1,1,chkSize + 1,chkSize + 1 };
	for (int ord = 0; ord < 4; ord++)
	{
		for (int i = startrow[ord]; i <= startrow[ord] + chkSize; i++)
		{
			for (int j = startcol[ord]; j <= startcol[ord] + chkSize; j++)
			{
				fullSizedMap[i][j] = map[order[ord]][i - startrow[ord]+1][j - startcol[ord]+1];
			}
		}
	}
}
ull getHash(int r, int c, int (*map)[MAX+1])
{
	ull hash=0;
	for (int i = r; i < r + 8; i++)
	{
		for (int j = c; j < c + 8; j++)
		{
			hash = hash * 2 + map[i][j];
		}
	}
	return hash;
}
void compress(ull (*compressMap)[33],int size, int (*map)[MAX+1])
{
	for (int i = 1; i <= size / 8; i++)
	{
		for (int j = 1; j <= size / 8; j++) 
			compressMap[i][j] = getHash((i-1)*8+1, (j-1)*8+1, map);
	}
}
extern void shuffledVDSD(char document[][MAX]) {
	for (int i = 1; i <= chkSize; i++)
	{
		for (int j = 1; j <= chkSize; j++)
		{
			map[cnt][i][j] = document[i-1][j-1];
			accMap[cnt][i][j] = map[cnt][i][j] + accMap[cnt][i - 1][j] + accMap[cnt][i][j - 1] -
				accMap[cnt][i - 1][j - 1];
		}
	}
	cnt++;
	if (cnt == 5)
	{
		DFS(0);
		paintMap();
		compress(compressMap, fullSize,fullSizedMap);
	}
}
int diffBit(ull a)
{
	int cnt = 0;
	for (; a; a >>= 16)
	{
		cnt += numOfOnes[a&mask];
	}
	return cnt;
}
bool isOK(ull(*map)[33], int r, int c)
{
	int cnt = 0;
	for (int i = 0; i < chkSize / 8; i++)
	{
		for (int j = 0; j < chkSize / 8; j++)
		{
			cnt += diffBit(compressMap[r + i][c + j] ^ map[i+1][j+1]);
			if (cnt > threshold) return false;
		}
	}
	return true;
}

extern int verifyDocument(char document[][MAX]) {
	for (int i = 0; i < chkSize; i++)
	{
		for (int j = 0; j < chkSize; j++)
		{
			verified[0][1+i][1+j] = document[i][j];
			verified[1][1 + i][1 + j] = document[chkSize - 1 - j][i];
			verified[2][i + 1][j + 1] = document[j][chkSize - 1 - i];
		}
	}
	for (int i = 0; i < 3; i++)
	{
		compress(newCompressed[i], chkSize,verified[i]);
	}
	for (int i = 1; i <= chkSize / 8 + 1; i++)
	{
		for (int j = 1; j <= chkSize / 8 + 1; j++)
		{
			if (isOK(newCompressed[0], i, j)) return 1;
			if (isOK(newCompressed[1], i, j)) return 1;
			if (isOK(newCompressed[2], i, j)) return 1;
		}
	}
	return 0;

}

