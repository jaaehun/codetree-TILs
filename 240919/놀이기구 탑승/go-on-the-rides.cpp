#include<iostream>
#include<vector>
#include<cmath>

using namespace std;

int map[21][21];
vector<int> like_friend[401];

int dy[4] = { -1,1,0,0 };
int dx[4] = { 0,0,-1,1 };

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n;
	cin >> n;

	for (int p = 0; p < n * n; p++) {
		int num;
		cin >> num;

		for (int f = 0; f < 4; f++) {
			int a;
			cin >> a;
			like_friend[num].push_back(a);
		}

		int max_y = 0;
		int max_x = 0;
		int max_friend = -1;
		int max_empty = -1;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (map[i][j] == 0) {
					int now_friend = 0;
					int now_empty = 0;
					for (int d = 0; d < 4; d++) {
						int now_y = i + dy[d];
						int now_x = j + dx[d];
						if (now_y >= 0 && now_y < n && now_x >= 0 && now_x < n) {
							if (map[now_y][now_x] == 0) {
								now_empty++;
							}
							for (int v = 0; v < 4; v++) {
								if (like_friend[num][v] == map[now_y][now_x]) {
									now_friend++;
								}
							}
						}
					}
					if (now_friend > max_friend) {
						max_y = i;
						max_x = j;
						max_friend = now_friend;
						max_empty = now_empty;
					}
					else if (now_friend == max_friend) {
						if (now_empty > max_empty) {
							max_y = i;
							max_x = j;
							max_empty = now_empty;
						}
					}
				}
			}
		}
		map[max_y][max_x] = num;
	}

	int result = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int num = map[i][j];
			int cnt = 0;
			for (int d = 0; d < 4; d++) {
				int now_y = i + dy[d];
				int now_x = j + dx[d];
				if (now_y >= 0 && now_y < n && now_x >= 0 && now_x < n) {
					
					for (int v = 0; v < 4; v++) {
						if (like_friend[num][v] == map[now_y][now_x]) {
							cnt++;
						}
					}
				}
			}
			result += pow(10, cnt - 1);
		}
	}

	cout << result;

	return 0;
}