#include<iostream>
#include<list>
#include<unordered_map>
#include<algorithm>

using namespace std;

int N, M;
unordered_map<int, int> um;
list<int> belt[10];
int where[100001];
int weight[100001];
int wrong[10];
int parent[10];

int find_idx(int id) {
	if (um.find(id) != um.end()) {
		return um[id];
	}
	else {
		return -1;
	}
}

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

	int belt_n = 0;
	int cnt = 0;
	for (int i = 0; i < n; i++) {
		int id;
		cin >> id;

		um[id] = um.size();
		belt[belt_n].push_back(um[id]);
		where[um[id]] = belt_n;
		cnt++;
		if (cnt == n / m) {
			belt_n++;
			cnt = 0;
		}
	}

	for (int i = 0; i < n; i++) {
		int w;
		cin >> w;
		weight[i] = w;
	}

	for (int i = 0; i < m; i++) {
		parent[i] = i;
	}
}

int get(int w_max) {
	int sum = 0;
	for (int i = 0; i < M; i++) {
		if (wrong[i] == 0) {
			auto iter = belt[i].begin();
			int now_idx = *iter;
			int now_weight = weight[*iter];

			if (now_weight <= w_max) {
				sum += now_weight;
				belt[i].pop_front();
				where[now_idx] = -1;
			}
			else {
				belt[i].pop_front();
				belt[i].push_back(now_idx);
			}
		}
	}

	return sum;
}

int remove(int r_id) {
	int now_idx = find_idx(r_id);
	if (now_idx == -1) {
		return -1;
	}
	else {
		if (where[now_idx] == -1) {
			return -1;
		}
		else {
			int now_belt = where[now_idx];
			now_belt = find_parent(now_belt);
			auto iter = find(belt[now_belt].begin(), belt[now_belt].end(), now_idx);
			belt[now_belt].erase(iter);
			where[now_idx] = -1;

			return 1;
		}
	}
}

int check(int r_id) {
	int now_idx = find_idx(r_id);
	if (now_idx == -1) {
		return -1;
	}
	else {
		if (where[now_idx] == -1) {
			return -1;
		}
		else {
			int now_belt = where[now_idx];
			now_belt = find_parent(now_belt);
			auto iter = find(belt[now_belt].begin(), belt[now_belt].end(), now_idx);
			list<int> tmp;
			tmp.splice(tmp.begin(), belt[now_belt], iter, belt[now_belt].end());
			belt[now_belt].splice(belt[now_belt].begin(), tmp);

			return now_belt + 1;
		}
	}
}

int broken(int b_num) {
	b_num--;
	if (wrong[b_num] == 1) {
		return -1;
	}
	else {
		int now_n = b_num;
		while (true) {
			now_n++;
			if (now_n == M) {
				now_n = 0;
			}
			if (wrong[now_n] == 0) {
				break;
			}
		}
		belt[now_n].splice(belt[now_n].end(), belt[b_num]);
		wrong[b_num] = 1;
		parent[b_num] = now_n;

		return 1;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int q;
	cin >> q;
	for (int t = 1; t <= q; t++) {
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
			int result = get(w_max);
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
			cout << result << "\n";
		}
		else {
			int b_num;
			cin >> b_num;
			int flag = broken(b_num);
			if (flag == -1) {
				cout <<  -1 << "\n";
			}
			else {
				cout << b_num << "\n";
			}
		}
	}
	return 0;
}