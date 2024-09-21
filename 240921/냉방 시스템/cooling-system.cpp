#include<iostream>
#include<queue>
#include<string.h>

using namespace std;

struct WALL {
	int left;
	int up;
	int right;
	int down;
};

int N, K;
int map[21][21];
int visit[21][21];
WALL wall_map[21][21];
int cool_map[21][21];
int tmp_map[21][21];
queue<pair<pair<int, int>, int>> q;

int dy[4] = { 0,-1,0,1 };
int dx[4] = { -1,0,1,0 };

int turn_left(int d) {
	if (d == 0) {
		return 3;
	}
	else {
		return d - 1;
	}
}

int turn_right(int d) {
	if (d == 3) {
		return 0;
	}
	else {
		return d + 1;
	}
}

int possible(int now_y, int now_x, int d) {
	if (d == 0) {
		return wall_map[now_y][now_x].left;
	}
	else if (d == 1) {
		return wall_map[now_y][now_x].up;
	}
	else if (d == 2) {
		return wall_map[now_y][now_x].right;
	}
	else {
		return wall_map[now_y][now_x].down;
	}
}

void blow(int start_y, int start_x, int d) {
	start_y += dy[d];
	start_x += dx[d];
	q.push({ {start_y,start_x},5 });
	visit[start_y][start_x] = 1;
	cool_map[start_y][start_x] += 5;

	while (!q.empty()) {
		int now_y = q.front().first.first;
		int now_x = q.front().first.second;
		int now_n = q.front().second;

		q.pop();

		if (now_n > 1) {
			int now_d = d;
			int y = now_y;
			int x = now_x;
			now_d = turn_left(now_d);
			int flag = possible(y, x, now_d);
			if (flag == 0) {
				y += dy[now_d];
				x += dx[now_d];
				if (y > 0 && y <= N && x > 0 && x <= N) {
					if (visit[y][x] == 0) {
						now_d = turn_right(now_d);
						int flag2 = possible(y, x, now_d);
						if (flag2 == 0) {
							y += dy[now_d];
							x += dx[now_d];
							if (y > 0 && y <= N && x > 0 && x <= N) {
								if (visit[y][x] == 0) {
									cool_map[y][x] += now_n - 1;
									q.push({ {y,x},now_n - 1 });
									visit[y][x] = 1;
								}
							}
						}
					}
				}
			}

			now_d = d;
			y = now_y;
			x = now_x;
			flag = possible(y, x, now_d);
			if (flag == 0) {
				y += dy[now_d];
				x += dx[now_d];
				if (y > 0 && y <= N && x > 0 && x <= N) {
					if (visit[y][x] == 0) {
						cool_map[y][x] += now_n - 1;
						q.push({ {y,x},now_n - 1 });
						visit[y][x] = 1;
					}
				}
			}

			now_d = d;
			y = now_y;
			x = now_x;
			now_d = turn_right(now_d);
			flag = possible(y, x, now_d);
			if (flag == 0) {
				y += dy[now_d];
				x += dx[now_d];
				if (y > 0 && y <= N && x > 0 && x <= N) {
					if (visit[y][x] == 0) {
						now_d = turn_left(now_d);
						int flag2 = possible(y, x, now_d);
						if (flag2 == 0) {
							y += dy[now_d];
							x += dx[now_d];
							if (y > 0 && y <= N && x > 0 && x <= N) {
								if (visit[y][x] == 0) {
									cool_map[y][x] += now_n - 1;
									q.push({ {y,x},now_n - 1 });
									visit[y][x] = 1;
								}
							}
						}
					}
				}
			}
		}
	}
	memset(visit, 0, sizeof(visit));
}

void cool() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j] > 1) {
				blow(i, j, map[i][j] - 2);
			}
		}
	}
}

void mix() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			int now_y = i;
			int now_x = j;
			int now_n = cool_map[now_y][now_x];

			int next_y = now_y + dy[2];
			int next_x = now_x + dx[2];
			if (next_y > 0 && next_y <= N && next_x > 0 && next_x <= N) {
				if (wall_map[now_y][now_x].right == 0) {
					int next_n = cool_map[next_y][next_x];
					if (now_n > next_n) {
						int minus = (now_n - next_n) / 4;
						tmp_map[now_y][now_x] -= minus;
						tmp_map[next_y][next_x] += minus;
					}
					else if (now_n < next_n) {
						int minus = (next_n - now_n) / 4;
						tmp_map[now_y][now_x] += minus;
						tmp_map[next_y][next_x] -= minus;
					}
				}
			}

			next_y = now_y + dy[3];
			next_x = now_x + dx[3];
			if (next_y > 0 && next_y <= N && next_x > 0 && next_x <= N) {
				if (wall_map[now_y][now_x].down == 0) {
					int next_n = cool_map[next_y][next_x];
					if (now_n > next_n) {
						int minus = (now_n - next_n) / 4;
						tmp_map[now_y][now_x] -= minus;
						tmp_map[next_y][next_x] += minus;
					}
					else if (now_n < next_n) {
						int minus = (next_n - now_n) / 4;
						tmp_map[now_y][now_x] += minus;
						tmp_map[next_y][next_x] -= minus;
					}
				}
			}
		}
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (tmp_map != 0) {
				cool_map[i][j] += tmp_map[i][j];
			}
		}
	}
	
	memset(tmp_map, 0, sizeof(tmp_map));
}

void hot() {
	for (int i = 1; i <= N; i++) {
		if (cool_map[1][i] != 0) {
			cool_map[1][i]--;
		}
	}
	for (int i = 2; i <= N; i++) {
		if (cool_map[i][1] != 0) {
			cool_map[i][1]--;
		}
		if (cool_map[i][N] != 0) {
			cool_map[i][N]--;
		}
	}
	for (int i = 2; i < N; i++) {
		if (cool_map[N][i] != 0) {
			cool_map[N][i]--;
		}
	}
}

int check() {
	int flag = 0;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j] == 1) {
				if (cool_map[i][j] < K) {
					flag = 1;
					break;
				}
			}
		}
		if (flag == 1) {
			break;
		}
	}

	return flag;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int M;
	cin >> N >> M >> K;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> map[i][j];
		}
	}

	for (int i = 0; i < M; i++) {
		int y, x, s;
		cin >> y >> x >> s;
		if (s == 0) {
			wall_map[y][x].up = 1;
			wall_map[y - 1][x].down = 1;
		}
		else {
			wall_map[y][x].left = 1;
			wall_map[y][x - 1].right = 1;
		}
	}

	int time = 0;
	while (true) {
		cool();
		mix();
		hot();
		time++;
		if (time > 100) {
			time = -1;
			break;
		}
		int flag = check();
		if (flag == 0) {
			break;
		}
	}

	cout << time;

	return 0;
}