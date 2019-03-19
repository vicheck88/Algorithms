#include <stdio.h>
inline int strlen(char *p)
{
	int i;
	for (i = 0; p[i]; i++);
	return i;
}
inline int strcmp(const char *p, const char *q)
{
	int i;
	for (int i = 0; p[i] && q[i] && p[i] == q[i]; i++);
	return p[i] - q[i];
}
int flag;
char a[110], b[110], ans[220];
inline void convert(char *p, int *A)
{
	int minus = 0;
	if (p[0] == '-') flag += 1, minus = 1;
	int len = strlen(p);
	for (int i = len - 1, j = 0; i - minus >= 0; j++, i--) A[j] = p[i] - '0';
}
void multiply(char *a, char *b)
{
	int i,j;
	if (a[0]=='0' || b[0]=='0')
	{
		ans[0] = '0';
		ans[1] = 0;
		return;
	}
	int X[110] = { 0 }, Y[110] = { 0 };
	convert(a, X);
	convert(b, Y);
	int Z[220] = { 0 };
	int alen = strlen(a), blen = strlen(b);
	for (i = 0; i < alen; i++)
	{
		for (j = 0; j < blen; j++)
		{
			Z[i + j] += X[i] * Y[j];
		}
	}
	for (i = 0; i < alen + blen + 1; i++)
	{
		if (Z[i] > 9)
		{
			Z[i + 1] += Z[i] / 10;
			Z[i] %= 10;
		}
	}
	while (!Z[i]) i--;
	int minus = 0;
	if (flag & 1) minus = 1, ans[0] = '-';
	for (j = minus; i >= 0; j++) 
		ans[j] = Z[i--] + '0';
	ans[j] = 0;
}

int main()
{
	while (1)
	{
		flag = 0;
		scanf("%s", a); scanf("%s", b);
		if (a[0] == '0') break;
		multiply(a, b);
		printf("%s\n", ans);
	}
	return 0;
}