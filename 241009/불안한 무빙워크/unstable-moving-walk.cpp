#include<iostream>

using namespace std;

struct INFO {
	int start;
	int n;
	int end;
};

int N, K;
int arr[101];
int map[101];
int broke_cnt;
INFO movingwalk;

void moving() {
	int now_start = movingwalk.start;
	int now_N = movingwalk.n;
	int now_end = movingwalk.end;

	movingwalk.start = now_end;
	movingwalk.n = now_N - 1;
	if (now_N - 1 == 0) {
		movingwalk.n = 2 * N;
	}
	if (map[movingwalk.n] == 1) {
		map[movingwalk.n] = 0;
	}
	movingwalk.end = now_end - 1;
	if (now_end - 1 == 0) {
		movingwalk.end = 2 * N;
	}
}

void move_people() {
	int now_start = movingwalk.start;
	int now_N = movingwalk.n;
	int now_end = movingwalk.end;

	int now = now_N;
	while (true) {
		int prev = now - 1;
		if (prev == 0) {
			prev = 2 * N;
		}

		if (map[prev] == 1) {
			if (map[now] == 0 && arr[now] != 0) {
				map[prev] = 0;
				map[now] = 1;
				if (now == now_N) {
					map[now] = 0;
				}
				arr[now]--;
				if (arr[now] == 0) {
					broke_cnt++;
				}
			}
		}
		now = prev;
		if (now == now_start) {
			break;
		}
	}
}

void put_first() {
	int now_start = movingwalk.start;
	int now_N = movingwalk.n;
	int now_end = movingwalk.end;

	if (map[now_start] == 0 && arr[now_start] != 0) {
		map[now_start] = 1;
		arr[now_start]--;
		if (arr[now_start] == 0) {
			broke_cnt++;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	cin >> N >> K;
	for (int i = 1; i <= 2 * N; i++) {
		cin >> arr[i];
	}

	movingwalk.start = 1;
	movingwalk.n = N;
	movingwalk.end = 2 * N;

	int result = 1;
	broke_cnt = 0;
	while (true) {
		moving();
		move_people();
		put_first();
		if (broke_cnt >= K) {
			break;
		}
		result++;
	}

	cout << result;

	return 0;
}