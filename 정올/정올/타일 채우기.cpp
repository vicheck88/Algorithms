#include <stdio.h>
#define UPPERLEFT 0
#define UPPERRIGHT 1
#define LOWERLEFT 2
#define LOWERRIGHT 3

int N;
int R, C;
int map[512][512];
int tile[4][2][2]=
{
{{0,1},{1,1}},
{{2,0},{2,2}},
{{3,3},{0,3}},
{{4,4},{4,0}}
};
void fill(int sr, int sc, int er, int ec,int R,int C);
int main()
{
	scanf("%d", &N);
	scanf("%d %d", &R, &C);
	fill(0, 0, N - 1, N - 1,R,C);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++) printf("%d ", map[i][j]);
		printf("\n");
	}
}
void put(int mr, int mc, int dir);
void fill(int sr, int sc, int er, int ec,int R,int C)
{
	if (sr == er || sc == ec) return;
	int mr = (sr + er) / 2, mc = (sc + ec) / 2;
	if (R <= mr)
	{
		if (C <= mc)
		{
			put(mr, mc, UPPERLEFT);
			fill(sr, sc, mr, mc, R, C);
			fill(sr, mc + 1, mr, ec, mr, mc + 1);
		}
		else
		{
			put(mr, mc, UPPERRIGHT);
			fill(sr, sc, mr, mc, mr, mc);
			fill(sr, mc + 1, mr, ec, R, C);
		}
		fill(mr + 1, sc, er, mc, mr + 1, mc);
		fill(mr + 1, mc + 1, er, ec, mr + 1, mc + 1);
	}
	else
	{
		if (C <= mc)
		{
			put(mr, mc, LOWERLEFT);
			fill(mr + 1, sc, er, mc, R, C);
			fill(mr + 1, mc + 1, er, ec, mr + 1, mc + 1);
		}
		else
		{
			put(mr, mc, LOWERRIGHT);
			fill(mr + 1, sc, er, mc, mr + 1, mc);
			fill(mr + 1, mc + 1, er, ec, R, C);
		}
		fill(sr, sc, mr, mc, mr, mc);
		fill(sr, mc + 1, mr, ec, mr, mc + 1);
	}
}
void put(int mr, int mc, int dir)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if(map[i + mr][j + mc]==0 ) 
				map[i+mr][j+mc]= tile[dir][i][j];
		}
	}
}