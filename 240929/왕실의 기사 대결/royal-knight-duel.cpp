#include<iostream>
#include<string.h>

using namespace std;

struct INFO {
	int first_hp;
	int hp;
	int live;
};

int L, N;
int map[41][41];
INFO knight[31];
int knight_map[41][41];
int tmp_map[41][41];

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

int possible(int id, int d) {
	int flag = 1;
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			if (knight_map[i][j] == id) {
				int next_y = i + dy[d];
				int next_x = j + dx[d];
				if (next_y > 0 && next_y <= L && next_x > 0 && next_x <= L) {
					if (map[next_y][next_x] == 2) {
						flag = -1;
						break;
					}
					if (knight_map[next_y][next_x] != 0 && knight_map[next_y][next_x] != id) {
						flag = possible(knight_map[next_y][next_x], d);
						if (flag == -1) {
							break;
						}
					}
				}
				else {
					flag = -1;
					break;
				}
			}
		}
		if (flag == -1) {
			break;
		}
	}

	return flag;
}

void move(int id, int d, int first_id) {
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			if (knight_map[i][j] == id) {
				int next_y = i + dy[d];
				int next_x = j + dx[d];
				
				if (map[next_y][next_x] == 1 && id != first_id) {
					knight[id].hp--;
				}

				if (knight_map[next_y][next_x] != 0 && knight_map[next_y][next_x] != id) {
					move(knight_map[next_y][next_x], d, first_id);
				}
				tmp_map[next_y][next_x] = id;
				knight_map[i][j] = 0;
			}
		}
	}
}

void command(int id, int d) {
	if (knight[id].live == 1) {
		int flag = possible(id, d);
		if (flag == 1) {
			move(id, d, id);
		}

		for (int i = 1; i <= N; i++) {
			if (knight[i].hp <= 0) {
				knight[i].live = -1;
			}
		}

		for (int i = 1; i <= L; i++) {
			for (int j = 1; j <= L; j++) {
				if (tmp_map[i][j] != 0) {
					knight_map[i][j] = tmp_map[i][j];
				}
			}
		}
		memset(tmp_map, 0, sizeof(tmp_map));
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int Q;
	cin >> L >> N >> Q;

	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cin >> map[i][j];
		}
	}

	for (int p = 1; p <= N; p++) {
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;
		knight[p].first_hp = k;
		knight[p].hp = k;
		knight[p].live = 1;

		for (int i = r; i < r + h; i++) {
			for (int j = c; j < c + w; j++) {
				knight_map[i][j] = p;
			}
		}
	}

	for (int t = 1; t <= Q; t++) {
		int i, d;
		cin >> i >> d;
		command(i, d);
	}

	int result = 0;
	for (int i = 1; i <= N; i++) {
		if (knight[i].live == 1) {
			result += knight[i].first_hp - knight[i].hp;
		}
	}

	cout << result;

	return 0;
}