#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS 
#endif 

#include <stdio.h> 

enum { USER_INIT, EGG, IMAGO, GET_EGG_CNT };

extern void userInit(int boardSize);
extern void toEgg(int r, int c);
extern void toImago(int r, int c, int d, int L);
extern int getEggCount(int tick);

int process() {
	int queryCnt, menu;
	int r, c, d, L;
	int ans, reVal, tick, score = 100;

	scanf("%d", &queryCnt);

	for (int i = 0; i<queryCnt; ++i) {
		scanf("%d", &menu);
		if (menu == USER_INIT) {
			int boardSize;
			scanf("%d", &boardSize);
			userInit(boardSize);
		}
		else if (menu == EGG) {
			scanf("%d %d", &r, &c);
			toEgg(r, c);
		}
		else if (menu == IMAGO) {
			scanf("%d %d %d %d", &r, &c, &d, &L);
			toImago(r, c, d, L);
		}
		else if (menu == GET_EGG_CNT) {
			scanf("%d %d", &tick, &ans);
			reVal = getEggCount(tick);
			if (reVal != ans) 
				score = 0;
		}
	}
	return score;
}

int main(void) {

	//freopen("input.txt", "r", stdin);

	setbuf(stdout, NULL);

	int tc, TestCase;
	int totalScore = 0, score = 100;

	scanf("%d", &TestCase);
	for (tc = 1; tc <= TestCase; ++tc) {
		score = process();
		printf("#%d : %d\n", tc, score);
		totalScore += score;
	}
	printf("Total Score : %d\n", totalScore);
	return 0;
}

