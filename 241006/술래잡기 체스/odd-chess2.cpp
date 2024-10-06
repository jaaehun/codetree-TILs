#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

struct INFO {
	int y;
	int x;
	int d;
	int live;
};

int map[5][5];
int tmp_map[5][5];
INFO defenser[17];
vector<int> result;

int dy[9] = { 0,-1,-1,0,1,1,1,0,-1 };
int dx[9] = { 0,0,-1,-1,-1,0,1,1,1 };

int turn_dir(int d) {
	if (d == 9) {
		return 1;
	}
	else {
		return d + 1;
	}
}

void copy_map(int A[5][5], int B[5][5]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			A[i][j] = B[i][j];
		}
	}
}

void calculate(int score, int y, int x, int dir) {
	for (int i = 1; i <= 16; i++) {
		if (defenser[i].live == 0) {
			int now_y = defenser[i].y;
			int now_x = defenser[i].x;
			int now_d = defenser[i].d;

			for (int a = 0; a < 8; a++) {
				int yy = now_y + dy[now_d];
				int xx = now_x + dx[now_d];

				if (yy >= 0 && yy < 4 && xx >= 0 && xx < 4) {
					if (map[yy][xx] != -1) {
						if (map[yy][xx] > 0) {
							int num = map[yy][xx];
							map[now_y][now_x] = num;
							defenser[num].y = now_y;
							defenser[num].x = now_x;
						}
						map[yy][xx] = i;
						defenser[i].y = yy;
						defenser[i].x = xx;
						defenser[i].d = now_d;
						break;
					}
				}
				now_d = turn_dir(now_d);
			}
		}
	}

	int flag = 0;
	for (int i = 1; i < 4; i++) {
		int now_y = y + i * dy[dir];
		int now_x = x + i * dx[dir];

		if (now_y >= 0 && now_y < 4 && now_x >= 0 && now_x < 4) {
			if (map[now_y][now_x] > 0) {
				flag = 1;
				copy_map(map, tmp_map);
				int num = map[now_y][now_x];
				int now_dir = defenser[num].d;
				map[y][x] = 0;
				map[now_y][now_x] = -1;
				defenser[num].live = -1;
				calculate(score + num, now_y, now_x, now_dir);
				copy_map(tmp_map, map);
				defenser[num].live = 0;
			}
		}
	}
	if (flag == 0) {
		result.push_back(score);
		return;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int p, d;
			cin >> p >> d;
			map[i][j] = p;
			defenser[p].y = i;
			defenser[p].x = j;
			defenser[p].d = d;
		}
	}

	int score = map[0][0];
	int now_dir = defenser[map[0][0]].d;
	defenser[map[0][0]].live = -1;
	map[0][0] = -1;

	calculate(score, 0, 0, now_dir);

	sort(result.rbegin(), result.rend());

	cout << result[0];

	return 0;
}