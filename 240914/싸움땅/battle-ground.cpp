#include<iostream>
#include<vector>
#include<queue>

using namespace std;

struct PLAYER {
	int y;
	int x;
	int power;
	int gun;
	int score;
	int dir;
};

int n, m;
vector<PLAYER> player;
int player_map[21][21];
priority_queue<int> gun_map[21][21];

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

int turn_back(int d) {
	if (d == 0) {
		return 2;
	}
	else if (d == 1) {
		return 3;
	}
	else if (d == 2) {
		return 0;
	}
	else {
		return 1;
	}
}

int turn_90(int d) {
	if (d == 3) {
		return 0;
	}
	else {
		return d + 1;
	}
}

void lose(int a, int y, int x) {
	if (player[a].gun != 0) {
		gun_map[y][x].push(player[a].gun);
		player[a].gun = 0;
	}
	int now_y = y;
	int now_x = x;
	int now_d = player[a].dir;
	for (int i = 0; i < 4; i++) {
		int next_y = now_y + dy[now_d];
		int next_x = now_x + dx[now_d];

		if (next_y > 0 && next_y <= n && next_x > 0 && next_x <= n) {
			if (player_map[next_y][next_x] == 0) {
				now_y = next_y;
				now_x = next_x;
				break;
			}
		}
		now_d = turn_90(now_d);
	}
	player_map[now_y][now_x] = a;
	player[a].y = now_y;
	player[a].x = now_x;
	player[a].dir = now_d;

	if (gun_map[now_y][now_x].size() != 0) {
		int tmp = gun_map[now_y][now_x].top();
		gun_map[now_y][now_x].pop();

		player[a].gun = tmp;
	}
}

void win(int a, int y, int x, int s) {
	player_map[y][x] = a;
	player[a].y = y;
	player[a].x = x;
	player[a].score += s;

	if (gun_map[y][x].size() != 0) {
		int tmp = gun_map[y][x].top();
		if (tmp > player[a].gun) {
			gun_map[y][x].pop();
			if (player[a].gun != 0) {
				gun_map[y][x].push(player[a].gun);
			}
			player[a].gun = tmp;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int k;
	cin >> n >> m >> k;

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			int a;
			cin >> a;
			if (a > 0) {
				gun_map[i][j].push(a);
			}
		}
	}

	player.push_back({ 0,0,0,0,0,0 });
	for (int i = 1; i <= m; i++) {
		int y, x, s, d;
		cin >> y >> x >> d >> s;
		player.push_back({ y,x,s,0,0,d });
		player_map[y][x] = i;
	}

	for (int t = 1; t <= k; t++) {
		for (int i = 1; i <= m; i++) {
			int now_y = player[i].y;
			int now_x = player[i].x;
			int now_d = player[i].dir;
			player_map[now_y][now_x] = 0;

			int next_y = now_y + dy[now_d];
			int next_x = now_x + dx[now_d];
			if (next_y > 0 && next_y <= n && next_x > 0 && next_x <= n) {
				now_y = next_y;
				now_x = next_x;
			}
			else {
				now_d = turn_back(now_d);
				now_y = now_y + dy[now_d];
				now_x = now_x + dx[now_d];
			}

			if (player_map[now_y][now_x] == 0) {
				if (gun_map[now_y][now_x].size() != 0) {
					int tmp = gun_map[now_y][now_x].top();
					if (tmp > player[i].gun) {
						gun_map[now_y][now_x].pop();
						if (player[i].gun != 0) {
							gun_map[now_y][now_x].push(player[i].gun);
						}
						player[i].gun = tmp;
					}
				}
				player_map[now_y][now_x] = i;
				player[i].y = now_y;
				player[i].x = now_x;
				player[i].dir = now_d;
			}
			else {
				int a = i;
				int b = player_map[now_y][now_x];
				int score = abs(player[a].power + player[a].gun - player[b].power - player[b].gun);
				if (player[a].power + player[a].gun > player[b].power + player[b].gun) {
					lose(b, now_y, now_x);
					win(a, now_y, now_x, score);
				}
				else if (player[a].power + player[a].gun == player[b].power + player[b].gun) {
					if (player[a].power > player[b].power) {
						lose(b, now_y, now_x);
						win(a, now_y, now_x, score);
					}
					else {
						lose(a, now_y, now_x);
						win(b, now_y, now_x, score);
					}
				}
				else {
					lose(a, now_y, now_x);
					win(b, now_y, now_x, score);
				}
			}
		}
	}

	for (int i = 1; i <= m; i++) {
		cout << player[i].score << " ";
	}

	return 0;
}