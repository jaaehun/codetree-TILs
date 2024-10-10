#include<iostream>
#include<queue>
#include<cmath>
#include<string.h>

using namespace std;

int N;
int map[65][65];
int tmp_map[65][65];
int visit[65][65];
queue<int> level;
queue<pair<int,int>> q;

int dy[4] = { -1,1,0,0 };
int dx[4] = { 0,0,-1,1 };

void spin_map(int n, int begin_y, int begin_x, int end_y, int end_x) {
	int mid_y = (begin_y + end_y) / 2;
	int mid_x = (begin_x + end_x) / 2;

	for (int i = begin_y; i <= mid_y; i++) {
		for (int j = mid_x + 1; j <= end_x; j++) {
			tmp_map[i][j] = map[i][begin_x + j - (mid_x + 1)];	
		}
	}

	for (int i = mid_y + 1; i <= end_y; i++) {
		for (int j = mid_x + 1; j <= end_x; j++) {
			tmp_map[i][j] = map[begin_y + i - (mid_y + 1)][j];
		}
	}

	for (int i = mid_y + 1; i <= end_y; i++) {
		for (int j = begin_x; j <= mid_x; j++) {
			tmp_map[i][j] = map[i][mid_x + (j - begin_x + 1)];
		}
	}

	for (int i = begin_y; i <= mid_y; i++) {
		for (int j = begin_x; j <= mid_x; j++) {
			tmp_map[i][j] = map[mid_y + (i - begin_y + 1)][j];
		}
	}
}

void spin(int n) {
	int bign = pow(2, n);
	
	for (int i = 0; i < N; i += bign) {
		for (int j = 0; j < N; j += bign) {
			int begin_y = i;
			int begin_x = j;
			int end_y = begin_y + bign - 1;
			int end_x = begin_x + bign - 1;
			spin_map(n, begin_y, begin_x, end_y, end_x);
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			map[i][j] = tmp_map[i][j];
		}
	}
	memset(tmp_map, 0, sizeof(tmp_map));
}

void melt() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (map[i][j] != 0) {
				int cnt = 0;
				for (int d = 0; d < 4; d++) {
					int y = i + dy[d];
					int x = j + dx[d];

					if (y >= 0 && y < N && x >= 0 && x < N) {
						if (map[y][x] > 0) {
							cnt++;
						}
					}
				}
				if (cnt < 3) {
					tmp_map[i][j] = -1;
				}
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (tmp_map[i][j] == -1) {
				map[i][j]--;
			}
		}
	}
	memset(tmp_map, 0, sizeof(tmp_map));
}

int bfs(int start_y, int start_x) {
	int cnt = 1;
	q.push({ start_y,start_x });
	visit[start_y][start_x] = 1;

	while (!q.empty()) {
		int now_y = q.front().first;
		int now_x = q.front().second;

		q.pop();

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y >= 0 && y < N && x >= 0 && x < N) {
				if (map[y][x] > 0 && visit[y][x] == 0){
					cnt++;
					q.push({ y,x });
					visit[y][x] = 1;
				}
			}
		}
	}

	return cnt;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int Q;

	cin >> N >> Q;
	N = pow(2, N);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}

	for (int i = 0; i < Q; i++) {
		int a;
		cin >> a;
		level.push(a);
	}

	while (!level.empty()) {
		int now = level.front();
		level.pop();

		if (now != 0) {
			spin(now);
		}
		melt();
	}

	int sum = 0;
	int max_cnt = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (map[i][j] > 0) {
				sum += map[i][j];
				if (visit[i][j] == 0) {
					int gcnt = bfs(i, j);
					if (gcnt > max_cnt) {
						max_cnt = gcnt;
					}
				}
			}
		}
	}
	memset(visit, 0, sizeof(visit));

	cout << sum << "\n" << max_cnt;

	return 0;
}