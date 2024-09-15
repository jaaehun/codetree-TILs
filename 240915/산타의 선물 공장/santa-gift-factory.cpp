#include<iostream>
#include<unordered_map>
#include<vector>

using namespace std;

struct BELT {
	int first;
	int end;
	int wrong;
	int size;
};

struct BOX {
	int weight;
	int belt_num;
	int front;
	int back;
};

BELT belt[11];
int parent[11];
unordered_map<int, BOX> box;
int N, M;

int find_parent(int n) {
	if (parent[n] == n) {
		return n;
	}
	else {
		return parent[n] = find_parent(parent[n]);
	}
}

void init(int n, int m) {
	N = n;
	M = m;

	int belt_n = 1;
	int cnt = 0;
	vector<int> v_box;
	for (int i = 0; i < n; i++) {
		int id;
		cin >> id;
		v_box.push_back(id);
		cnt++;
		if (cnt == 1) {
			belt[belt_n].first = id;
			belt[belt_n].size++;
			box[id] = { 0,belt_n,-1,0 };
		}
		else if (cnt == n / m) {
			belt[belt_n].end = id;
			belt[belt_n].size++;
			box[v_box[i - 1]].back = id;
			box[id] = { 0,belt_n,v_box[i - 1],-1 };

			belt_n++;
			cnt = 0;
		}
		else {
			box[v_box[i - 1]].back = id;
			belt[belt_n].size++;
			box[id] = { 0,belt_n,v_box[i - 1],0 };
		}
	}

	for (int i = 0; i < n; i++) {
		int weight;
		cin >> weight;

		box[v_box[i]].weight = weight;
	}

	for (int i = 1; i <= m; i++) {
		parent[i] = i;
	}

	v_box.clear();
}

int unload(int w_max) {
	int sum = 0;
	for (int i = 1; i <= M; i++) {
		if (belt[i].wrong == 0 && belt[i].size != 0) {
			int now_first = belt[i].first;
			int now_end = belt[i].end;
			int now_front = box[now_first].front;
			int now_back = box[now_first].back;
			if (box[now_first].weight <= w_max) {
				if (belt[i].size == 1) {
					belt[i].first = -1;
					belt[i].end = -1;
				}
				else {
					belt[i].first = now_back;
					box[now_back].front = -1;
				}
				box[now_first].belt_num = -1;
				sum += box[now_first].weight;
				belt[i].size--;
			}
			else {
				if (belt[i].size == 1) {
					continue;
				}
				else {
					belt[i].end = now_first;
					belt[i].first = now_back;

					box[now_end].back = now_first;
					box[now_back].front = -1;
					box[now_first].front = now_end;
					box[now_first].back = -1;
				}
			}
		}
	}

	return sum;
}

int remove(int r_id) {
	if (box.find(r_id) == box.end()) {
		return -1;
	}
	else {
		if (box[r_id].belt_num == -1) {
			return -1;
		}
		else {
			int belt_n = box[r_id].belt_num;
			belt_n = find_parent(belt_n);
			int now_front = box[r_id].front;
			int now_back = box[r_id].back;
			if (now_front == -1 && now_back == -1) {
				belt[belt_n].first = -1;
				belt[belt_n].end = -1;
			}
			else if (now_front == -1)
			{
				belt[belt_n].first = now_back;
				box[now_back].front = -1;
			}
			else if (now_back == -1)
			{
				belt[belt_n].end = now_back;
				box[now_front].back = -1;
			}
			else
			{
				box[now_front].back = now_back;
				box[now_back].front = now_front;
			}
			box[r_id].belt_num = -1;
			belt[belt_n].size--;

			return 1;
		}
	}
}

int check(int f_id) {
	if (box.find(f_id) == box.end()) {
		return -1;
	}
	else {
		if (box[f_id].belt_num == -1) {
			return -1;
		}
		else {
			int belt_n = box[f_id].belt_num;
			belt_n = find_parent(belt_n);
			int now_front = box[f_id].front;
			int now_back = box[f_id].back;
			int now_first = belt[belt_n].first;
			int now_end = belt[belt_n].end;

			if (belt[belt_n].first == f_id) {
				// nothing
			}
			else if (belt[belt_n].end == f_id) {
				belt[belt_n].end = now_front;
				box[now_front].back = -1;
				belt[belt_n].first = f_id;
				box[now_first].front = f_id;
				box[f_id].front = -1;
				box[f_id].back = now_first;
			}
			else {
				belt[belt_n].end = now_front;
				box[now_front].back = -1;
				belt[belt_n].first = f_id;
				box[now_first].front = now_end;
				box[now_end].back = now_first;
				box[f_id].front = -1;
			}

			return belt_n;
		}
	}
}

int brocken(int n) {
	if (belt[n].wrong == 1) {
		return -1;
	}
	else {
		int tmp = n;
		while (true) {
			tmp++;
			if (tmp == M + 1) {
				tmp = 1;
			}
			if (belt[tmp].wrong == 0) {
				break;
			}
		}
		int now_end = belt[tmp].end;
		int prev_first = belt[n].first;
		box[now_end].back = prev_first;
		box[prev_first].front = now_end;
		belt[tmp].end = belt[n].end;
		
		belt[tmp].size += belt[n].size;

		belt[tmp].size = -1;
		belt[n].first = -1;
		belt[n].end = -1;
		belt[n].wrong = 1;

		parent[n] = tmp;

		return 1;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int Q;
	cin >> Q;
	for (int t = 1; t <= Q; t++) {
		int cmd;
		cin >> cmd;
		if (cmd == 100) {
			int n, m;
			cin >> n >> m;
			init(n, m);
		}
		else if (cmd == 200) {
			int w_max;
			cin >> w_max;
			int result = unload(w_max);
			cout << result << "\n";
		}
		else if (cmd == 300) {
			int r_id;
			cin >> r_id;
			int flag = remove(r_id);
			if (flag == -1) {
				cout << -1 << "\n";
			}
			else {
				cout << r_id << "\n";
			}
		}
		else if (cmd == 400) {
			int f_id;
			cin >> f_id;
			int result = check(f_id);
			if (result == -1) {
				cout << -1 << "\n";
			}
			else {
				cout << result << "\n";
			}
		}
		else {
			int b_num;
			cin >> b_num;
			int flag = brocken(b_num);
			if (flag == -1) {
				cout << -1 << "\n";
			}
			else {
				cout << b_num << "\n";
			}
		}
	}

	return 0;
}