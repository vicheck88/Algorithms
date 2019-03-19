#include <stdio.h>
#define INF 0x7f123456
int T;
int clock[16];
int clk_switch[10][16]=
{
{ 1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,0 },
{ 0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1 },
{ 1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,1,1,1,0,1,0,1,0,0,0 },
{ 1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1 },
{ 0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1 },
{ 0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,1 },
{ 0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0 }
};
inline int min(int x, int y) { return x > y ? y : x; }
inline bool aligned()
{
	int i;
	for (i = 0; i < 16; i++) if (clock[i] != 12) break;
	if (i == 16) return true;
	return false;
}
void push_button(int button)
{
	for (int i = 0; i < 16; i++)
	{
		if (!clk_switch[button][i]) continue;
		clock[i] += 3;
		if (clock[i] == 15) clock[i] = 3;
	}
}

int DFS(int button)
{
	if (button >= 10)
	{
		return aligned() ? 0 : INF;
	}
	int ret = INF;
	for (int i = 0; i < 4; i++)
	{
		ret = min(ret, i + DFS(button + 1));
		push_button(button);
	}
	return ret;
}


int main()
{
	scanf("%d", &T);
	while (T--)
	{
		for (int i = 0; i < 16; i++) scanf("%d", &clock[i]);
		int ans = DFS(0);
		if (ans == INF) ans = -1;
		printf("%d\n", ans);
	}
}.