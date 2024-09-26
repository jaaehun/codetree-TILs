#include<iostream>
#include<vector>
#include<string.h>
#include<queue>

using namespace std;

int N;
vector<int> map[16][16];
vector<int> tmp_map[16][16];
int basecamp_map[16][16];
vector<pair<int, int>> store;
int possible_map[16][16];
int visit[16][16];
int back_y[16][16];
int back_x[16][16];
int goal_cnt;
queue<pair<int, int>> q;
queue<pair<pair<int, int>, int>> q2;

int dy[4] = { -1,0,0,1 };
int dx[4] = { 0,-1,1,0 };

void bfs(int start_y, int start_x, int num) {
	q.push({ start_y,start_x });
	visit[start_y][start_x] = 1;

	int target_y = store[num].first;
	int target_x = store[num].second;
	while (!q.empty()) {
		int now_y = q.front().first;
		int now_x = q.front().second;

		q.pop();

		if (now_y == target_y && now_x == target_x) {
			while (!q.empty()) {
				q.pop();
			}
			break;
		}

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y > 0 && y <= N && x > 0 && x <= N) {
				if (visit[y][x] == 0 && possible_map[y][x] >= 0) {
					q.push({ y,x });
					visit[y][x] = 1;
					back_y[y][x] = now_y;
					back_x[y][x] = now_x;
				}
			}
		}
	}

	memset(visit, 0, sizeof(visit));
}

void move_player() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j].size() != 0) {
				for (int v = 0; v < map[i][j].size(); v++) {
					int num = map[i][j][v];
					int target_y = store[num].first;
					int target_x = store[num].second;
					bfs(i, j, num);

					int now_y = target_y;
					int now_x = target_x;
					int now_d = 0;
					while (true) {
						int next_y = back_y[now_y][now_x];
						int next_x = back_x[now_y][now_x];

						if (next_y == i && next_x == j) {
							for (int d = 0; d < 4; d++) {
								int y = next_y + dy[d];
								int x = next_x + dx[d];

								if (y == now_y && x == now_x) {
									now_d = d;
									break;
								}
							}
							break;
						}

						now_y = next_y;
						now_x = next_x;
					}
					memset(back_y, 0, sizeof(back_y));
					memset(back_x, 0, sizeof(back_x));

					int next_y = i + dy[now_d];
					int next_x = j + dx[now_d];

					if (next_y == target_y && next_x == target_x) {
						goal_cnt++;
						possible_map[next_y][next_x] = 1;
					}
					else {
						tmp_map[next_y][next_x].push_back(num);
					}
				}
			}
		}
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			map[i][j] = tmp_map[i][j];
			tmp_map[i][j].clear();

			if (possible_map[i][j] == 1) {
				possible_map[i][j] = -1;
			}
		}
	}
}

void bfs2(int start_y, int start_x, int num) {
	q2.push({ {start_y,start_x},0 });
	visit[start_y][start_x] = 1;

	int min_cnt = 0;
	int min_y = 0;
	int min_x = 0;
	while (!q2.empty()) {
		int now_y = q2.front().first.first;
		int now_x = q2.front().first.second;
		int cnt = q2.front().second;

		q2.pop();

		if (min_cnt != 0) {
			if (cnt == min_cnt) {
				if (now_y < min_y) {
					min_y = now_y;
					min_x = now_x;
				}
				else if (now_y == min_y) {
					if (now_x < min_x) {
						min_y = now_y;
						min_x = now_x;
					}
				}
			}
			else if (cnt > min_cnt) {
				while (!q2.empty()) {
					q2.pop();
				}
				break;
			}
		}

		if (basecamp_map[now_y][now_x] != 0 && min_cnt == 0) {
			min_cnt = cnt;
			min_y = now_y;
			min_x = now_x;
		}

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y > 0 && y <= N && x > 0 && x <= N) {
				if (visit[y][x] == 0 && possible_map[y][x] >= 0) {
					q2.push({ {y,x},cnt + 1 });
					visit[y][x] = 1;
				}
			}
		}
	}

	basecamp_map[min_y][min_x] = 0;
	map[min_y][min_x].push_back(num);
	possible_map[min_y][min_x] = -1;

	memset(visit, 0, sizeof(visit));
}

void go_basecamp(int n){
	int target_y = store[n].first;
	int target_x = store[n].second;

	bfs2(target_y, target_x, n);
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int M;
	cin >> N >> M;

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> basecamp_map[i][j];
		}
	}

	store.push_back({ 0,0 });
	for (int i = 1; i <= M; i++) {
		int y, x;
		cin >> y >> x;
		store.push_back({ y,x });
	}

	goal_cnt = 0;
	int time = 0;
	while (true) {
		time++;
		move_player();
		if (goal_cnt == M) {
			break;
		}
		if (time <= M) {
			go_basecamp(time);
		}
	}

	cout << time;

	return 0;
}