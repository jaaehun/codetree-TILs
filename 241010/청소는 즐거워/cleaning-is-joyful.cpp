#include<iostream>

using namespace std;

int N;
int map[500][500];
int cur_y, cur_x;
int prev_d;
int cur_d, cur_cnt, small_cnt, big_cnt;
int result;

int dy[4] = { 0,1,0,-1 };
int dx[4] = { -1,0,1,0 };

int turn_left(int d) {
	if (d == 3) {
		return 0;
	}
	else {
		return d + 1;
	}
}

int turn_right(int d) {
	if (d == 0) {
		return 3;
	}
	else {
		return d - 1;
	}
}

void move_position() {
	cur_y += dy[cur_d];
	cur_x += dx[cur_d];
	prev_d = cur_d;
	cur_cnt++;
	if (cur_cnt == small_cnt) {
		cur_cnt = 0;
		big_cnt++;
		cur_d = turn_left(cur_d);
		if (big_cnt == 2) {
			big_cnt = 0;
			small_cnt++;
		}
	}
}

void put_dust(int yy, int xx, int d, int s, int n) {
	int y = yy + s * dy[d];
	int x = xx + s * dx[d];
	if (y >= 0 && y < N && x >= 0 && x < N) {
		map[y][x] += n;
	}
	else {
		result += n;
	}
}

void move_dust() {
	int total = map[cur_y][cur_x];
	int per1 = total * 1 / 100;
	int per2 = total * 2 / 100;
	int per5 = total * 5 / 100;
	int per7 = total * 7 / 100;
	int per10 = total * 10 / 100;
	int pera = total - 2 * (per1 + per2 + per7 + per10) - per5;


	int now_y = cur_y;
	int now_x = cur_x;
	int now_d = prev_d;
	int tmp_y = cur_y;
	int tmp_x = cur_x;
	int tmp_d = prev_d;
	
	map[now_y][now_x] = 0;

	put_dust(now_y, now_x, now_d, 1, pera);
	put_dust(now_y, now_x, now_d, 2, per5);

	now_y = cur_y;
	now_x = cur_x;
	now_d = turn_right(now_d);
	tmp_y = now_y + dy[now_d];
	tmp_x = now_x + dx[now_d];
	tmp_d = now_d;
	put_dust(now_y, now_x, now_d, 1, per7);
	put_dust(now_y, now_x, now_d, 2, per2);

	now_y = tmp_y;
	now_x = tmp_x;
	now_d = turn_right(tmp_d);
	put_dust(now_y, now_x, now_d, 1, per1);
	now_d = turn_left(tmp_d);
	put_dust(now_y, now_x, now_d, 1, per10);

	now_y = cur_y;
	now_x = cur_x;
	now_d = turn_left(now_d);
	tmp_y = now_y + dy[now_d];
	tmp_x = now_x + dx[now_d];
	tmp_d = now_d;
	put_dust(now_y, now_x, now_d, 1, per7);
	put_dust(now_y, now_x, now_d, 2, per2);

	now_y = tmp_y;
	now_x = tmp_x;
	now_d = turn_right(tmp_d);
	put_dust(now_y, now_x, now_d, 1, per10);
	now_d = turn_left(tmp_d);
	put_dust(now_y, now_x, now_d, 1, per1);
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}

	cur_y = N / 2;
	cur_x = N / 2;
	cur_d = 0;
	cur_cnt = 0;
	small_cnt = 1;
	big_cnt = 0;
	prev_d = 0;
	result = 0;
	while (true) {
		move_position();
		move_dust();
		if (cur_y == 0 && cur_x == 0) {
			break;
		}
	}

	cout << result;

	return 0;
}