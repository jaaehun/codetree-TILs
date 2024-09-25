#include<iostream>
#include<vector>
#include<string.h>

using namespace std;

int N, M;
int map[11][11];
vector<int> player_map[11][11];
int exit_y, exit_x;
int exit_cnt;
int sum;

int dy[4] = { -1,1,0,0 };
int dx[4] = { 0,0,-1,1 };

void player_move() {
	vector<int> tmp[11][11];
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (player_map[i][j].size() != 0) {
				for (int k = 0; k < player_map[i][j].size(); k++) {
					int num = player_map[i][j][k];
					int min_dis = abs(exit_y - i) + abs(exit_x - j);
					int min_y = i;
					int min_x = j;
					for (int d = 0; d < 4; d++) {
						int y = i + dy[d];
						int x = j + dx[d];

						if (y > 0 && y <= N && x > 0 && x <= N) {
							if (map[y][x] == 0) {
								if (abs(exit_y - y) + abs(exit_x - x) < min_dis) {
									min_dis = abs(exit_y - y) + abs(exit_x - x);
									min_y = y;
									min_x = x;
								}
							}
						}
					}
					if (min_y != i || min_x != j) {
						sum++;
					}
					if (min_y == exit_y && min_x == exit_x) {
						exit_cnt++;
					}
					else {
						tmp[min_y][min_x].push_back(num);
					}
				}
			}
		}
	}
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			player_map[i][j] = tmp[i][j];
		}
	}
}

int cal_dis() {
	int min = 987654321;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (player_map[i][j].size() != 0) {
				int now_dis = max(abs(i - exit_y), abs(j - exit_x));
				if (now_dis < min) {
					min = now_dis;
				}
			}
		}
	}
	return min;
}

void spin_map(int n) {
	int copy[11][11];
	memset(copy, 0, sizeof(copy));
	vector<int> tmp[11][11];
	int begin_y = 0;
	int begin_x = 0;
	int end_y = 0; 
	int end_x = 0;
	for (int i = 1; i <= N - n; i++) {
		int flag = 0;
		for (int j = 1; j <= N - n; j++) {
			int flage = 0;
			int flagp = 0;
			for (int a = i; a <= i + n; a++) {
				for (int b = j; b <= j + n; b++) {
					if (a == exit_y && b == exit_x) {
						flage = 1;
					}
					if (player_map[a][b].size() != 0) {
						flagp = 1;
					}
				}
			}
			if (flage == 1 && flagp == 1) {
				flag = 1;
				begin_y = i;
				begin_x = j;
				end_y = i + n;
				end_x = j + n;
				break;
			}
		}
		if (flag == 1) {
			break;
		}
	}
	int tmp_y = 0;
	int tmp_x = 0;
	for (int i = begin_y; i <= end_y; i++) {
		for (int j = begin_x; j <= end_x; j++) {
			if (map[i][j] - 1 > 0) {
				copy[begin_y + j - begin_x][end_x - i + begin_y] = map[i][j] - 1;
			}
			else {
				copy[begin_y + j - begin_x][end_x - i + begin_y] = 0;
			}
			tmp[begin_y + j - begin_x][end_x - i + begin_y] = player_map[i][j];

			if (i == exit_y && j == exit_x) {
				tmp_y = begin_y + j - begin_x;
				tmp_x = end_x - i + begin_y;
			}
		}
	}
	exit_y = tmp_y;
	exit_x = tmp_x;

	for (int i = begin_y; i <= end_y; i++) {
		for (int j = begin_x; j <= end_x; j++) {
			map[i][j] = copy[i][j];
			player_map[i][j] = tmp[i][j];
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int K;
	cin >> N >> M >> K;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> map[i][j];
		}
	}
	for (int i = 1; i <= M; i++) {
		int a, b;
		cin >> a >> b;
		player_map[a][b].push_back(i);
	}
	int a, b;
	cin >> a >> b;
	exit_y = a;
	exit_x = b;	
	exit_cnt = 0;
	sum = 0;

	for (int t = 1; t <= K; t++) {
		player_move();
		if (exit_cnt == M) {
			break;
		}
		int min_dis = cal_dis();
		spin_map(min_dis);
	}

	cout << sum << "\n";
	cout << exit_y << " " << exit_x;

	return 0;
}