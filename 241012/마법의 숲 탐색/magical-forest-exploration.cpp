#include<iostream>
#include<string.h>
#include<queue>

using namespace std;

int R, C;
int map[71][71];
int visit[71][71];
int result;
queue<pair<pair<int, int>, int>> q;

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

int turn_right(int d) {
	if (d == 3) {
		return 0;
	}
	else {
		return d + 1;
	}
}

int turn_left(int d) {
	if (d == 0) {
		return 3;
	}
	else {
		return d - 1;
	}
}

int move_player(int start_y, int start_x) {
	int max_y = 0;
	int num = map[start_y][start_x];
	q.push({ {start_y,start_x},num });
	visit[start_y][start_x] = 1;

	while (!q.empty()) {
		int now_y = q.front().first.first;
		int now_x = q.front().first.second;
		int now_n = q.front().second;

		q.pop();

		if (now_y > max_y) {
			max_y = now_y;
		}

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y > 0 && y <= R && x > 0 && x <= C) {
				if (map[y][x] != 0 && visit[y][x] == 0) {
					if (now_n != -1) {
						if (map[y][x] > 0) {
							if (map[y][x] == now_n) {
								q.push({ {y,x},now_n });
								visit[y][x] = 1;
							}
						}
						else {
							if (map[y][x] == -now_n) {
								q.push({ {y,x},-1 });
								visit[y][x] = 1;
							}
						}
					}
					else {
						if (map[y][x] > 0) {
							q.push({ {y,x},map[y][x] });
							visit[y][x] = 1;
						}
						else {
							q.push({ {y,x},-1 });
							visit[y][x] = 1;
						}
					}
				}
			}
		}
	}

	memset(visit, 0, sizeof(visit));

	return max_y;
}

void fix_position(int num, int fix_y, int fix_x, int fix_d) {
	map[fix_y][fix_x] = num;
	for (int i = 0; i < 4; i++) {
		int now_y = fix_y + dy[i];
		int now_x = fix_x + dx[i];

		if (i == fix_d) {
			map[now_y][now_x] = -num;
		}
		else {
			map[now_y][now_x] = num;
		}
	}

	int deep = move_player(fix_y, fix_x);
	result += deep;
}

void go_down(int num, int start_y, int start_x, int start_d) {
	int now_y = start_y;
	int now_x = start_x;
	int now_d = start_d;
	while (true) {
		if (map[now_y + 1][now_x - 1] != 0 || map[now_y + 1][now_x + 1] != 0 || map[now_y + 2][now_x] != 0) {
			if (now_x <= 2 || map[now_y - 1][now_x - 1] != 0 || map[now_y][now_x - 2] != 0 || map[now_y + 1][now_x - 1] != 0 || map[now_y + 1][now_x - 2] != 0 || map[now_y + 1][now_x] != 0 || map[now_y + 2][now_x - 1] != 0) {
				if (now_x >= C - 1 || map[now_y - 1][now_x + 1] != 0 || map[now_y][now_x + 2] || map[now_y + 1][now_x + 1] != 0 || map[now_y + 1][now_x] != 0 || map[now_y + 2][now_x + 1] != 0 || map[now_y + 1][now_x + 2] != 0) {
					if (now_y < 2) {
						memset(map, 0, sizeof(map));
					}
					else {
						fix_position(num, now_y, now_x, now_d);
					}
					break;
				}
				else {
					now_x++;
					now_y++;
					now_d = turn_right(now_d);
					if (now_y == R - 1) {
						fix_position(num, now_y, now_x, now_d);
						break;
					}
				}
			}
			else {
				now_x--;
				now_y++;
				now_d = turn_left(now_d);
				if (now_y == R - 1) {
					fix_position(num, now_y, now_x, now_d);
					break;
				}
			}
		}
		else {
			now_y++;
			if (now_y == R - 1) {
				fix_position(num, now_y, now_x, now_d);
				break;
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int K;
	cin >> R >> C >> K;

	result = 0;
	for (int k = 1; k <= K; k++) {
		int c, d;
		cin >> c >> d;
		int start_y = -1;
		int start_x = c;
		int now_d = d;
		go_down(k, start_y, start_x, now_d);
	}

	cout << result;

	return 0;
}