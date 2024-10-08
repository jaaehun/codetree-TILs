#include<iostream>
#include<queue>
#include<string.h>

using namespace std;

struct INFO {
	int y;
	int x;
};

int N;
int wall_map[21][21];
int map[21][21];
int visit[21][21];
INFO goal_info[401];
int goal_cnt;
int taxi_y, taxi_x;
int battery;
int target_n, target_y, target_x;
queue < pair<pair<int, int>, int>> q;

int dy[4] = { 0,0,-1,1 };
int dx[4] = { -1,1,0,0 };

int pick_player(int start_y, int start_x) {
	q.push({ {start_y,start_x},0 });
	visit[start_y][start_x] = 1;

	int max_cnt = -1;
	while (!q.empty()) {
		int now_y = q.front().first.first;
		int now_x = q.front().first.second;
		int cnt = q.front().second;

		q.pop();

		if (map[now_y][now_x] != 0) {
			if (max_cnt == -1) {
				max_cnt = cnt;
				target_n = map[now_y][now_x];
				target_y = now_y;
				target_x = now_x;
			}
			else {
				if (cnt > max_cnt) {
					while (!q.empty()) {
						q.pop();
					}
					break;
				}
				else if (cnt == max_cnt) {
					if (now_y < target_y) {
						target_n = map[now_y][now_x];
						target_y = now_y;
						target_x = now_x;
					}
					else if (now_y == target_y) {
						if (now_x < target_x) {
							target_n = map[now_y][now_x];
							target_y = now_y;
							target_x = now_x;
						}
					}
				}
			}
		}

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y > 0 && y <= N && x > 0 && x <= N) {
				if (wall_map[y][x] == 0 && visit[y][x] == 0) {
					q.push({ {y,x},cnt + 1 });
					visit[y][x] = 1;
				}
			}
		}
	}

	memset(visit, 0, sizeof(visit));

	return max_cnt;
}

int move_goal(int start_y, int start_x) {
	int goal_y = goal_info[target_n].y;
	int goal_x = goal_info[target_n].x;

	q.push({ {start_y,start_x},0 });
	visit[start_y][start_x] = 1;

	int max_cnt = 0;
	while (!q.empty()) {
		int now_y = q.front().first.first;
		int now_x = q.front().first.second;
		int cnt = q.front().second;

		q.pop();

		if (now_y == goal_y && now_x == goal_x) {
			max_cnt = cnt;
			taxi_y = now_y;
			taxi_x = now_x;
			while (!q.empty()) {
				q.pop();
			}
			goal_cnt++;
			break;
		}

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y > 0 && y <= N && x > 0 && x <= N) {
				if (wall_map[y][x] == 0 && visit[y][x] == 0) {
					q.push({ {y,x},cnt + 1 });
					visit[y][x] = 1;
				}
			}
		}
	}

	memset(visit, 0, sizeof(visit));

	return max_cnt;
}


int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int M, C;
	cin >> N >> M >> C;

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> wall_map[i][j];
		}
	}

	cin >> taxi_y >> taxi_x;

	for (int i = 1; i <= M; i++) {
		int y_s, x_s, y_e, x_e;
		cin >> y_s >> x_s >> y_e >> x_e;
		map[y_s][x_s] = i;
		goal_info[i].y = y_e;
		goal_info[i].x = x_e;
	}

	goal_cnt = 0;
	battery = C;
	while (true) {
		int dis = pick_player(taxi_y, taxi_x);
		battery -= dis;
		if (battery < 0) {
			battery = -1;
			break;
		}
		taxi_y = target_y;
		taxi_x = target_x;
		map[target_y][target_x] = 0;
		dis = move_goal(taxi_y, taxi_x);
		battery -= dis;
		if (battery < 0) {
			battery = -1;
			break;
		}
		battery += dis * 2;
		if (goal_cnt == M) {
			break;
		}
	}

	cout << battery;

	return 0;
}