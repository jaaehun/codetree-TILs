#include<iostream>

using namespace std;

int map[10][10];
int result;

void move_1(int y, int x) {
	int now_y = y;
	int now_x = x;
	while (true) {
		int next_x = now_x + 1;
		if (next_x == 10) {
			map[now_y][now_x] = 1;
			break;
		}
		if (map[now_y][next_x] == 1) {
			map[now_y][now_x] = 1;
			break;
		}
		now_x = next_x;
	}
	now_x = x;
	while (true) {
		int next_y = now_y + 1;
		if (next_y == 10) {
			map[now_y][now_x] = 1;
			break;
		}
		if (map[next_y][now_x] == 1) {
			map[now_y][now_x] = 1;
			break;
		}
		now_y = next_y;
	}
}

void move_2(int y, int x) {
	int now_y = y;
	int now_x = x + 1;
	while (true) {
		int next_x = now_x + 1;
		if (next_x == 10) {
			map[now_y][now_x] = 1;
			map[now_y][now_x - 1] = 1;
			break;
		}
		if (map[now_y][next_x] == 1) {
			map[now_y][now_x] = 1;
			map[now_y][now_x - 1] = 1;
			break;
		}
		now_x = next_x;
	}
	now_x = x;
	while (true) {
		int next_y = now_y + 1;
		if (next_y == 10) {
			map[now_y][now_x] = 1;
			map[now_y][now_x + 1] = 1;
			break;
		}
		if (map[next_y][now_x] == 1 || map[next_y][now_x + 1] == 1) {
			map[now_y][now_x] = 1;
			map[now_y][now_x + 1] = 1;
			break;
		}
		now_y = next_y;
	}
}

void move_3(int y, int x) {
	int now_y = y;
	int now_x = x;
	while (true) {
		int next_x = now_x + 1;
		if (next_x == 10) {
			map[now_y][now_x] = 1;
			map[now_y + 1][now_x] = 1;
			break;
		}
		if (map[now_y][next_x] == 1 || map[now_y + 1][next_x] == 1) {
			map[now_y][now_x] = 1;
			map[now_y + 1][now_x] = 1;
			break;
		}
		now_x = next_x;
	}
	now_y = y + 1;
	now_x = x;
	while (true) {
		int next_y = now_y + 1;
		if (next_y == 10) {
			map[now_y][now_x] = 1;
			map[now_y - 1][now_x] = 1;
			break;
		}
		if (map[next_y][now_x] == 1) {
			map[now_y][now_x] = 1;
			map[now_y - 1][now_x] = 1;
			break;
		}
		now_y = next_y;
	}
}

void move_puzzle(int t, int y, int x) {
	if (t == 1) {
		move_1(y, x);
	}
	else if (t == 2) {
		move_2(y, x);
	}
	else {
		move_3(y, x);
	}
}

void puzzle_right(int n) {
	int now_x = n;
	while (true) {
		if (now_x == 3) {
			break;
		}
		for (int i = 0; i < 4; i++) {
			map[i][now_x] = map[i][now_x - 1];
		}
		now_x--;
	}
}

void puzzle_down(int n) {
	int now_y = n;
	while (true) {
		if (now_y == 3) {
			break;
		}
		for (int i = 0; i < 4; i++) {
			map[now_y][i] = map[now_y - 1][i];
		}
		now_y--;
	}
}


void clear_puzzle() {
	for (int i = 9; i >= 6; i--) {
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (map[j][i] == 1) {
				cnt++;
			}
		}
		if (cnt == 4) {
			puzzle_right(i);
			i++;
			result++;
		}
	}

	for (int i = 9; i >= 6; i--) {
		int cnt = 0;
		for (int j = 0; j < 4; j++) {
			if (map[i][j] == 1) {
				cnt++;
			}
		}
		if (cnt == 4) {
			puzzle_down(i);
			i++;
			result++;
		}
	}
}

void push_puzzle() {
	while (true) {
		int flag = 0;
		for (int i = 0; i < 4; i++) {
			if (map[i][5] == 1) {
				flag = 1;
			}
		}
		if (flag == 0) {
			break;
		}
		else {
			puzzle_right(9);
		}
	}
	while (true) {
		int flag = 0;
		for (int i = 0; i < 4; i++) {
			if (map[5][i] == 1) {
				flag = 1;
			}
		}
		if (flag == 0) {
			break;
		}
		else {
			puzzle_down(9);
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int k;
	cin >> k;

	result = 0;
	for (int q = 1; q <= k; q++) {
		int t, y, x;
		cin >> t >> y >> x;

		move_puzzle(t, y, x);
		clear_puzzle();
		push_puzzle();
	}

	int sum = 0;
	for (int i = 6; i < 10; i++) {
		for (int j = 0; j < 4; j++) {
			if (map[i][j] == 1) {
				sum++;
			}
			if (map[j][i] == 1) {
				sum++;
			}
		}
	}

	cout << result << "\n" << sum;

	return 0;
}