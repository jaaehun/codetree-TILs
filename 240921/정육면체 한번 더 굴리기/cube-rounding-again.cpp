#include<iostream>
#include<queue>
#include<string.h>

using namespace std;

int N;
int map[21][21];
int visit[21][21];
int dice[6];
int result;
queue<pair<int, int>> q;

int dy[4] = { 0,1,0,-1 };
int dx[4] = { 1,0,-1,0 };

int turn_90(int d) {
	if (d == 3) {
		return 0;
	}
	else {
		return d + 1;
	}
}

int turn_270(int d) {
	if (d == 0) {
		return 3;
	}
	else {
		return d - 1;
	}
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

void right() {
	int d0 = dice[0];
	int d1 = dice[1];
	int d2 = dice[2];
	int d3 = dice[3];
	int d4 = dice[4];
	int d5 = dice[5];

	dice[0] = d4;
	dice[1] = d1;
	dice[2] = d0;
	dice[3] = d3;
	dice[4] = d5;
	dice[5] = d2;
}

void left() {
	int d0 = dice[0];
	int d1 = dice[1];
	int d2 = dice[2];
	int d3 = dice[3];
	int d4 = dice[4];
	int d5 = dice[5];

	dice[0] = d2;
	dice[1] = d1;
	dice[2] = d5;
	dice[3] = d3;
	dice[4] = d0;
	dice[5] = d4;
}

void up() {
	int d0 = dice[0];
	int d1 = dice[1];
	int d2 = dice[2];
	int d3 = dice[3];
	int d4 = dice[4];
	int d5 = dice[5];

	dice[0] = d1;
	dice[1] = d5;
	dice[2] = d2;
	dice[3] = d0;
	dice[4] = d4;
	dice[5] = d3;
}

void down() {
	int d0 = dice[0];
	int d1 = dice[1];
	int d2 = dice[2];
	int d3 = dice[3];
	int d4 = dice[4];
	int d5 = dice[5];

	dice[0] = d3;
	dice[1] = d0;
	dice[2] = d2;
	dice[3] = d5;
	dice[4] = d4;
	dice[5] = d1;
}

int bfs(int start_y, int start_x) {
	q.push({ start_y,start_x });
	visit[start_y][start_x] = 1;
	int cnt = 1;
	int target = map[start_y][start_x];

	while (!q.empty()) {
		int now_y = q.front().first;
		int now_x = q.front().second;

		q.pop();

		for (int d = 0; d < 4; d++) {
			int y = now_y + dy[d];
			int x = now_x + dx[d];

			if (y >= 0 && y < N && x >= 0 && x < N) {
				if (map[y][x] == target && visit[y][x] == 0) {
					cnt++;
					q.push({ y,x });
					visit[y][x] = 1;
				}
			}
		}
	}

	memset(visit, 0, sizeof(visit));

	return cnt;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int M;
	cin >> N >> M;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}

	for (int i = 1; i <= 6; i++) {
		dice[i - 1] = i;
	}

	int now_y = 0;
	int now_x = 0;
	int now_d = 0;
	result = 0;
	for (int t = 1; t <= M; t++) {
		int next_y = now_y + dy[now_d];
		int next_x = now_x + dx[now_d];

		if (next_y >= 0 && next_y < N && next_x >= 0 && next_x < N) {
			now_y = next_y;
			now_x = next_x;
		}
		else {
			now_d = turn_back(now_d);
			now_y += dy[now_d];
			now_x += dx[now_d];
		}

		if (now_d == 0) {
			right();
		}
		else if (now_d == 1) {
			down();
		}
		else if (now_d == 2) {
			left();
		}
		else {
			up();
		}

		int score = bfs(now_y, now_x);
		result += map[now_y][now_x] * score;

		if (dice[5] > map[now_y][now_x]) {
			now_d = turn_90(now_d);
		}
		else if (dice[5] < map[now_y][now_x]) {
			now_d = turn_270(now_d);
		}
	}
	cout << result;

	return 0;
}