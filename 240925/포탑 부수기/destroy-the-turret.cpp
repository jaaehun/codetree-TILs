#include<iostream>
#include<queue>
#include<string.h>

using namespace std;

struct INFO {
	int num;
	int round;
};

int N, M;
INFO map[11][11];
int recover_map[11][11];
int att_y, att_x, def_y, def_x;
int result;

int visit[11][11];
int back_y[11][11];
int back_x[11][11];
queue<pair<int, int>> q;

int dy[4] = { 0,1,0,-1 };
int dx[4] = { 1,0,-1,0 };
int dyy[8] = { -1,-1,0,1,1,1,0,-1 };
int dxx[8] = { 0,1,1,1,0,-1,-1,-1 };

void pick_attacker() {
	int min_att = 987654321;
	int min_round = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (map[i][j].num != 0) {
				if (map[i][j].num < min_att) {
					min_att = map[i][j].num;
					min_round = map[i][j].round;
					att_y = i;
					att_x = j;
				}
				else if (map[i][j].num == min_att) {
					if (map[i][j].round > min_round) {
						min_round = map[i][j].round;
						att_y = i;
						att_x = j;
					}
					else if (map[i][j].round == min_round) {
						if (i + j > att_y + att_x) {
							att_y = i;
							att_x = j;
						}
						else if (i + j == att_y + att_x) {
							if (j > att_x) {
								att_y = i;
								att_x = j;
							}
						}
					}
				}
			}
		}
	}
}

void pick_defenser() {
	int max_def = 0;
	int max_round = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (map[i][j].num != 0 && (i != att_y || j != att_x)) {
				if (map[i][j].num > max_def) {
					max_def = map[i][j].num;
					max_round = map[i][j].round;
					def_y = i;
					def_x = j;
				}
				else if (map[i][j].num == max_def) {
					if (map[i][j].round < max_round) {
						max_round = map[i][j].round;
						def_y = i;
						def_x = j;
					}
					else if (map[i][j].round == max_round) {
						if (i + j < def_y + def_x) {
							def_y = i;
							def_x = j;
						}
						else if (i + j == def_y + def_x) {
							if (j < def_x) {
								def_y = i;
								def_x = j;
							}
						}
					}
				}
			}
		}
	}
}

int possible(int start_y, int start_x) {
	int flag = 0;
	q.push({ start_y,start_x });
	visit[start_y][start_x] = 1;

	while (!q.empty()) {
		int now_y = q.front().first;
		int now_x = q.front().second;

		q.pop();

		if (now_y == def_y && now_x == def_x) {
			flag = 1;
			break;
		}

		for (int i = 0; i < 4; i++) {
			int y = (now_y + dy[i] + N) % N;
			int x = (now_x + dx[i] + M) % M;

			if (visit[y][x] == 0 && map[y][x].num != 0) {
				q.push({ y,x });
				visit[y][x] = 1;
				back_y[y][x] = now_y;
				back_x[y][x] = now_x;
			}
		}
	}

	while (!q.empty()) {
		q.pop();
	}
	memset(visit, 0, sizeof(visit));

	return flag;
}

void lazer_attack() {
	int att = map[att_y][att_x].num;
	int now_y = def_y;
	int now_x = def_x;
	map[now_y][now_x].num -= att;

	while (true) {
		int next_y = back_y[now_y][now_x];
		int next_x = back_x[now_y][now_x];

		if (next_y == att_y && next_x == att_x) {
			break;
		}

		map[next_y][next_x].num -= att / 2;
		recover_map[next_y][next_x] = 1;

		now_y = next_y;
		now_x = next_x;
	}
}

void bomb_attack() {
	int att = map[att_y][att_x].num;
	map[def_y][def_x].num -= att;

	for (int i = 0; i < 8; i++) {
		int now_y = (def_y + dyy[i] + N) % N;
		int now_x = (def_x + dxx[i] + M) % M;

		if (map[now_y][now_x].num != 0 && (now_y != att_y || now_x != att_x)) {
			map[now_y][now_x].num -= att / 2;
			recover_map[now_y][now_x] = 1;
		}
	}
}


int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int K;
	cin >> N >> M >> K;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> map[i][j].num;
			map[i][j].round = 0;
		}
	}

	for (int t = 1; t <= K; t++) {
		att_y = 0;
		att_x = 0;
		pick_attacker();
		map[att_y][att_x].num += N + M;
		map[att_y][att_x].round = t;
		recover_map[att_y][att_x] = 1;
		def_y = 0;
		def_x = 0;
		pick_defenser();
		recover_map[def_y][def_x] = 1;

		int flag = possible(att_y, att_x);
		if (flag == 1) {
			lazer_attack();
			memset(back_y, 0, sizeof(back_y));
			memset(back_x, 0, sizeof(back_x));
		}
		else {
			bomb_attack();
		}

		result = 0;
		int cnt = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (map[i][j].num < 0) {
					map[i][j].num = 0;
				}
				if (map[i][j].num != 0 && recover_map[i][j] == 0) {
					map[i][j].num++;
				}
				if (map[i][j].num != 0) {
					cnt++;
				}
				if (map[i][j].num > result) {
					result = map[i][j].num;
				}
			}
		}
		memset(recover_map, 0, sizeof(recover_map));

		if (cnt == 1) {
			break;
		}
	}
	cout << result;

	return 0;
}