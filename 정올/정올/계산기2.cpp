#include <stdio.h>
inline int strlen(const char *p)
{
	register int i;
	for (i = 0; p[i]; i++);
	return i;
}
inline int strcmp(const char *p,const char *q)
{
	register int i;
	for (i = 0; p[i] && q[i] && p[i] == q[i]; i++);
	return p[i] - q[i];
}
inline int intncmp(int *X, int *Y, int len)
{
	int i;
	for (i = 0; i < len && X[i] == Y[i]; i++);
	if (i == len) return 0;
return X[i] - Y[i];
}
inline void strcpy(char *p, const char *q) { while (*p++ = *q++); }
inline bool bigger(char *p, char *q)
{
	int plen = strlen(p), qlen = strlen(q);
	if (plen > qlen || (plen == qlen && strcmp(p, q) >= 0)) return true;
	return false;
}
inline void convertinv(const char *p, int *X)
{
	int len = strlen(p);
	for (int i = len - 1, j = 0; i >= 0; i--, j++) X[j] = p[i] - '0';
}
inline void convert(char *p, int *X)
{
	for (int i = 0; p[i]; i++) X[i] = p[i] - '0';
}
inline void convertback(int *Z, char *c, int len)
{
	int i, j;
	i = len - 1;
	while (!Z[i]) i--;
	for (j = 0; i >= 0; j++, i--) c[j] = Z[i] + '0';
	c[j] = 0;
}
void add(char *a, char *b, char *c)
{
	int X[310] = { 0 }, Y[310] = { 0 }, Z[310] = { 0 };
	int alen = strlen(a), blen = strlen(b);
	int i, j;
	convertinv(a, X);
	convertinv(b, Y);
	for (i = 0; i <= alen || i <= blen; i++)
	{
		Z[i] += X[i] + Y[i];
		if (Z[i] > 9) Z[i] -= 10, X[i + 1]++;
	}
	convertback(Z, c, (alen>blen ? alen+1:blen+1));
}
void subtract(char *a, char *b, char *c)
{
	int i, j;
	char *ap, *bp;
	int X[310] = { 0 }, Y[310] = { 0 }, Z[310] = { 0 };
	int alen = strlen(a), blen = strlen(b);
	convertinv(a, X), convertinv(b, Y);
	for (i = 0; i < alen; i++)
	{
		Z[i] += (X[i] - Y[i]);
		if (Z[i] < 0) X[i + 1]--, Z[i] += 10;
	}
	convertback(Z, c, alen);
}
void multiply(char *a, const char *b, char *c)
{
	int i, j;
	if (a[0] == '0' || b[0] == '0')
	{
		strcpy(c, "0");
		return;
	}
	int X[310] = { 0 }, Y[310] = { 0 }, Z[310] = { 0 };
	int alen = strlen(a), blen = strlen(b);
	convertinv(a, X), convertinv(b, Y);
	for (i = 0; i < alen; i++)
	{
		for (j = 0; j < blen; j++)
		{
			Z[i + j] += X[i] * Y[j];
		}
	}
	for (i = 0; i <= alen + blen; i++)
	{
		if (Z[i] > 9)
		{
			Z[i + 1] += Z[i] / 10;
			Z[i] %= 10;
		}
	}
	convertback(Z, c, alen+blen);
}
bool check(int *X, int *Y, int len)
{
	if (intncmp(X, Y, len) < 0) return false;
	for (int i = len - 1; i >= 0; i--)
	{
		X[i] -= Y[i];
		if (X[i] < 0) X[i] += 10, X[i - 1]--;
	}
	return true;
}
void divide(char *a, char *b, char *c)
{
	int i, j, cnt = 0;
	int X[310] = { 0 }, Y[310] = { 0 }, Z[310] = { 0 };
	int alen = strlen(a), blen = strlen(b);
	convert(a, X), convert(b, Y);
	if (alen < blen || (alen == blen && strcmp(a, b) < 0))
	{
		strcpy(c, "0");
		return;
	}
	for (i = 0; i <= alen - blen; i++)
	{
		while (check(X + i, Y, blen)) Z[i]++;
		if (X[i] && i<alen-blen)
		{
			X[i + 1] += (X[i] * 10);
			X[i] = 0;
		}
	}
	
	for (i = 0; !X[i]; i++);
	for (j=0; i < alen; i++,j++) a[j] = X[i] + '0';
	a[j] = 0;
	for (i = 0; !Z[i]; i++);
	for (j=0; i <= alen - blen; i++,j++) c[j] = Z[i] + '0';
	c[j] = 0;

}
struct rational
{
	int sign;
	char numerator[310];
	char denominator[310];
	rational()
	{
		sign = 1;
		strcpy(numerator, "0"), strcpy(denominator, "1");
	}
	rational(const char *a)
	{
		sign = 1;
		strcpy(numerator, a), strcpy(denominator, "1");
	}
	rational operator*(rational &b)
	{
		rational c = *this;
		multiply(numerator, b.numerator, c.numerator);
		multiply(denominator, b.denominator, c.denominator);
		c.sign = sign * b.sign;
		return c;
	}
	rational operator/(rational &b)
	{
		rational c = *this;
		multiply(denominator, b.numerator, c.denominator);
		multiply(numerator, b.denominator, c.numerator);
		c.sign = sign * b.sign;
		return c;
	}
	rational operator+(rational &b)
	{
		rational c = *this;
		if (strcmp(b.denominator, denominator))
		{
			multiply(denominator, b.denominator, c.denominator);
			multiply(numerator, b.denominator, c.numerator);
			multiply(denominator, b.numerator, b.numerator);
		}
		if (sign == b.sign) add(c.numerator, b.numerator, c.numerator);
		else
		{
			if (bigger(c.numerator, b.numerator)) 
				subtract(c.numerator, b.numerator, c.numerator);
			else
			{
				c.sign = b.sign;
				subtract(b.numerator, c.numerator, c.numerator);
			}
		}
		return c;
	}
};

int main()
{
	int T, i, j;
	char str[310];
	char num[310] = { 0 };
	char sol[310] = { 0 };
	char op = 0;
	rational result;
	rational tmp;
	int sign = 1;
	scanf("%d", &T);
	while (T--)
	{
		result = rational();
		tmp = rational();
		op = 0;
		scanf("%s", str);
		for (i = 0, j = 0; str[i]; i++)
		{
			if ('0' <= str[i] && str[i] <= '9') num[j++] = str[i];
			else
			{
				num[j] = 0;
				switch (op)
				{
				case '*': multiply(tmp.numerator, num, tmp.numerator); break;
				case '/': multiply(tmp.denominator, num, tmp.denominator); break;
				case 0: tmp = rational(num), tmp.sign = sign;
				}
				if (str[i] == '+' || str[i] == '-' || str[i] == '=')
				{
					result = result + tmp;
					tmp = rational();
					if (str[i] == '-') sign = -1;
					else sign = 1;
				}
				if (str[i] == '*' || str[i] == '/') op = str[i];
				else op = 0;
				j = 0;
			}
		}
		if (result.sign == -1) printf("-");
		divide(result.numerator, result.denominator, sol);
		printf("%s.", sol);
		for (int i = 0; i < 30; i++)
		{
			multiply(result.numerator, "10", result.numerator);
			divide(result.numerator, result.denominator, sol);
			printf("%c", sol[0]);
		}
		printf("\n");
	}
	return 0;
}