#include <stdio.h>
int T;
char str[1001];
int cnt;
inline void strcat(char *p, const char *q)
{
	while (*p) p++;
	while (*p++ = *q++);
}
char * reverse()
{
	char *tmp = new char[1000]();
	if (str[cnt] == 'b' || str[cnt] == 'w')
	{
		tmp[0] = str[cnt++]; return tmp;
	}
	cnt++;
	char *upperleft = reverse();
	char *upperright = reverse();
	char *lowerleft = reverse();
	char *lowerright = reverse();
	strcat(tmp, "x");
	strcat(tmp, lowerleft);
	strcat(tmp, lowerright);
	strcat(tmp, upperleft);
	strcat(tmp, upperright);
	delete[] upperleft, upperright, lowerleft, lowerright;
	return tmp;
}

int main()
{
	scanf("%d", &T);
	while (T--)
	{
		cnt = 0;
		scanf("%s", str);
		char *sol = reverse();
		printf("%s\n",sol);
		delete[] sol;
	}
}