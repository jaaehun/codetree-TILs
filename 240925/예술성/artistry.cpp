#include<iostream>
#include<vector>
#include<set>
#include<queue>
#include<string.h>

using namespace std;

struct INFO {
	int num;
	int cnt;
	int y;
	int x;
};

int N;
int result;
int map[30][30];
int tmp_map[30][30];
int group_map[30][30];
int group_n;
int visit[30][30];
vector<INFO> group_info;
set<int> group_near;
queue<pair<int, int>> q;

int dy[4] = { -1,1,0,0 };
int dx[4] = { 0,0,-1,1 };

void bfs(int start_y, int start_x) {
	q.push({ start_y,start_x });
	visit[start_y][start_x] = 1;
	group_map[start_y][start_x] = group_n;
	int target = map[start_y][start_x];
	int cnt = 1;

	while (!q.empty()) {
		int now_y = q.front().first;
		int now_x = q.front().second;

		q.pop();

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y >= 0 && y < N && x >= 0 && x < N) {
				if (map[y][x] == target && visit[y][x] == 0) {
					q.push({ y,x });
					visit[y][x] = 1;
					group_map[y][x] = group_n;
					cnt++;
				}
			}
		}
	}
	group_info.push_back({ target,cnt,start_y,start_x });
}

void make_group() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (visit[i][j] == 0) {
				bfs(i, j);
				group_n++;
			}
		}
	}
	memset(visit, 0, sizeof(visit));
}

void bfs2(int start_y, int start_x) {
	q.push({ start_y,start_x });
	visit[start_y][start_x] = 1;
	int target = group_map[start_y][start_x];

	while (!q.empty()) {
		int now_y = q.front().first;
		int now_x = q.front().second;

		q.pop();

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y >= 0 && y < N && x >= 0 && x < N) {
				if (group_map[y][x] == target) {
					if (visit[y][x] == 0) {
						q.push({ y,x });
						visit[y][x] = 1;
					}
				}
				else {
					if (group_map[y][x] > target) {
						group_near.insert(group_map[y][x]);
					}
				}
			}
		}
	}
	memset(visit, 0, sizeof(visit));
}

int calculate(int start_y, int start_x, int new_n) {
	q.push({ start_y,start_x });
	visit[start_y][start_x] = 1;
	int target = group_map[start_y][start_x];
	int cnt = 0;

	while (!q.empty()) {
		int now_y = q.front().first;
		int now_x = q.front().second;

		q.pop();

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y >= 0 && y < N && x >= 0 && x < N) {
				if (group_map[y][x] == target) {
					if (visit[y][x] == 0) {
						q.push({ y,x });
						visit[y][x] = 1;
					}
				}
				else {
					if (group_map[y][x] == new_n) {
						cnt++;
					}
				}
			}
		}
	}
	memset(visit, 0, sizeof(visit));

	return cnt;
}

void get_score() {
	for (int v = 0; v < group_info.size() - 1; v++) {
		int start_y = group_info[v].y;
		int start_x = group_info[v].x;

		bfs2(start_y, start_x);

		if (group_near.size() != 0) {
			auto iter = group_near.begin();
			while (true) {
				int new_n = *iter;
				int cnt = calculate(start_y, start_x, new_n);
				result += (group_info[v].cnt + group_info[new_n].cnt) * group_info[v].num * group_info[new_n].num * cnt;
				advance(iter, 1);
				if (iter == group_near.end()) {
					break;
				}
			}
			group_near.clear();
		}
	}
	memset(group_map, 0, sizeof(group_map));
	group_info.clear();
}

void mid_spin() {
	for (int i = 0; i < N; i++) {
		tmp_map[N / 2][i] = map[i][N / 2];
		tmp_map[i][N / 2] = map[N / 2][N - i - 1];
	}
}

void spin(int begin_y, int begin_x, int end_y, int end_x) {
	int n = end_y - begin_y + 1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			tmp_map[begin_y + i][end_x - j] = map[begin_y + j][begin_x + i];
		}
	}
}

void small_spin() {
	spin(0, 0, N / 2 - 1, N / 2 - 1);
	spin(0, N / 2 + 1, N / 2 - 1, N - 1);
	spin(N / 2 + 1, 0, N - 1, N / 2 - 1);
	spin(N / 2 + 1, N / 2 + 1, N - 1, N - 1);
}

void make_map() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			map[i][j] = tmp_map[i][j];
		}
	}
	memset(tmp_map, 0, sizeof(tmp_map));
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}

	result = 0;
	for (int t = 0; t <= 3; t++) {
		group_n = 0;
		make_group();
		get_score();
		mid_spin();
		small_spin();
		make_map();
	}

	cout << result;

	return 0;
}