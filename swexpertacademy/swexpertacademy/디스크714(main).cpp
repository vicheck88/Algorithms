#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;

static char disk[16][1024][1024];
static char file_name[100][16];
static int file_size[100];

static int disk_id;
static int data_size;

int seed;
int score;
int penalty = 1000000;

void init();
void read_file(char* filename, int offset, char* data, int size);
void insert_file(char* filename, int offset, char* data, int size);
void delete_file(char* filename, int offset, int size);

int pseudo_rand() {
	seed = (214013 * seed + 2531011);
	return (seed >> 16) & 0x7FFF;
}

bool change_disk(int _disk_id) {
	if (_disk_id < 0 || _disk_id >= 16) return false;
	disk_id = _disk_id;
	return true;
}

bool read_disk(int sector_id, char* data) {
	if (sector_id < 0 || sector_id >= 1024) return false;
	memcpy(data, disk[disk_id][sector_id], 1024);
	return true;
}

bool write_disk(int sector_id, char* data) {
	if (sector_id < 0 || sector_id >= 1024) return false;
	memcpy(disk[disk_id][sector_id], data, 1024);
	return true;
}

void _build() {
	init();
	data_size = 0;
	for (int i = 0; i < 100; i++) {
		file_size[i] = 0;
	}
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 16; j++) {
			file_name[i][j] = 'A' + pseudo_rand() % 26;
		}
	}
}

void _insert() {
	char data[4096];
	int size = pseudo_rand() % 4096 + 1;
	for (int i = 0; i < size; i++) {
		data[i] = pseudo_rand() % 255 + 1;
	}
	int fidx = pseudo_rand() % 100;
	int offset = (pseudo_rand() * pseudo_rand()) % (file_size[fidx] + 1);
	insert_file(file_name[fidx], offset, data, size);
	data_size += size;
	file_size[fidx] += size;
}

void _delete() {
	int size = pseudo_rand() % 4096 + 1;
	int fidx = pseudo_rand() % 100;
	int offset = (pseudo_rand() * pseudo_rand()) % (file_size[fidx] + 1);
	if (file_size[fidx] < size + offset) size = file_size[fidx] - offset;
	if (size == 0) return;
	delete_file(file_name[fidx], offset, size);
	data_size -= size;
	file_size[fidx] -= size;
}

#define MAX_TABLE 56941  
unsigned long HASH(char *str, int size) {
	unsigned long hash = 5381;
	for (int i = 0; i < size; i++) {
		int c = str[i]; if (c < 0) c *= -1;
		hash = (((hash << 5) + hash) + c) % MAX_TABLE;
	}

	return hash % MAX_TABLE;
}

void _verify() {
	int size = 4096;
	char data[4096];
	for (int i = 0; i < 100; i++) {
		int ans;
		scanf("%d", &ans);
		int offset = (pseudo_rand()*pseudo_rand()) % file_size[i];
		if (offset + size > file_size[i]) size = file_size[i] - offset;
		read_file(file_name[i], offset, data, size);
		int h = HASH(data, size);
		if (ans != h) {
			score += penalty;
		}

	}
}


int main() {
	seed = 2;
	clock_t s, e;
	s = clock();
	int T = 10;
	for (int t = 1; t <= T; t++) {
		_build();
		int cnt = 0;
		while (data_size < 10 * 1024 * 1024) {
			int r = pseudo_rand() % 10;
			if (r < 6) { _insert(); cnt++; }
			else _delete();

		}
		_verify();
	}
	e = clock();
	score += (e - s) / 1000;
	if (score < 5000) cout << "PASS" << endl;
	else cout << "FAIL" << endl;
	return 0;
}
