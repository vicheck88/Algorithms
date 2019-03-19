
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>

extern void initUser(int N);
extern void makeLabRoom(int id, int stR, int stC, int h, int w, int evr, int evc);
extern int getShortestPath(int s, int e);

typedef enum
{
	INIT_ORDER,
	ADD_ORDER,
	GET_SP_ORDER,
	FINISH_ORDER
} ORDER;

static void process()
{
	int N, order;
	int LabRoomID, startR, startC;
	int height, width, evR, evC;
	int startNum, arrivalNum, userResult;

	scanf("%d", &N);

	initUser(N);
	while (true)
	{
		scanf("%d", &order);
		if (order == ADD_ORDER)
		{
			scanf("%d %d %d", &LabRoomID, &startR, &startC);
			scanf("%d %d", &height, &width);
			scanf("%d %d", &evR, &evC);
			makeLabRoom(LabRoomID, startR, startC,
				height, width, evR, evC);
		}
		else if (order == GET_SP_ORDER)
		{
			scanf("%d %d", &startNum, &arrivalNum);
			userResult = getShortestPath(startNum, arrivalNum);
			printf("%d\n", userResult);
		}
		else if (order == FINISH_ORDER)
			break;
	}

}

int main()
{
	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	int testCase, tc;

	scanf("%d", &testCase);

	for (tc = 1; tc <= testCase; tc++)
	{
		process();
	}
	return 0;
}

