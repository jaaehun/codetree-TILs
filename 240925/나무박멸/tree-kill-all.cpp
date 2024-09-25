#include<iostream>
#include<string.h>

using namespace std;

int N, K, C;
int map[21][21];
int tmp_map[21][21];
int death_map[21][21];
int kill_map[21][21];
int result;

int dy[4] = { -1,1,0,0 };
int dx[4] = { 0,0,-1,1 };
int dyy[4] = { -1,-1,1,1 };
int dxx[4] = { -1,1,-1,1 };

void grow() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (map[i][j] > 0) {
				int cnt = 0;
				for (int d = 0; d < 4; d++) {
					int now_y = i + dy[d];
					int now_x = j + dx[d];

					if (now_y >= 0 && now_y < N && now_x >= 0 && now_x < N) {
						if (map[now_y][now_x] > 0) {
							cnt++;
						}
					}
				}
				map[i][j] += cnt;
			}
		}
	}
}

void breed() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (map[i][j] > 0) {
				int cnt = 0;
				for (int d = 0; d < 4; d++) {
					int now_y = i + dy[d];
					int now_x = j + dx[d];

					if (now_y >= 0 && now_y < N && now_x >= 0 && now_x < N) {
						if (map[now_y][now_x] == 0 && death_map[now_y][now_x] == 0) {
							cnt++;
						}
					}
				}
				if (cnt != 0) {
					int num = map[i][j] / cnt;
					for (int d = 0; d < 4; d++) {
						int now_y = i + dy[d];
						int now_x = j + dx[d];

						if (now_y >= 0 && now_y < N && now_x >= 0 && now_x < N) {
							if (map[now_y][now_x] == 0 && death_map[now_y][now_x] == 0) {
								tmp_map[now_y][now_x] += num;
							}
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (tmp_map[i][j] > 0) {
				map[i][j] = tmp_map[i][j];
			}
		}
	}

	memset(tmp_map, 0, sizeof(tmp_map));
}

void kill() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (map[i][j] > 0) {
				kill_map[i][j] += map[i][j];
				for (int d = 0; d < 4; d++) {
					for (int s = 1; s <= K; s++) {
						int now_y = i + s * dyy[d];
						int now_x = j + s * dxx[d];

						if (now_y >= 0 && now_y < N && now_x >= 0 && now_x < N) {
							if (map[now_y][now_x] > 0) {
								kill_map[i][j] += map[now_y][now_x];
							}
							else {
								break;
							}
						}
						else {
							break;
						}
					}
				}
			}
		}
	}
	int max_n = -1;
	int max_y = 0;
	int max_x = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (kill_map[i][j] > max_n) {
				max_n = kill_map[i][j];
				max_y = i;
				max_x = j;
			}
		}
	}
	result += max_n;

	map[max_y][max_x] = 0;
	death_map[max_y][max_x] = -C - 1;
	for (int d = 0; d < 4; d++) {
		for (int s = 1; s <= K; s++) {
			int now_y = max_y + s * dyy[d];
			int now_x = max_x + s * dxx[d];

			if (now_y >= 0 && now_y < N && now_x >= 0 && now_x < N) {
				if (map[now_y][now_x] > 0) {
					map[now_y][now_x] = 0;
					death_map[now_y][now_x] = -C - 1;
				}
				else {
					death_map[now_y][now_x] = -C - 1;
					break;
				}
			}
			else {
				break;
			}
		}
	}
	memset(kill_map, 0, sizeof(kill_map));
}

void heal() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (death_map[i][j] < 0) {
				death_map[i][j]++;
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int M;
	cin >> N >> M >> K >> C;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}

	result = 0;
	for (int t = 1; t <= M; t++) {
		grow();
		breed();
		kill();
		heal();
	}

	cout << result;

	return 0;
}