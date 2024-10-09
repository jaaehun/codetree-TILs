#include<iostream>
#include<vector>

using namespace std;

struct INFO {
	int m;
	int s;
	int d;
};

int N;
vector<INFO> map[51][51];
vector<INFO> tmp_map[51][51];

int dy[8] = { -1,-1,0,1,1,1,0,-1 };
int dx[8] = { 0,1,1,1,0,-1,-1,-1 };

void move_atom() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j].size() != 0) {
				for (int v = 0; v < map[i][j].size(); v++) {
					int now_m = map[i][j][v].m;
					int now_s = map[i][j][v].s;
					int now_d = map[i][j][v].d;

					int next_y = i + now_s * dy[now_d];
					while (true) {
						next_y += N;
						if (next_y > 0) {
							break;
						}
					}
					next_y %= N;
					if (next_y == 0) {
						next_y = N;
					}
					int next_x = j + now_s * dx[now_d];
					while (true) {
						next_x += N;
						if (next_x > 0) {
							break;
						}
					}
					next_x %= N;
					if (next_x == 0) {
						next_x = N;
					}
					tmp_map[next_y][next_x].push_back({ now_m,now_s,now_d });
				}
				map[i][j].clear();
			}
		}
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (tmp_map[i][j].size() != 0) {
				if (tmp_map[i][j].size() == 1) {
					map[i][j] = tmp_map[i][j];
				}
				else {
					int sum_m = 0;
					int sum_s = 0;
					int cnt_d = 0;
					int size = tmp_map[i][j].size();

					for (int v = 0; v < size; v++) {
						sum_m += tmp_map[i][j][v].m;
						sum_s += tmp_map[i][j][v].s;

						int now_d = tmp_map[i][j][v].d;
						if (now_d % 2 == 0) {
							cnt_d++;
						}
					}
					sum_m /= 5;
					sum_s /= size;

					if (sum_m != 0) {
						int now_d = 0;
						if (cnt_d == 0 || cnt_d == size) {
							now_d = 0;
						}
						else {
							now_d = 1;
						}

						for (int d = 0; d < 4; d++) {
							map[i][j].push_back({ sum_m,sum_s,now_d });
							now_d += 2;
						}
					}
				}
				tmp_map[i][j].clear();
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int M, K;
	cin >> N >> M >> K;

	for (int i = 1; i <= M; i++) {
		int y, x, m, s, d;
		cin >> y >> x >> m >> s >> d;
		map[y][x].push_back({ m,s,d });
	}

	for (int t = 1; t <= K; t++) {
		move_atom();
	}

	int result = 0;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j].size() != 0) {
				for (int v = 0; v < map[i][j].size(); v++) {
					result += map[i][j][v].m;
				}
			}
		}
	}

	cout << result;

	return 0;

}