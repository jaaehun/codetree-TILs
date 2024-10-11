#include<iostream>
#include<queue>
#include<string.h>

using namespace std;

int map[5][5];
int tmp_map[5][5];
int tmp2_map[5][5];
int visit[5][5];
int visit2[5][5];
int result;
queue<int> new_block;
int target_n, target_y, target_x, target_d;
queue<pair<int,int>> q;

int dy[4] = { -1,1,0,0 };
int dx[4] = { 0,0,-1,1 };

void copy_map(int A[5][5], int B[5][5]) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			B[i][j] = A[i][j];
		}
	}
}

void turn_90(int y, int x) {
	int begin_y = y - 1;
	int begin_x = x - 1;
	int end_y = y + 1;
	int end_x = x + 1;

	tmp2_map[y][x] = tmp_map[y][x];
	for (int i = 0; i < 2; i++) {
		tmp2_map[begin_y + i][end_x] = tmp_map[begin_y][begin_x + i];
		tmp2_map[end_y][end_x - i] = tmp_map[begin_y + i][end_x];
		tmp2_map[end_y - i][begin_x] = tmp_map[end_y][end_x - i];
		tmp2_map[begin_y][begin_x + i] = tmp_map[end_y - i][begin_x];
	}

	for (int i = begin_y; i <= end_y; i++) {
		for (int j = begin_x; j <= end_x; j++) {
			tmp_map[i][j] = tmp2_map[i][j];
		}
	}
	memset(tmp2_map, 0, sizeof(tmp2_map));
}

void turn_180(int y, int x) {
	int begin_y = y - 1;
	int begin_x = x - 1;
	int end_y = y + 1;
	int end_x = x + 1;

	tmp2_map[y][x] = tmp_map[y][x];
	for (int i = 0; i < 2; i++) {
		tmp2_map[begin_y + i][end_x] = tmp_map[end_y - i][begin_x];
		tmp2_map[end_y][end_x - i] = tmp_map[begin_y][begin_x + i];
		tmp2_map[end_y - i][begin_x] = tmp_map[begin_y + i][end_x];
		tmp2_map[begin_y][begin_x + i] = tmp_map[end_y][end_x - i];
	}

	for (int i = begin_y; i <= end_y; i++) {
		for (int j = begin_x; j <= end_x; j++) {
			tmp_map[i][j] = tmp2_map[i][j];
		}
	}
	memset(tmp2_map, 0, sizeof(tmp2_map));
}

void turn_270(int y, int x) {
	int begin_y = y - 1;
	int begin_x = x - 1;
	int end_y = y + 1;
	int end_x = x + 1;

	tmp2_map[y][x] = tmp_map[y][x];
	for (int i = 0; i < 2; i++) {
		tmp2_map[begin_y + i][end_x] = tmp_map[end_y][end_x - i];
		tmp2_map[end_y][end_x - i] = tmp_map[end_y - i][begin_x];
		tmp2_map[end_y - i][begin_x] = tmp_map[begin_y][begin_x + i];
		tmp2_map[begin_y][begin_x + i] = tmp_map[begin_y + i][end_x];
	}

	for (int i = begin_y; i <= end_y; i++) {
		for (int j = begin_x; j <= end_x; j++) {
			tmp_map[i][j] = tmp2_map[i][j];
		}
	}
	memset(tmp2_map, 0, sizeof(tmp2_map));
}

int bfs_tmp(int start_y, int start_x) {
	q.push({ start_y,start_x });
	visit[start_y][start_x] = 1;
	int cnt = 1;
	int target = tmp_map[start_y][start_x];

	while (!q.empty()) {
		int now_y = q.front().first;
		int now_x = q.front().second;

		q.pop();

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y >= 0 && y < 5 && x >= 0 && x < 5) {
				if (tmp_map[y][x] == target && visit[y][x] == 0) {
					cnt++;
					q.push({ y,x });
					visit[y][x] = 1;
				}
			}
		}
	}
	return cnt;
}

