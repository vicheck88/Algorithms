#include <stdio.h>
inline int strlen(char *p)
{
	int i;
	for (i = 0; p[i]; i++);
	return i;
}
inline int strcmp(const char *p, const char *q)
{
	int i = 0;
	for (; p[i] && q[i] && p[i] == q[i]; i++);
	return p[i] - q[i];
}
inline int strncmp(const char *p, const char *q, int len)
{
	int i;
	for (i = 0; i < len && p[i] && q[i] && p[i] == q[i]; i++);
	if (i == len) return 0;
	return p[i] - q[i];
}
inline int intncmp(int *X, int *Y, int len)
{
	int i;
	for (i = 0; i < len; i++) if (X[i] != Y[i]) break;
	if (i == len) return 0;
	return X[i] - Y[i];
}
char a[210], b[210], ans[210];
inline void convert(char *p, int *X)
{
	for (int i = 0; p[i]; i++) X[i] = p[i] - '0';
}
bool subtract(int *X, int *Y, int len)
{
	if (intncmp(X, Y, len) < 0) return false;
	for (int i = len - 1; i >= 0; i--)
	{
		X[i] -= Y[i];
		if (X[i] < 0) X[i - 1]--, X[i] += 10;
	}
	return true;
}
void divide(char *a, char *b)
{
	int i, j;
	int flag = 0;
	char *ap, *bp;
	int alen, blen, aplen, bplen;
	alen = strlen(a), blen = strlen(b);
	if (alen > blen || (alen == blen && strcmp(a, b) > 0))
		ap = a, bp = b, aplen = alen, bplen = blen;
	else ap = b, bp = a, aplen = blen, bplen = alen;
	int X[220] = { 0 }, Y[220] = { 0 }, Z[220] = { 0 };
	convert(ap, X), convert(bp, Y);
	for (int i = 0; i <= aplen - bplen; i++)
	{
		while(subtract(X + i, Y, bplen)) Z[i]++;
		if (X[i] > 0) X[i + 1] += X[i] * 10;
	}
	i = 0;
	while (!Z[i]) i++;
	for (j=0 ; i <= aplen - bplen; i++,j++) ans[j] = Z[i] + '0';
	ans[j] = 0;
}
int main()
{
	while (1)
	{
		scanf("%s", a); scanf("%s", b);
		if (a[0] == '0' || b[0] == '0') break;
		divide(a, b);
		printf("%s\n", ans);
	}
	return 0;
}