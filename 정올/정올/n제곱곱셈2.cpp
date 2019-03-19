typedef unsigned long long ull;
const int MAX = 257;
const int HMAX = 129;
int X[HMAX], Y[HMAX];
ull num1[32], num2[32];
inline int strlen(const char *p)
{
	int i;
	for (i = 0; p[i]; i++);
	return i;
}

inline void convert(ull *X, const char *a, int d, int len, int part, int rem)
{
	int i, j = part - 1;
	int cnt = 0;
	int num;
	if (rem != 0) rem -= 5;
	for (i = rem; i < len; i++)
	{
		cnt++;
		if (i < 0) continue;
		num = a[i] <= '9' ? a[i] - '0' : a[i] - 'A' + 10;
		X[j] = X[j] * d + num;
		if (!(cnt % 5)) j--;
	}
}
void cal(char* result, const char* arr1, const char* arr2, int d)
{
	int i, j;
	int Z[MAX] = { 0 };
	for (int i = 0; i < 32; i++)
	{
		num1[i] = num2[i] = 0;
	}
	ull num3[65] = { 0 };
	int len1 = strlen(arr1), len2 = strlen(arr2);
	int rem1 = len1 - ((len1 / 5)  * 5), rem2 = len2 - ((len2 /5) *5);
	int part1 = (len1 /5) + (rem1 != 0), part2 = (len2 /5) + (rem2 != 0);
	convert(num1, arr1, d, len1, part1, rem1);
	convert(num2, arr2, d, len2, part2, rem2);

	for (i = 0; i < part1; i++)
	{
		for (j = 0; j < part2; j++)
		{
			num3[i + j] += num1[i] * num2[j];
		}
	}
	for (i = 0, j = 0; i <= part1 + part2; i++, j = i * 4)
	{
		while (num3[i])
		{
			Z[j++] += (int)(num3[i] % d);
			num3[i] /= d;
		}
	}
	for (i = 0; i <= len1 + len2; i++)
	{
		if (Z[i] >= d)
		{
			Z[i + 1] += Z[i] / d;
			Z[i] %= d;
		}
	}
	while (!Z[i]) i--;
	for (j = 0; i >= 0; i--, j++)
	{
		result[j] = Z[i] < 10 ? Z[i] + '0' : Z[i] + 55;
	}
	result[j] = 0;
}