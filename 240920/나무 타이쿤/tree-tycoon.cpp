#include<iostream>
#include<string.h>

using namespace std;

int N;
int map[16][16];
int medicine_map[16][16];
int copy_map[16][16];

int dy[9] = { 0,0,-1,-1,-1,0,1,1,1 };
int dx[9] = { 0,1,1,0,-1,-1,-1,0,1 };
int dyy[4] = { -1,-1,1,1 };
int dxx[4] = { -1,1,-1,1 };

void move(int d, int p) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (medicine_map[i][j] == 1) {
				int y = (i + p * dy[d] + 2 * N) % N;
				int x = (j + p * dx[d] + 2 * N) % N;

				copy_map[y][x] = 1;
			}
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			medicine_map[i][j] = copy_map[i][j];
		}
	}

	memset(copy_map, 0, sizeof(copy_map));
}

void put() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (medicine_map[i][j] == 1) {
				map[i][j]++;
			}
		}
	}
}

void grow() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (medicine_map[i][j] == 1) {
				int cnt = 0;
				for (int d = 0; d < 4; d++) {
					int y = i + dyy[d];
					int x = j + dxx[d];
					if (y >= 0 && y < N && x >= 0 && x < N) {
						if (map[y][x] >= 1) {
							cnt++;
						}
					}
				}
				map[i][j] += cnt;
			}
		}
	}
}

void buy() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (map[i][j] >= 2 && medicine_map[i][j] == 0) {
				map[i][j] -= 2;
				copy_map[i][j] = 1;
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			medicine_map[i][j] = copy_map[i][j];
		}
	}

	memset(copy_map, 0, sizeof(copy_map));
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int M;
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}
	for (int i = N - 2; i < N; i++) {
		for (int j = 0; j < 2; j++) {
			medicine_map[i][j] = 1;
		}
	}

	for (int i = 0; i < M; i++) {
		int d, p;
		cin >> d >> p;
		move(d, p);
		put();
		grow();
		buy();
	}

	int result = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (map[i][j] > 0) {
				result += map[i][j];
			}
		}
	}

	cout << result;

	return 0;
}