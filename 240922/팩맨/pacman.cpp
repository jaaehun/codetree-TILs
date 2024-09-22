#include<iostream>
#include<vector>
#include<queue>
#include<string.h>

using namespace std;

int pak_y, pak_x;
vector<int> map[5][5];
vector<int> egg_map[5][5];
vector<int> tmp_map[5][5];
int death_map[5][5];
int visit[5][5];

queue<pair<pair<int, int>, int>> q;
vector<int> v_dir;
vector<int> v_tmp;
int dfs_max;

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,-1,0,1 };
int dyy[9] = { 0,-1,-1,0,1,1,1,0,-1 };
int dxx[9] = { 0,0,-1,-1,-1,0,1,1,1 };

int turn_45(int d) {
	if (d == 8) {
		return 1;
	}
	else {
		return d + 1;
	}
}

void clone_start() {
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			if (map[i][j].size() != 0) {
				egg_map[i][j] = map[i][j];
			}
		}
	}
}

void monster_move() {
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			if (map[i][j].size() != 0) {
				for (int v = 0; v < map[i][j].size(); v++) {
					int now_d = map[i][j][v];
					int flag = 0;
					for (int d = 0; d < 8; d++) {
						int y = i + dyy[now_d];
						int x = j + dxx[now_d];

						if (y > 0 && y <= 4 && x > 0 && x <= 4) {
							if (death_map[y][x] == 0) {
								if (y != pak_y || x != pak_x) {
									tmp_map[y][x].push_back(now_d);
									flag = 1;
									break;
								}
							}
						}
						now_d = turn_45(now_d);
					}
					if (flag == 0) {
						tmp_map[i][j].push_back(now_d);
					}
				}
			}
		}
	}

	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			map[i][j] = tmp_map[i][j];
			tmp_map[i][j].clear();
		}
	}
}

int calculate() {
	int now_y = pak_y;
	int now_x = pak_x;
	int cnt = 0;

	for (int i = 0; i < 3; i++) {
		now_y += dy[v_tmp[i]];
		now_x += dx[v_tmp[i]];

		if (now_y > 0 && now_y <= 4 && now_x > 0 && now_x <= 4) {
			if (map[now_y][now_x].size() != 0 && visit[now_y][now_x] == 0) {
				cnt += map[now_y][now_x].size();
			}
			visit[now_y][now_x] = 1;
		}
		else {
			cnt = -1;
			break;
		}
	}

	memset(visit, 0, sizeof(visit));

	return cnt;
}

void dfs(int cnt) {
	if (cnt == 3) {
		int num = calculate();
		if (num > dfs_max) {
			dfs_max = num;
			v_dir = v_tmp;
		}
		return;
	}

	v_tmp.push_back(0);
	dfs(cnt + 1);
	v_tmp.pop_back();

	v_tmp.push_back(1);
	dfs(cnt + 1);
	v_tmp.pop_back();

	v_tmp.push_back(2);
	dfs(cnt + 1);
	v_tmp.pop_back();

	v_tmp.push_back(3);
	dfs(cnt + 1);
	v_tmp.pop_back();
}

void pakman_move() {
	dfs_max = -1;
	dfs(0);
	v_tmp.clear();
	for (int i = 0; i < 3; i++) {
		pak_y += dy[v_dir[i]];
		pak_x += dx[v_dir[i]];

		map[pak_y][pak_x].clear();
		death_map[pak_y][pak_x] = -3;
	}
	v_dir.clear();
}

void death_clean() {
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			if (death_map[i][j] < 0) {
				death_map[i][j]++;
			}
		}
	}
}

void clone_finish() {
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			if (egg_map[i][j].size() != 0) {
				for (int v = 0; v < egg_map[i][j].size(); v++) {
					map[i][j].push_back(egg_map[i][j][v]);
				}
				egg_map[i][j].clear();
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int M, T;
	cin >> M >> T;
	
	int r, c;
	cin >> r >> c;
	pak_y = r;
	pak_x = c;

	for (int i = 0; i < M; i++) {
		int r, c, d;
		cin >> r >> c >> d;
		map[r][c].push_back(d);
	}

	for (int t = 1; t <= T; t++) {
		clone_start();
		monster_move();
		pakman_move();
		death_clean();
		clone_finish();
	}

	int result = 0;
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			if (map[i][j].size() != 0) {
				result += map[i][j].size();
			}
		}
	}

	cout << result;

	return 0;
}