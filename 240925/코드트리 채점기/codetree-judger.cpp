#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<iostream>
#include<vector>
#include<string>
#include<queue>
#include<unordered_map>

using namespace std;

struct INFO {
	int level;
	int time;
	string name;
};

bool operator> (const INFO& a, const INFO& b) {
	if (a.level != b.level) return a.level > b.level;
	return a.time > b.time;
}

struct INFO2 {
	int start;
	string u;
};

struct INFO3 {
	int start;
	int gap;
};

int N;
int run_cnt;
int result;
unordered_map<string, int> um;
priority_queue<INFO, vector<INFO>, greater<>> waiting_queue[301];
unordered_map<string,int> waiting;
int run[301];
INFO3 finish[301];
priority_queue<int,vector<int>,greater<>> muchine_queue;
INFO2 muchine[50001];

int find_idx(string s) {
	return um[s];
}

void init(int n, string u) {
	N = n;

	for (int i = 0; i < N; i++) {
		muchine_queue.push(i);
	}
	string domain = "";
	for (int i = 0; i < u.size(); i++) {
		if (u[i] == '/') {
			break;
		}
		domain += u[i];
	}

	if (um.find(domain) == um.end()) {
		um[domain] = um.size();
	}
	int now_idx = find_idx(domain);

	waiting_queue[now_idx].push({ 1,0,u });
	waiting[u] = 1;
	run_cnt = 0;
	result = 1;
}

void request(int t, int p, string u) {
	if (waiting[u] == 1) {
		return;
	}
	else {
		string domain = "";
		for (int i = 0; i < u.size(); i++) {
			if (u[i] == '/') {
				break;
			}
			domain += u[i];
		}
		if (um.find(domain) == um.end()) {
			um[domain] = um.size();
		}
		int now_idx = find_idx(domain);
		waiting_queue[now_idx].push({ p,t,u });
		waiting[u] = 1;
		result++;
	}
}

void attempt(int t) {
	if (run_cnt < N) {
		INFO min = { 987654321,987654321,"" };
		int min_idx = 0;
		for (int i = 0; i < 301; i++) {
			if (run[i] == 1) {
				continue;
			}
			if (finish[i].start != 0 || finish[i].gap != 0) {
				if (finish[i].gap != 0 || finish[i].start != 0) {
					if (t < finish[i].start + 3 * finish[i].gap) {
						continue;
					}
				}
			}
			if (!waiting_queue[i].empty()) {
				INFO tmp = waiting_queue[i].top();
				if (min > tmp) {
					min = tmp;
					min_idx = i;
				}
			}
		}
		if (min.name == "") {
			return;
		}
		else {
			waiting_queue[min_idx].pop();
			result--;
			waiting[min.name] = 0;
			run[min_idx] = 1;
			run_cnt++;

			int muchine_n = muchine_queue.top();
			muchine_queue.pop();
			muchine[muchine_n].start = t;
			muchine[muchine_n].u = min.name;
		}
	}
}

void complete(int t, int num) {
	if (muchine[num - 1].start != 0 || muchine[num - 1].u != "") {
		int start = muchine[num - 1].start;
		string u = muchine[num - 1].u;
		string domain = "";
		for (int i = 0; i < u.size(); i++) {
			if (u[i] == '/') {
				break;
			}
			domain += u[i];
		}
		int now_idx = find_idx(domain);
		run[now_idx] = 0;
		finish[now_idx] = { start,t - start };
		muchine[num - 1].start = 0;
		muchine[num - 1].u = "";
		run_cnt--;
		muchine_queue.push(num - 1);
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);

	setbuf(stdout, NULL);
	// freopen("input(2).txt", "r", stdin);

	int Q;
	cin >> Q;
	for (int i = 1; i <= Q; i++) {
		int cmd;
		cin >> cmd;
		if (cmd == 100) {
			string u;
			cin >> N >> u;
			init(N, u);
		}
		else if (cmd == 200) {
			int t, p;
			string u;
			cin >> t >> p >> u;
			request(t, p, u);
		}
		else if (cmd == 300) {
			int t;
			cin >> t;
			attempt(t);
		}
		else if(cmd == 400) {
			int t, num;
			cin >> t >> num;
			complete(t, num);
		}
		else {
			int t;
			cin >> t;
			cout << result << "\n";
		}
	}
	return 0;
}