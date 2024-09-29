#include<iostream>
#include<vector>

using namespace std;

struct Node {
	int parent;
	int children[2];
	int authority;
	int alert;

	Node() {
		parent = -1;
		for (int i = 0; i < 2; i++) {
			children[i] = -1;
		}
		authority = 0;
		alert = 1;
	}
};

int N;
Node chat[100001];
int cnt;

void init() {
	for (int i = 1; i <= N; i++) {
		int p;
		cin >> p;
		chat[i].parent = p;
		if (chat[p].children[0] == -1) {
			chat[p].children[0] = i;
		}
		else {
			chat[p].children[1] = i;
		}
	}

	for (int i = 1; i <= N; i++) {
		int a;
		cin >> a;
		chat[i].authority = a;
	}
}

void set_alert(int n) {
	if (chat[n].alert == 1) {
		chat[n].alert = -1;
	}
	else {
		chat[n].alert = 1;
	}
}

void set_authority(int n, int p) {
	chat[n].authority = p;
}

void change_parent(int c1, int c2) {
	int parent1 = chat[c1].parent;
	int parent2 = chat[c2].parent;

	chat[c1].parent = parent2;
	chat[c2].parent = parent1;

	for (int i = 0; i < 2; i++) {
		if (chat[parent1].children[i] == c1) {
			chat[parent1].children[i] = c2;
		}
		if (chat[parent2].children[i] == c2) {
			chat[parent2].children[i] = c1;
		}
	}
}

void calculate(int n, int depth) {
	for (int i = 0; i < 2; i++) {
		if (chat[n].children[i] != -1) {
			int now = chat[n].children[i];
			if (chat[now].alert == 1) {
				int power = chat[now].authority;
				if (power >= depth) {
					cnt++;
				}
				calculate(now, depth + 1);
			}
		}
	}
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
			cnt = 0;
			calculate(c, 1);
			cout << cnt << "\n";
		}
	}
	return 0;
}