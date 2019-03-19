#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define PENALTY 1000000
#define MAX_HASH 987654321

char Data[1024 * 1024 + 5];
int score, DS;
int seed = 1124;

extern void init();
extern void put_char(char c);
extern void put_enter();
extern void backspace();
extern void move_cursor_up(int n);
extern void move_cursor_down(int n);
extern void move_cursor_left(int n);
extern void move_cursor_right(int n);
extern void close(char* d);

int pseudo_rand()
{
	seed = seed * 214013 + 2531011;
	return (seed >> 16) & 0x7FFF;
}

void run(char* d)
{
	DS = 0;
	while (DS < 1024 * 1024 - 1) {
		if (pseudo_rand() % 100 == 0) {
			put_enter();
			DS++;
		}
		if (pseudo_rand() % 100 == 0) {
			switch (pseudo_rand() % 4)
			{
			case 0:
				move_cursor_up(pseudo_rand() % 100);
				break;
			case 1:
				move_cursor_down(pseudo_rand() % 100);
				break;
			case 2:
				move_cursor_left(pseudo_rand() % 100); // in line
				break;
			case 3:
				move_cursor_right(pseudo_rand() % 100); // in line
				break;
			default:
				break;
			}
		}
		if (pseudo_rand() % 100 == 0) {
			backspace();
			DS--;
		}
		put_char('A' + (pseudo_rand() % 26));
		DS++;
	}
	close(d);
}

int verify(char* d) {
	int temp;
	scanf("%d", &temp);

	unsigned int hash = 5381;
	for (int i = 0; i < DS; i++) {
		hash = (((hash << 5) + hash) + d[i]) % MAX_HASH;
	}
	if (temp == hash)  return 1;
	else return 0;
}

int main()
{
	int T;
	//freopen("sample_input.txt", "r", stdin);
	//--------------------//
	int Grading; // This is irrelevant to the problem
	scanf("%d", &Grading);
	//--------------------//

	time_t st = clock();
	scanf("%d", &T);
	for (register int testcase = 0; testcase < T; testcase++)
	{
		init();
		run(Data);
		if (!verify(Data))
			score += PENALTY;
	}
	time_t et = clock();
	score += (et - st) / 1000;
	if (score < 10000) printf("PASS\n");
	else printf("FAIL\n");
	printf("SCORE : %d\n", score);
	return 0;
}