#include <stdio.h>

#include <stdlib.h>

#include <string.h>



enum {

	Add = 1,

	Del = 2,

	Mov = 3,

	Cnt = 4

};



extern void init_user();

extern void Add_child(char name[]);

extern int Delete(char name[]);

extern void Move(char name[]);

extern int Count(char name[]);



//int main()
//
//{
//
//	int T, N, order;
//
//	char name[15];
//
//	scanf("%d", &T);
//
//	for (int i = 0; i < T; i++) {
//
//		scanf("%d", &N);
//
//		init_user();
//
//		while (N--) {
//
//			scanf("%d", &order);
//
//			switch (order) {
//
//			case Add:
//
//				scanf("%s", name);
//
//				Add_child(name);
//
//				break;
//
//			case Del:
//
//				scanf("%s", name);
//
//				printf("%d\n", Delete(name));
//
//				break;
//
//			case Mov:
//
//				scanf("%s", name);
//
//				Move(name);
//
//				break;
//
//			case Cnt:
//
//				scanf("%s", name);
//
//				printf("%d\n", Count(name));
//
//			}
//
//		}
//
//	}
//
//	return 0;
//
//}

//  data 생성 테스트 프로그램

void create(int order, char name[])

{

	for (int i = 0; i < 12; i++) name[i] = rand() % 26 + 'a';

	if (order < 2) name[rand() % 6 + 6] = 0;

	else name[rand() % 5 + 1] = 0;

}



int main()

{

	srand(5);

	int T, N, tmp, order, cnt[4];

	char name[15];

	T = 2;

	for (int tc = 0; tc < T; tc++) {

		cnt[0] = cnt[1] = 20000; cnt[2] = 200;

		N = 20000 + rand() % 20001;

		init_user();

		while (N--) {

			order = rand() % 402 / 100;

			if (order == 0) order = 1;

			if (cnt[order / 2] <= 0) continue;

			cnt[order / 2]--;

			switch (order) {

			case Add:

				create(order, name);

				Add_child(name);

				break;

			case Del:

				if (rand() % 3) create(order, name);

				else strcpy(name, "ALL");

				printf("%d\n", Delete(name));

				break;

			case Mov:

				tmp = rand() % 9;

				if (tmp == 0) strcpy(name, "root");

				else if (tmp < 3) strcpy(name, "parent");

				else if (tmp < 5) strcpy(name, "first");

				else create(order, name);

				Move(name);

				break;

			case Cnt:

				if (rand() % 3) create(order, name);

				else strcpy(name, "ALL");

				printf("%d\n", Count(name));

			}

		}

	}

}

