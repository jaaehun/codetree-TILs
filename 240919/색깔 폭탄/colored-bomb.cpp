#include<iostream>
#include<string.h>
#include<queue>

using namespace std;

struct INFO {
	int total;
	int red;
	int y;
	int x;
};

bool operator< (const INFO& A, const INFO& B) {
	if (A.total != B.total) {
		return A.total < B.total;
	}
	else if (A.red != B.red) {
		return A.red > B.red;
	}else if (A.y != B.x) {
		return A.y < B.y;
	}
	else {
		return A.x > B.x;
	}
}

int N, M;
int map[21][21];
int copy_map[21][21];
int visit[21][21];
int visit2[21][21];
int result;

priority_queue<INFO> group;
queue<pair<int,int>> q;

int dy[4] = { -1,1,0,0 };
int dx[4] = { 0,0,-1,1 };

void bfs(int start_y, int start_x) {
	int color = map[start_y][start_x];
	q.push({ start_y,start_x });
	visit[start_y][start_x] = 1;
	visit2[start_y][start_x] = 1;

	int gtotal = 1;
	int gred = 0;
	int gy = start_y;
	int gx = start_x;
	while(!q.empty()) {
		int now_y = q.front().first;
		int now_x = q.front().second;

		q.pop();

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y > 0 && y <= N && x > 0 && x <= N) {
				if (visit2[y][x] == 0) {
					if (map[y][x] == color) {
						gtotal++;
						q.push({ y,x });
						visit[y][x] = 1;
						visit2[y][x] = 1;
						if (y > gy) {
							gy = y;
							gx = x;
						}
						else if (y == gy) {
							if (x < gx) {
								gy = y;
								gx = x;
							}
						}
					}
					else if (map[y][x] == 0) {
						gtotal++;
						gred++;
						q.push({ y,x });
						visit2[y][x] = 1;
					}
				}
			}
		}
	}

	if (gtotal >= 2) {
		group.push({ gtotal,gred,gy,gx });
	}
	memset(visit2, 0, sizeof(visit2));
}

void bfs2(int start_y, int start_x) {
	int color = map[start_y][start_x];
	q.push({ start_y,start_x });
	map[start_y][start_x] = -2;
	visit2[start_y][start_x] = 1;

	int C = 1;
	while (!q.empty()) {
		int now_y = q.front().first;
		int now_x = q.front().second;

		q.pop();

		for (int i = 0; i < 4; i++) {
			int y = now_y + dy[i];
			int x = now_x + dx[i];

			if (y > 0 && y <= N && x > 0 && x <= N) {
				if (visit2[y][x] == 0) {
					if (map[y][x] == color || map[y][x] == 0) {
						C++;
						q.push({ y,x });
						map[y][x] = -2;
						visit2[y][x] = 1;
					}
				}
			}
		}
	}

	result += C * C;
	memset(visit2, 0, sizeof(visit2));
}

int bomb() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j] > 0 && visit[i][j] == 0) {
				bfs(i, j);
			}
		}
	}
	if (group.size() == 0) {
		memset(visit, 0, sizeof(visit));
		return -1;
	}
	else {
		INFO target = group.top();
		while (!group.empty()) {
			group.pop();
		}
		int target_y = target.y;
		int target_x = target.x;

		bfs2(target_y, target_x);
		memset(visit, 0, sizeof(visit));

		return 1;
	}
}

void gravity() {
	for (int i = N; i > 1; i--) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j] == -2) {
				int now_y = i;
				while (true) {
					now_y--;
					if (now_y == 0) {
						break;
					}
					if (map[now_y][j] >= 0) {
						map[i][j] = map[now_y][j];
						map[now_y][j] = -2;
						break;
					}
					if (map[now_y][j] == -1) {
						break;
					}
				}
			}
		}
	}
}

void spin() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			copy_map[1 + j][1 + i] = map[1 + i][N - j];
		}
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			map[i][j] = copy_map[i][j];
		}
	}

	memset(copy_map, 0, sizeof(copy_map));
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	cin >> N >> M;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> map[i][j];
		}
	}

	result = 0;
	while (true) {
		int flag = bomb();
		if (flag == -1) {
			break;
		}
		gravity();
		spin();
		gravity();
	}

	cout << result;

	return 0;
}