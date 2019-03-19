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
	for (i = 0; p[i] && q[i]; i++) if (p[i] != q[i]) break;
	return p[i] - q[i];
}
void plus(char *a, char *b, char *c)
{
	int i, j;
	int alen = strlen(a), blen = strlen(b);
	int x[300] = { 0 }, y[300] = { 0 };
	int z[300] = { 0 };
	for (i = alen - 1, j = 0; i >= 0; i--, j++)
		x[j] = a[i] - '0';
	for (i = blen - 1, j = 0; i >= 0; i--, j++)
		y[j] = b[i] - '0';
	for (i = 0; i < alen || i < blen; i++)
	{
		z[i] += x[i] + y[i];
		if (z[i] > 9)
		{
			z[i + 1]++, z[i] -= 10;
		}
	}
	while (!z[i]) i--;
	for (j = 0; i >= 0; j++) c[j] = z[i--] + '0';
	c[j] = 0;
}
void minus(char *a, char *b, char *c)
{
	int i, j;
	char *ap, *bp;
	int alen = strlen(a), blen = strlen(b);
	if (alen > blen || (alen == blen && strcmp(a, b) > 0))
	{
		ap = a, bp = b;
	}
	else if (strcmp(a, b) == 0)
	{
		c[0] = '0'; return;
	}
	else
	{
		ap = b, bp = a;
	}
	int aplen = strlen(ap), bplen = strlen(bp);
	int x[300] = { 0 }, y[300] = { 0 };
	int z[300] = { 0 };
	for (i = aplen - 1, j = 0; i >= 0; i--, j++)
		x[j] = ap[i] - '0';
	for (i = bplen - 1, j = 0; i >= 0; i--, j++)
		y[j] = bp[i] - '0';
	for (i = 0; i < aplen || i < bplen; i++)
	{
		z[i] = x[i] - y[i];
		if (z[i] < 0)
		{
			x[i + 1]--, z[i] += 10;
		}
	}
	while (!z[i]) i--;
	for (j = 0; i >= 0; j++) c[j] = z[i--] + '0';
	c[j] = 0;
}

int main()
{
	char a[300], b[300];
	char ans[300];
	while (1)
	{
		scanf("%s", a);
		scanf("%s", b);
		if (strcmp(a, "0") == 0 && strcmp(b, "0") == 0) break;
		plus(a, b, ans);
		printf("%s\n", ans);
		for (int i = 0; i < 300; i++) ans[i] = 0;
		minus(a, b, ans);
		printf("%s\n", ans);
		for (int i = 0; i < 300; i++) ans[i] = 0;
	}
	return 0;
}