int calculate() {
	int sum = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (visit[i][j] == 0) {
				int cnt = bfs_tmp(i, j);
				if (cnt >= 3) {
					sum += cnt;
				}
			}
		}
	}
	memset(visit, 0, sizeof(visit));

	return sum;
}

void check(int y, int x) {
	copy_map(map, tmp_map);
	turn_90(y, x);
	int cnt = calculate();
	if (cnt > target_n) {
		target_n = cnt;
		target_d = 90;
		target_y = y;
		target_x = x;
	}
	else if (cnt == target_n) {
		if (90 < target_d) {
			target_d = 90;
			target_y = y;
			target_x = x;
		}
	}

	copy_map(map, tmp_map);
	turn_180(y, x);
	cnt = calculate();
	if (cnt > target_n) {
		target_n = cnt;
		target_d = 180;
		target_y = y;
		target_x = x;
	}
	else if (cnt == target_n) {
		if (180 < target_d) {
			target_d = 180;
			target_y = y;
			target_x = x;
		}
	}

	copy_map(map, tmp_map);
	turn_270(y, x);
	cnt = calculate();
	if (cnt > target_n) {
		target_n = cnt;
		target_d = 270;
		target_y = y;
		target_x = x;
	}
}

void exploration() {
	target_n = -1;
	target_d = 0;
	target_y = 0;
	target_x = 0;
	for (int j = 1; j <= 3; j++) {
		for (int i = 1; i <= 3; i++) {
			check(i, j);
		}
	}
	copy_map(map, tmp_map);
	if (target_d == 90) {
		turn_90(target_y, target_x);
	}
	else if (target_d == 180) {
		turn_180(target_y, target_x);
	}
	else {
		turn_270(target_y, target_x);
	}
	copy_map(tmp_map, map);

	memset(tmp_map, 0, sizeof(tmp_map));
}

int bfs_map(int start_y, int start_x) {
	q.push({ start_y,start_x });
	visit[start_y][start_x] = 1;
	int cnt = 1;
	int target = map[start_y][start_x];

	while (!q.empty()) {
		int now_y = q.front().first;
		int now_x = q.front().second;

		q.pop();

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y >= 0 && y < 5 && x >= 0 && x < 5) {
				if (map[y][x] == target && visit[y][x] == 0) {
					cnt++;
					q.push({ y,x });
					visit[y][x] = 1;
				}
			}
		}
	}
	return cnt;
}

void bfs_map2(int start_y, int start_x) {
	q.push({ start_y,start_x });
	visit2[start_y][start_x] = 1;
	int target = map[start_y][start_x];
	map[start_y][start_x] = 0;

	while (!q.empty()) {
		int now_y = q.front().first;
		int now_x = q.front().second;

		q.pop();

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y >= 0 && y < 5 && x >= 0 && x < 5) {
				if (map[y][x] == target && visit2[y][x] == 0) {
					q.push({ y,x });
					visit2[y][x] = 1;
					map[y][x] = 0;
				}
			}
		}
	}
	memset(visit2, 0, sizeof(visit2));
}

void put_block() {
	for (int j = 0; j < 5; j++) {
		for (int i = 4; i >= 0; i--) {
			if (map[i][j] == 0) {
				int new_num = new_block.front();
				new_block.pop();
				map[i][j] = new_num;
			}
		}
	}
}

void get_score() {
	while (true) {
		int flag = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (visit[i][j] == 0) {
					int cnt = bfs_map(i, j);
					if (cnt >= 3) {
						result += cnt;
						flag = 1;
						bfs_map2(i, j);
					}
				}
			}
		}
		memset(visit, 0, sizeof(visit));
		if (flag == 0) {
			break;
		}
		put_block();
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int K, M;
	cin >> K >> M;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cin >> map[i][j];
		}
	}

	for (int i = 0; i < M; i++) {
		int a;
		cin >> a;
		new_block.push(a);
	}

	for (int t = 1; t <= K; t++) {
		result = 0;
		exploration();
		if (target_n == 0) {
			break;
		}
		get_score();
		cout << result << " ";
	}
	return 0;
}