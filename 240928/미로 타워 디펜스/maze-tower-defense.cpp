#include<iostream>
#include<vector>
#include<string.h>

using namespace std;

int N;
int map[26][26];
int result;
vector<int> v;
vector<int> v2;
vector<int> tmp;

int dy[4] = { 0,1,0,-1 };
int dx[4] = { 1,0,-1,0 };

int turn(int d) {
	if (d == 0) {
		return 3;
	}
	else {
		return d - 1;
	}
}

void attack(int d, int p) {
	int now_y = N / 2;
	int now_x = N / 2;
	for (int i = 1; i <= p; i++) {
		int y = now_y + i * dy[d];
		int x = now_x + i * dx[d];

		if (map[y][x] > 0) {
			result += map[y][x];
			map[y][x] = -2;
		}
	}
}

void put() {
	int now_y = N / 2;
	int now_x = N / 2;
	int now_d = 2;
	int now_n = 1;
	int small = 0;
	int big = 0;
	while (true) {
		now_y += dy[now_d];
		now_x += dx[now_d];
		if (map[now_y][now_x] > 0) {
			v.push_back(map[now_y][now_x]);
		}
		else if (map[now_y][now_x] == 0) {
			break;
		}
		small++;
		if (small == now_n) {
			now_d = turn(now_d);
			small = 0;
			big++;
		}
		if (big == 2) {
			now_n++;
			big = 0;
		}
	}
}

void score() {
	while (true) {
		int flag = 0;
		int now_idx = 0;
		int cnt = 1;
		while (true) {
			tmp.push_back(v[now_idx]);
			now_idx++;
			if (v[now_idx] == v[now_idx - 1]) {
				cnt++;
			}
			else {
				if (cnt >= 4) {
					result += v[now_idx - 1] * cnt;
					flag = 1;
				}
				else {
					for (int i = 0; i < tmp.size(); i++) {
						v2.push_back(tmp[i]);
					}
				}
				tmp.clear();
				cnt = 1;
			}
			if (now_idx == v.size() - 1) {
				if (cnt >= 4) {
					result += v[now_idx] * cnt;
					flag = 1;
				}
				else {
					for (int i = 0; i < tmp.size(); i++) {
						v2.push_back(tmp[i]);
					}
					v2.push_back(v[now_idx]);
				}
				tmp.clear();
				break;
			}
		}
		v = v2;
		v2.clear();
		if (flag == 0) {
			break;
		}
	}
}

void remake() {
	int now_idx = 0;
	int cnt = 1;
	while (true) {
		now_idx++;
		if (v[now_idx] == v[now_idx - 1]) {
			cnt++;
		}
		else {
			v2.push_back(cnt);
			v2.push_back(v[now_idx - 1]);
			cnt = 1;
		}
		if (now_idx == v.size() - 1) {
			v2.push_back(cnt);
			v2.push_back(v[now_idx]);
			break;
		}
	}
	v = v2;
	v2.clear();
}

void move() {
	memset(map, 0, sizeof(map));
	int now_y = N / 2;
	int now_x = N / 2;
	int now_d = 2;
	int now_n = 1;
	int small = 0;
	int big = 0;
	int now_idx = 0;
	while (true) {
		now_y += dy[now_d];
		now_x += dx[now_d];
		map[now_y][now_x] = v[now_idx];
		now_idx++;
		if (now_idx == v.size()) {
			break;
		}
		if (now_y == 0 && now_x == 0) {
			break;
		}
		small++;
		if (small == now_n) {
			now_d = turn(now_d);
			small = 0;
			big++;
		}
		if (big == 2) {
			now_n++;
			big = 0;
		}
	}
	v.clear();
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
	result = 0;
	for (int i = 1; i <= M; i++) {
		int d, p;
		cin >> d >> p;
		attack(d, p);
		put();
		if (v.size() == 0) {
			break;
		}
		score();
		remake();
		move();
	}
	cout << result;

	return 0;
}