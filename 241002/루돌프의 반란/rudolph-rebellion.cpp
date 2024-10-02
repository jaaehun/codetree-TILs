#include<iostream>
#include<string.h>

using namespace std;

struct SANTA {
	int y;
	int x;
	int live;
	int score;
};

int N, P, C, D;
int att_y, att_x;
int map[51][51];
int tmp_map[51][51];
SANTA santa[31];
int out_cnt;

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };
int dyy[8] = { -1,1,0,0,-1,-1,1,1 };
int dxx[8] = { 0,0,-1,1,-1,1,-1,1 };

int pow(int n) {
	return n * n;
}

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

void interactive(int flag, int start_y, int start_x, int d) {
	if (flag == 0) {
		int now_y = start_y;
		int now_x = start_x;
		while (true) {
			int next_y = now_y + dyy[d];
			int next_x = now_x + dxx[d];

			if (next_y > 0 && next_y <= N && next_x > 0 && next_x <= N) {
				tmp_map[next_y][next_x] = map[now_y][now_x];
				santa[map[now_y][now_x]].y = next_y;
				santa[map[now_y][now_x]].x = next_x;
				if (map[next_y][next_x] != 0) {
					now_y = next_y;
					now_x = next_x;
				}
				else {
					break;
				}
			}
			else {
				santa[map[now_y][now_x]].live = -100;
				out_cnt++;
				break;
			}
		}
	}
	else {
		int now_y = start_y;
		int now_x = start_x;
		while (true) {
			int next_y = now_y + dy[d];
			int next_x = now_x + dx[d];

			if (next_y > 0 && next_y <= N && next_x > 0 && next_x <= N) {
				tmp_map[next_y][next_x] = map[now_y][now_x];
				santa[map[now_y][now_x]].y = next_y;
				santa[map[now_y][now_x]].x = next_x;
				if (map[next_y][next_x] != 0) {
					now_y = next_y;
					now_x = next_x;
				}
				else {
					break;
				}
			}
			else {
				santa[map[now_y][now_x]].live = -100;
				out_cnt++;
				break;
			}
		}
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (tmp_map[i][j] > 0) {
				map[i][j] = tmp_map[i][j];
			}
		}
	}
	memset(tmp_map, 0, sizeof(tmp_map));
}

void attack(int flag, int start_y, int start_x, int d) {
	if (flag == 0) {
		int now_n = map[start_y][start_x];
		santa[now_n].score += C;
		map[start_y][start_x] = 0;

		int now_y = start_y + C* dyy[d];
		int now_x = start_x + C* dxx[d];
		if (now_y > 0 && now_y <= N && now_x > 0 && now_x <= N) {
			if (map[now_y][now_x] != 0) {
				interactive(0, now_y, now_x, d);
			}
			map[now_y][now_x] = now_n;
			santa[now_n].y = now_y;
			santa[now_n].x = now_x;
			santa[now_n].live = -2;
		}
		else {
			santa[now_n].live = -100;
			out_cnt++;
		}
	}
	else {
		d = turn_back(d);
		int now_n = map[start_y][start_x];
		santa[now_n].score += D;
		map[start_y][start_x] = 0;

		int now_y = start_y + D* dy[d];
		int now_x = start_x + D* dx[d];
		if (now_y > 0 && now_y <= N && now_x > 0 && now_x <= N) {
			if (map[now_y][now_x] != 0) {
				interactive(1, now_y, now_x, d);
			}
			map[now_y][now_x] = now_n;
			santa[now_n].y = now_y;
			santa[now_n].x = now_x;
			santa[now_n].live = -2;
		}
		else {
			santa[now_n].live = -100;
			out_cnt++;
		}
	}
}

void move_attacker() {
	int min_dis = 987654321;
	int target_y = 0;
	int target_x = 0;
	for (int i = 1; i <= P; i++) {
		if (santa[i].live != -100) {
			int now_y = santa[i].y;
			int now_x = santa[i].x;	
			int now_dis = pow(att_y - now_y) + pow(att_x - now_x);

			if (now_dis < min_dis) {
				min_dis = now_dis;
				target_y = now_y;
				target_x = now_x;
			}
			else if (now_dis == min_dis) {
				if (now_y > target_y) {
					target_y = now_y;
					target_x = now_x;
				}
				else if (now_y == target_y) {
					if (now_x > target_x) {
						target_y = now_y;
						target_x = now_x;
					}
				}
			}
		}
	}

	min_dis = 987654321;
	int target_d;
	for (int i = 0; i < 8; i++) {
		int now_y = att_y + dyy[i];
		int now_x = att_x + dxx[i];

		if (now_y > 0 && now_y <= N && now_x > 0 && now_x <= N) {
			int now_dis = pow(target_y - now_y) + pow(target_x - now_x);
			if (now_dis < min_dis) {
				min_dis = now_dis;
				target_d = i;
			}
		}
	}

	att_y += dyy[target_d];
	att_x += dxx[target_d];

	if (map[att_y][att_x] != 0) {
		attack(0, att_y, att_x, target_d);
	}
}

void move_santa() {
	for (int i = 1; i <= P; i++) {
		if (santa[i].live == 0) {
			int now_y = santa[i].y;
			int now_x = santa[i].x;
			int min_dis = pow(att_y - now_y) + pow(att_x - now_x);
			int target_d = -1;

			for (int d = 0; d < 4; d++) {
				int y = now_y + dy[d];
				int x = now_x + dx[d];

				if (y > 0 && y <= N && x > 0 && x <= N) {
					if (map[y][x] == 0) {
						int now_dis = pow(att_y - y) + pow(att_x - x);
						if (now_dis < min_dis) {
							min_dis = now_dis;
							target_d = d;
						}
					}
				}
			}
			if (target_d != -1) {
				map[now_y][now_x] = 0;
				now_y += dy[target_d];
				now_x += dx[target_d];

				if (now_y == att_y && now_x == att_x) {
					map[now_y][now_x] = i;
					attack(1, now_y, now_x, target_d);
				}
				else {
					map[now_y][now_x] = i;
					santa[i].y = now_y;
					santa[i].x = now_x;
				}
			}
		}
	}
}

void heal_santa() {
	for (int i = 1; i <= P; i++) {
		if (santa[i].live < 0 && santa[i].live != -100) {
			santa[i].live++;
		}
	}
}

void get_score() {
	for (int i = 1; i <= P; i++) {
		if (santa[i].live != -100) {
			santa[i].score++;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int M;
	cin >> N >> M >> P >> C >> D;

	int Ry, Rx;
	cin >> Ry >> Rx;
	att_y = Ry;
	att_x = Rx;

	for (int i = 1; i <= P; i++) {
		int Pn, Sy, Sx;
		cin >> Pn >> Sy >> Sx;
		map[Sy][Sx] = Pn;
		santa[Pn].y = Sy;
		santa[Pn].x = Sx;
		santa[Pn].live = 0;
		santa[Pn].score = 0;
	}

	out_cnt = 0;
	for (int i = 1; i <= M; i++) {
		move_attacker();
		if (out_cnt == P) {
			break;
		}
		move_santa();
		if (out_cnt == P) {
			break;
		}
		heal_santa();
		get_score();
	}

	for (int i = 1; i <= P; i++) {
		cout << santa[i].score << " ";
	}

	return 0;
}