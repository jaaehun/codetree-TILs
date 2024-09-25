#include<iostream>
#include<string.h>
#include<queue>

using namespace std;

int N;
int map[21][21];
int tmp_map[21][21];
int visit[21][21];
int visit2[21][21];
int result;
int ball_y, ball_x, ball_d;
queue<pair<pair<int, int>, int>> q;

int dy[4] = { 0,-1,0,1 };
int dx[4] = { 1,0,-1,0 };

int turn_left(int d) {
	if (d == 3) {
		return 0;
	}
	else {
		return d + 1;
	}
}

void move_player() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j] > 0) {
				if (map[i][j] == 1 || map[i][j] == 2 || map[i][j] == 3) {
					visit[i][j] = 1;
				}
				visit2[i][j] = 1;
			}
		}
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j] == 1) {
				int flag = 0;
				for (int d = 0; d < 4; d++) {
					int now_y = i + dy[d];
					int now_x = j + dx[d];

					if (now_y > 0 && now_y <= N && now_x > 0 && now_x <= N) {
						if (map[now_y][now_x] == 4) {
							flag = 1;
							tmp_map[now_y][now_x] = 1;
						}
					}
				}
				if (flag == 0) {
					for (int d = 0; d < 4; d++) {
						int now_y = i + dy[d];
						int now_x = j + dx[d];

						if (now_y > 0 && now_y <= N && now_x > 0 && now_x <= N) {
							if (map[now_y][now_x] == 3) {
								tmp_map[now_y][now_x] = 1;
							}
						}
					}
				}
			}
			if (map[i][j] == 3) {
				int flag = 0;
				for (int d = 0; d < 4; d++) {
					int now_y = i + dy[d];
					int now_x = j + dx[d];

					if (now_y > 0 && now_y <= N && now_x > 0 && now_x <= N) {
						if (map[now_y][now_x] == 2) {
							flag = 1;
							tmp_map[now_y][now_x] = 3;
							visit[i][j] = 0;
						}
					}
				}
				if (flag == 0) {
					for (int d = 0; d < 4; d++) {
						int now_y = i + dy[d];
						int now_x = j + dx[d];

						if (now_y > 0 && now_y <= N && now_x > 0 && now_x <= N) {
							if (map[now_y][now_x] == 1) {
								tmp_map[now_y][now_x] = 3;
							}
						}
					}
				}
			}
		}
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (visit[i][j] == 1 && tmp_map[i][j] == 0) {
				tmp_map[i][j] = 2;
			}
		}
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (visit2[i][j] == 1 && tmp_map[i][j] == 0) {
				tmp_map[i][j] = 4;
			}
		}
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			map[i][j] = tmp_map[i][j];
		}
	}

	memset(tmp_map, 0, sizeof(tmp_map));
	memset(visit, 0, sizeof(visit));
	memset(visit2, 0, sizeof(visit2));
}

void select_ball(int round) {
	int n = round % N;
	if (n == 0) {
		if (ball_d == 0) {
			ball_y = N;
			ball_x = 1;
		}
		else if (ball_d == 1) {
			ball_y = N;
			ball_x = N;
		}
		else if (ball_d == 2) {
			ball_y = 1;
			ball_x = N;
		}
		else {
			ball_y = 1;
			ball_x = 1;
		}
		ball_d = turn_left(ball_d);
	}
	else {
		if (ball_d == 0) {
			ball_y = n;
			ball_x = 1;
		}
		else if (ball_d == 1) {
			ball_y = N;
			ball_x = n;
		}
		else if (ball_d == 2) {
			ball_y = N - n + 1;
			ball_x = N;
		}
		else {
			ball_y = 1;
			ball_x = N - n + 1;
		}
	}
}

int bfs(int start_y, int start_x) {
	int num = 1;
	q.push({ {start_y,start_x}, 1 });
	visit[start_y][start_x] = 1;

	if (map[start_y][start_x] == 1) {
		tmp_map[start_y][start_x] = 3;
	}
	else if (map[start_y][start_x] == 3) {
		tmp_map[start_y][start_x] = 1;
	}

	while (!q.empty()) {
		int now_y = q.front().first.first;
		int now_x = q.front().first.second;
		int cnt = q.front().second;

		q.pop();

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y > 0 && y <= N && x > 0 && x <= N) {
				if (visit[y][x] == 0) {
					if (map[now_y][now_x] != 3 && map[y][x] == 1) {
						num = cnt + 1;
						visit[y][x] = 1;
						tmp_map[y][x] = 3;
					}
					else if (map[y][x] == 2) {
						q.push({ {y,x},cnt + 1 });
						visit[y][x] = 1;
					}
					else if (map[y][x] == 3) {
						visit[y][x];
						tmp_map[y][x] = 1;
					}
				}
			}
		}
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (tmp_map[i][j] != 0) {
				map[i][j] = tmp_map[i][j];
			}
		}
	}

	memset(visit, 0, sizeof(visit));
	memset(tmp_map, 0, sizeof(tmp_map));

	return num;
}

void throw_ball() {
	for (int i = 0; i < N; i++) {
		int now_y = ball_y + i * dy[ball_d];
		int now_x = ball_x + i * dx[ball_d];

		if (map[now_y][now_x] == 1 || map[now_y][now_x] == 2 || map[now_y][now_x] == 3) {
			int cnt = bfs(now_y, now_x);
			result += cnt * cnt;
			break;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int M, K;
	cin >> N >> M >> K;

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> map[i][j];
		}
	}

	result = 0;
	for (int t = 1; t <= K; t++) {
		move_player();
		select_ball(t);
		throw_ball();
	}

	cout << result;

	return 0;
}