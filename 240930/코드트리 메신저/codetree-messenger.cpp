#include<iostream>
#include<unordered_map>

using namespace std;

struct Node {
	int parent;
	int authority;
	int alert;
	unordered_map<int, int> cnt;

	Node() {
		parent = -1;
		authority = 0;
		alert = 1;
	}
};

int N;
Node chat[100001];

void init() {
	for (int i = 1; i <= N; i++) {
		int p;
		cin >> p;
		chat[i].parent = p;
	}

	for (int i = 1; i <= N; i++) {
		int a;
		cin >> a;
		if (a > 20) {
			a = 20;
		}
		chat[i].authority = a;
		int now = i;
		for (int j = 0; j < a; j++) {
			now = chat[now].parent;
			if (now == 0) {
				break;
			}
			chat[now].cnt[i] = 1;
		}
	}
}

void cnt_minus(int c) {
	int power = chat[c].authority;
	int now = c;
	for (int i = 0; i < power; i++) {
		if (chat[now].alert == -1) {
			break;
		}
		now = chat[now].parent;
		if (now == 0) {
			break;
		}
		chat[now].cnt[c] = -1;
	}
	for (auto iter = chat[c].cnt.begin(); iter != chat[c].cnt.end(); ++iter) {
		if (iter->second == 1) {
			int num = iter->first;
			now = num;
			power = chat[num].authority;
			int flag = 0;

			for (int i = 0; i < power; i++) {
				now = chat[now].parent;
				if (now == 0) {
					break;
				}
				if (flag == 1) {
					chat[now].cnt[num] = -1;
				}
				if (now == c) {
					flag = 1;
				}
				if (chat[now].alert == -1) {
					break;
				}
			}
		}
	}
}

void cnt_plus(int c) {
	int power = chat[c].authority;
	int now = c;
	for (int i = 0; i < power; i++) {
		if (chat[now].alert == -1) {
			break;
		}
		now = chat[now].parent;
		if (now == 0) {
			break;
		}
		chat[now].cnt[c] = 1;
	}
	for (auto iter = chat[c].cnt.begin(); iter != chat[c].cnt.end(); ++iter) {
		if (iter->second == 1) {
			int num = iter->first;
			now = num;
			power = chat[num].authority;
			int flag = 0;
			for (int i = 0; i < power; i++) {
				now = chat[now].parent;
				if (now == 0) {
					break;
				}
				if (flag == 1) {
					chat[now].cnt[num] = 1;
				}
				if (now == c) {
					flag = 1;
				}
				if (chat[now].alert == -1) {
					break;
				}
			}
		}
	}
}

void set_alert(int n) {
	if (chat[n].alert == 1) {
		cnt_minus(n);
		chat[n].alert = -1;
	}
	else {
		chat[n].alert = 1;
		cnt_plus(n);
	}
}

void set_authority(int n, int p) {
	int now = n;
	int power = chat[n].authority;
	for (int i = 0; i < power; i++) {
		now = chat[now].parent;
		if (now == 0) {
			break;
		}
		chat[now].cnt[n] = -1;
		if (chat[now].alert == -1) {
			break;
		}
	}

	power = min(20, p);
	chat[n].authority = power;

	now = n;
	for (int i = 0; i < power; i++) {
		now = chat[now].parent;
		if (now == 0) {
			break;
		}
		chat[now].cnt[n] = 1;
		if (chat[now].alert == -1) {
			break;
		}
	}
}

void change_parent(int c1, int c2) {
	int parent1 = chat[c1].parent;
	int parent2 = chat[c2].parent;

	if (parent1 != parent2) {
		cnt_minus(c1);
		cnt_minus(c2);

		chat[c1].parent = parent2;
		chat[c2].parent = parent1;

		cnt_plus(c1);
		cnt_plus(c2);
	}
}

int calculate(int n) {
	int cnt = 0;
	for (auto iter = chat[n].cnt.begin(); iter != chat[n].cnt.end(); ++iter) {
		if (iter->second == 1) {
			cnt++;
		}
	}

	return cnt;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int Q;
	cin >> N >> Q;

	for (int t = 1; t <= Q; t++) {
		int cmd;
		cin >> cmd;
		
		if (cmd == 100) {
			init();
		}
		else if (cmd == 200) {
			int c;
			cin >> c;
			set_alert(c);
		}
		else if (cmd == 300) {
			int c, power;
			cin >> c >> power;
			set_authority(c, power);
		}
		else if (cmd == 400) {
			int c1, c2;
			cin >> c1 >> c2;
			change_parent(c1, c2);
		}
		else {
			int c;
			cin >> c;
			int result = calculate(c);
			cout << result << "\n";
		}
	}
	return 0;
}