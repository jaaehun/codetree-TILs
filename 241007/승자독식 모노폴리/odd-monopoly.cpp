#include<iostream>
#include<vector>
#include<queue>
#include<string.h>

using namespace std;

struct INFO {
	int y;
	int x;
	int d;
	int f;
};

struct MAP {
	int num;
	int turn;
};

int N, M, K;
int map[21][21];
priority_queue<int, vector<int>, greater<int>> pq[21][21];
MAP monopoly_map[21][21];
INFO player[401];
int dir_arr[21][4][4];
int result_cnt;

int dy[5] = { 0,-1,1,0,0 };
int dx[5] = { 0,0,0,-1,1 };

void player_move() {
	for (int p = 1; p <= M; p++) {
		if (player[p].f == 0) {
			int now_y = player[p].y;
			int now_x = player[p].x;
			int now_d = player[p].d;

			int flag = 0;
			for (int i = 0; i < 4; i++) {
				int d = dir_arr[p][now_d][i];
				int y = now_y + dy[d];
				int x = now_x + dx[d];

				if (y >= 0 && y < N && x >= 0 && x < N) {
					if (monopoly_map[y][x].num == 0) {
						flag = 1;
						pq[y][x].push(p);
						player[p].y = y;
						player[p].x = x;
						player[p].d = d;
						break;
					}
				}
			}

			if (flag == 0) {
				for (int i = 0; i < 4; i++) {
					int d = dir_arr[p][now_d][i];
					int y = now_y + dy[d];
					int x = now_x + dx[d];

					if (y >= 0 && y < N && x >= 0 && x < N) {
						if (monopoly_map[y][x].num == p) {
							flag = 1;
							pq[y][x].push(p);
							player[p].y = y;
							player[p].x = x;
							player[p].d = d;
							break;
						}
					}
				}
				if (flag == 0) {
					pq[now_y][now_x].push(p);
				}
			}
		}
	}
	
	memset(map, 0, sizeof(map));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (!pq[i][j].empty()) {
				int num = pq[i][j].top();
				map[i][j] = num;
				monopoly_map[i][j].num = num;
				monopoly_map[i][j].turn = K + 1;
				pq[i][j].pop();
				if (!pq[i][j].empty()) {
					while (!pq[i][j].empty()) {
						num = pq[i][j].top();
						player[num].f = -1;
						result_cnt++;
						pq[i][j].pop();
					}
				}
			}
		}
	}
}

void minus_turn() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (monopoly_map[i][j].num != 0) {
				monopoly_map[i][j].turn--;
				if (monopoly_map[i][j].turn == 0) {
					monopoly_map[i][j].num = 0;
				}
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	cin >> N >> M >> K;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
			if (map[i][j] != 0) {
				player[map[i][j]].y = i;
				player[map[i][j]].x = j;
				monopoly_map[i][j].num = map[i][j];
				monopoly_map[i][j].turn = K;
			}
		}
	}

	for (int p = 1; p <= M; p++) {
		int d;
		cin >> d;
		player[p].d = d;
	}

	for (int p = 1; p <= M; p++) {
		for (int d = 1; d <= 4; d++) {
			for (int i = 0; i < 4; i++) {
				int a;
				cin >> a;
				dir_arr[p][d][i] = a;
			}
		}
	}

	int time = 1;
	result_cnt = 0;
	while (true) {
		player_move();
		if (result_cnt == M - 1) {
			break;
		}
		minus_turn();
		time++;
		if (time >= 1000) {
			time = -1;
			break;
		}
	}

	cout << time;

	return 0;
}