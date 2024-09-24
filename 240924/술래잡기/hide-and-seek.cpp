#include<iostream>
#include<vector>
#include<cmath>

using namespace std;

int N;
vector<int> map[100][100];
vector<int> tmp_map[100][100];
int tree_map[100][100];
int result;
int score;
int att_y, att_x, att_d, att_f;
int att_cnt, small_n, big_n;

int dy[4] = { 0,0,1,-1 };
int dx[4] = { -1,1,0,0 };

int turn_back(int d) {
	if (d == 0) {
		return 1;
	}
	else if (d == 1) {
		return 0;
	}
	else if (d == 2) {
		return 3;
	}
	else {
		return 2;
	}
}

int turn_right(int d) {
	if (d == 0) {
		return 3;
	}
	else if (d == 1) {
		return 2;
	}
	else if (d == 2) {
		return 0;
	}
	else {
		return 1;
	}
}

int turn_left(int d) {
	if (d == 0) {
		return 2;
	}
	else if (d == 1) {
		return 3;
	}
	else if (d == 2) {
		return 1;
	}
	else {
		return 0;
	}
}


void move_defenser() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j].size() != 0) {
				if (abs(i - att_y) + abs(j - att_x) <= 3) {
					for (int v = 0; v < map[i][j].size(); v++) {
						int now_d = map[i][j][v];
						int y = i + dy[now_d];
						int x = j + dx[now_d];
						if (y > 0 && y <= N && x > 0 && x <= N) {
							if (y == att_y && x == att_x) {
								tmp_map[i][j].push_back(now_d);
							}
							else {
								tmp_map[y][x].push_back(now_d);
							}
						}
						else {
							now_d = turn_back(now_d);
							int y = i + dy[now_d];
							int x = j + dx[now_d];
							if (y == att_y && x == att_x) {
								tmp_map[i][j].push_back(now_d);
							}
							else {
								tmp_map[y][x].push_back(now_d);
							}
						}
					}
				}
			}
		}
	}
	
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			map[i][j] = tmp_map[i][j];
			tmp_map[i][j].clear();
		}
	}
}

void move_attacker() {
	if (att_f == 0) {
		att_y += dy[att_d];
		att_x += dx[att_d];

		if (att_y == 1 && att_x == 1) {
			att_f = 1;
			att_d = turn_back(att_d);
			att_cnt = 0;
			small_n = 4;
			big_n = -1;
		}
		else {
			att_cnt++;
			if (att_cnt == small_n) {
				att_cnt = 0;
				att_d = turn_right(att_d);
				big_n++;
			}
			if (big_n == 2) {
				big_n = 0;
				small_n++;
			}
		}
	}
	else {
		att_y += dy[att_d];
		att_x += dx[att_d];

		if (att_y == N / 2 + 1 && att_x == N / 2 + 1) {
			att_f = 0;
			att_d = turn_back(att_d);
			att_cnt = 0;
			small_n = 1;
			big_n = 0;
		}
		else {
			att_cnt++;
			if (att_cnt == small_n) {
				att_cnt = 0;
				att_d = turn_left(att_d);
				big_n++;
			}
			if (big_n == 2) {
				big_n = 0;
				small_n--;
			}
		}
	}
}

void attack(int round) {
	for (int i = 0; i < 3; i++) {
		int now_y = att_y + i * dy[att_d];
		int now_x = att_x + i * dx[att_d];

		if (now_y > 0 && now_y <= N && now_x > 0 && now_x <= N) {
			if (map[now_y][now_x].size() != 0 && tree_map[now_y][now_x] == 0) {
				int size = map[now_y][now_x].size();
				result += round * size;
				score += size;
				map[now_y][now_x].clear();
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int M, H, K;
	cin >> N >> M >> H >> K;

	for (int i = 0; i < M; i++) {
		int y, x, d;
		cin >> y >> x >> d;
		map[y][x].push_back(d);
	}

	for (int i = 0; i < H; i++) {
		int y, x;
		cin >> y >> x;
		tree_map[y][x] = 1;
	}

	result = 0;
	score = 0;
	att_y = N / 2 + 1;
	att_x = N / 2 + 1;
	att_d = 3;
	att_f = 0;
	att_cnt = 0;
	small_n = 1;
	big_n = 0;
	for (int t = 1; t <= K; t++) {
		move_defenser();
		move_attacker();
		attack(t);
		if (score == M) {
			break;
		}
	}
	
	cout << result;

	return 0;
}