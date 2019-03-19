#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MIN_H 64
#define MAX_H 128
#define MAX_R 256
static char arr1[MAX_H + 1];
static char arr2[MAX_H + 1];
static char ans[MIN_H + 1];
static char result[MAX_R + 1];

extern void cal(char* result, const char* arr1, const char* arr2, int d);

static int getChar(int d) {
	int a = rand() % d;
	if (a < 10) {
		a += 48;
	}
	else {
		a += 55;
	}
	return a;
}

static int make_data(char* a, int d) {
	int i, len;
	len = (rand() % (MAX_H - MIN_H)) + MIN_H;
	for (i = 0; i < len; i++) {
		a[i] = getChar(d);
	}
	while (a[0] == '0') a[0] = getChar(d);
	a[len] = 0;
	return len;
}

static char plus(char a, char b, int d)
{
	a -= (a < 65) ? 48 : 55;
	b -= (b < 65) ? 48 : 55;
	char c = (a + b) % d;
	c += (c < 10) ? 48 : 55;
	return c;
}

int main() {
	int T, i, a, b, d;
	clock_t start, end;
	srand(3);
	start = clock();
	for (T = 1; T <= 50; T++) {
		for (i = 0; i < 64; i++) ans[i] = '0';
		for (i = 0; i < 10000; i++) {
			d = rand() % 35 + 2;
			a = make_data(arr1, d);
			b = make_data(arr2, d);
			cal(result, arr1, arr2, d);
			ans[i % 64] = plus(ans[i % 64], result[rand() % (a + b - 1)], d);
		}
		printf("#%d %s\n", T, ans);
	}
	end = clock();
	printf("Process Time : %.3f", (double)(end - start) / CLOCKS_PER_SEC);

	return 0;
}

