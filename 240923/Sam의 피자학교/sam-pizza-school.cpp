#include<iostream>
#include<string.h>

using namespace std;

int N, K;
int map[101][101];
int copy_map[101][101];
int visit[101][101];
int tmp_map[101][101];
int tmp2_map[101][101];
int max_n, min_n;

int dy[2] = { 0,1 };
int dx[2] = { 1,0 };

void put() {
	for (int i = 0; i < N; i++) {
		if (map[N - 1][i] == min_n) {
			map[N - 1][i]++;
		}
	}
}

void roll() {
	int now_r = 1;
	int now_c = 1;
	int now_last = N;
	int end_y = N - 1;
	int begin_x = 0;
	while (true) {
		int begin_y = end_y - (now_r - 1);
		int end_x = begin_x + (now_c - 1);

		for (int i = 0; i < now_r; i++) {
			for (int j = 0; j < now_c; j++) {
				tmp_map[i][j] = map[begin_y + i][begin_x + j];
			}
		}

		for (int i = 0; i < now_c; i++) {
			for (int j = 0; j < now_r; j++) {
				tmp2_map[i][j] = tmp_map[now_r - j - 1][i];
			}
		}

		for (int i = begin_y; i <= end_y; i++) {
			for (int j = begin_x; j <= end_x; j++) {
				map[i][j] = 0;
			}
		}

		now_last -= now_c;

		for (int i = 0; i < now_c; i++) {
			for (int j = 0; j < now_r; j++) {
				map[end_y - (now_c - i)][end_x + 1 + j] = tmp2_map[i][j];
			}
		}

		memset(tmp_map, 0, sizeof(tmp_map));
		memset(tmp2_map, 0, sizeof(tmp2_map));

		begin_x += now_c;

		if (now_r == now_c) {
			now_r++;
		}
		else {
			now_c++;
		}

		if (now_r > now_last - now_c) {
			break;
		}
	}
}

void press() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (map[i][j] != 0) {
				visit[i][j] = 1;
				int now_n = map[i][j];
				for (int d = 0; d < 2; d++) {
					int now_y = i + dy[d];
					int now_x = j + dx[d];

					if (now_y >= 0 && now_y < N && now_x >= 0 && now_x < N) {
						if (map[now_y][now_x] != 0) {
							int next_n = map[now_y][now_x];
							if (now_n > next_n) {
								int minus = (now_n - next_n) / 5;
								copy_map[i][j] -= minus;
								copy_map[now_y][now_x] += minus;
							}
							else if (now_n < next_n) {
								int minus = (next_n - now_n) / 5;
								copy_map[i][j] += minus;
								copy_map[now_y][now_x] -= minus;
							}
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (copy_map[i][j] != 0) {
				map[i][j] += copy_map[i][j];
			}
		}
	}

	memset(copy_map, 0, sizeof(copy_map));
}

void strech() {
	int now_idx = 0;
	for (int j = 0; j < N; j++) {
		for (int i = N - 1; i >= 0; i--) {
			if (visit[i][j] != 0) {
				copy_map[N - 1][now_idx] = map[i][j];
				now_idx++;
			}
		}
	}
	memset(map, 0, sizeof(map));
	memset(visit, 0, sizeof(visit));

	for (int i = 0; i < N; i++) {
		map[N - 1][i] = copy_map[N - 1][i];
	}
	memset(copy_map, 0, sizeof(copy_map));
}

void fold() {
	int half = N / 2;
	int mid = N / 2;
	for (int i = 0; i < half; i++) {
		map[N - 2][mid + i] = map[N - 1][mid - i - 1];
		map[N - 1][half - i - 1] = 0;
	}

	half /= 2;
	int begin_y = N - 2;
	int end_y = N - 1;
	int begin_x = mid;
	int end_x = mid + half - 1;
	int now_r = end_y - begin_y + 1;
	int now_c = end_x - begin_x + 1;

	for (int i = 0; i < now_r; i++) {
		for (int j = 0; j < now_c; j++) {
			tmp_map[i][j] = map[begin_y + i][begin_x + j];
		}
	}

	for (int i = 0; i < now_r; i++) {
		for (int j = 0; j < now_c; j++) {
			tmp2_map[i][j] = tmp_map[now_r - i - 1][now_c - j - 1];
		}
	}

	for (int i = begin_y; i <= end_y; i++) {
		for (int j = begin_x; j <= end_x; j++) {
			map[i][j] = 0;
		}
	}

	for (int i = 0; i < now_r; i++) {
		for (int j = 0; j < now_c; j++) {
			map[begin_y - (now_r - i)][end_x + 1 + j] = tmp2_map[i][j];
		}
	}

	memset(tmp_map, 0, sizeof(tmp_map));
	memset(tmp2_map, 0, sizeof(tmp2_map));
}

int check() {
	max_n = -1;
	min_n = 987654321;
	for (int i = 0; i < N; i++) {
		if (map[N - 1][i] > max_n) {
			max_n = map[N - 1][i];
		}
		if (map[N - 1][i] < min_n) {
			min_n = map[N - 1][i];
		}
	}

	if (max_n - min_n <= K) {
		return 1;
	}
	else {
		return -1;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	cin >> N >> K;

	max_n = -1;
	min_n = 987654321;
	for (int i = 0; i < N; i++) {
		cin >> map[N - 1][i];
		if (map[N - 1][i] > max_n) {
			max_n = map[N - 1][i];
		}
		if (map[N - 1][i] < min_n) {
			min_n = map[N - 1][i];
		}
	}

	int result = 0;
	while (true) {
		put();
		roll();
		press();
		strech();
		fold();
		press();
		strech();
		result++;
		int flag = check();
		if (flag == 1) {
			break;
		}
	}

	cout << result;

	return 0;
}