#include<iostream>

using namespace std;

struct BELT {
	int front;
	int end;
	int size;
};

struct BOX {
	int prev;
	int next;
	int belt_n;
};

BELT belt[100001];
BOX box[100001];

void init(int n, int m) {
	for (int i = 1; i <= n; i++) {
		belt[i].front = -1;
		belt[i].end = -1;
		belt[i].size = 0;
	}

	for (int i = 1; i <= m; i++) {
		int belt_n;
		cin >> belt_n;

		if (belt[belt_n].front == -1) {
			box[i].prev = -1;
			box[i].next = -1;
			box[i].belt_n = belt_n;
			belt[belt_n].front = i;
			belt[belt_n].end = i;
			belt[belt_n].size++;
		}
		else {
			box[i].prev = belt[belt_n].end;
			box[i].next = -1;
			box[i].belt_n = belt_n;
			box[belt[belt_n].end].next = i;
			belt[belt_n].end = i;
			belt[belt_n].size++;
		}
	}
}

int move(int src, int dst) {
	if(belt[src].size != 0) {
		if (belt[dst].size == 0) {
			int now_n = belt[src].front;
			while (true) {
				box[now_n].belt_n = dst;
				now_n = box[now_n].next;
				if (now_n == -1) {
					break;
				}
			}
			belt[dst].front = belt[src].front;
			belt[dst].end = belt[src].end;
			belt[dst].size += belt[src].size;

			belt[src].front = -1;
			belt[src].end = -1;
			belt[src].size = 0;
		}
		else {
			box[belt[dst].front].prev = belt[src].end;
			box[belt[src].end].next = belt[dst].front;
			int now_n = belt[src].front;
			while (true) {
				box[now_n].belt_n = dst;
				now_n = box[now_n].next;
				if (now_n == -1) {
					break;
				}
			}
			belt[dst].front = belt[src].front;
			belt[dst].size += belt[src].size;

			belt[src].front = -1;
			belt[src].end = -1;
			belt[src].size = 0;
		}
	}

	return belt[dst].size;
}

void change_zero(int a, int b) {
	int now_n = belt[b].front;
	int next_n = box[now_n].next;

	if (next_n == -1) {
		box[now_n].belt_n = a;
		belt[a].front = now_n;
		belt[a].end = now_n;
		belt[a].size++;
		belt[b].front = -1;
		belt[b].end = -1;
		belt[b].size--;
	}
	else {
		box[now_n].next = -1;
		box[now_n].belt_n = a;
		belt[a].front = now_n;
		belt[a].end = now_n;
		belt[a].size++;
		box[next_n].prev = -1;
		belt[b].front = next_n;
		belt[b].size--;
	}
}

void change_nonzero(int a, int b) {
	int a_now = belt[a].front;
	int a_next = box[a_now].next;
	int b_now = belt[b].front;
	int b_next = box[b_now].next;

	box[b_now].next = -1;
	box[b_now].belt_n = a;
	belt[a].front = b_now;
	belt[a].end = b_now;

	box[a_now].next = b_next;
	box[a_now].belt_n = b;
	belt[b].front = a_now;
}

int change(int src, int dst) {
	if (belt[src].size == 0 && belt[dst].size == 0) {
		return 0;
	}
	else {
		if (belt[src].size == 0 || belt[dst].size == 0) {
			if (belt[src].size == 0) {
				change_zero(src, dst);
			}
			else {
				change_zero(dst, src);
			}
		}
		else {
			int src_now = belt[src].front;
			int src_next = box[src_now].next;
			int dst_now = belt[dst].front;
			int dst_next = box[dst_now].next;

			if (src_next == -1 && dst_next == -1) {
				box[dst_now].belt_n = src;
				belt[src].front = dst_now;
				belt[src].end = dst_now;

				box[src_now].belt_n = dst;
				belt[dst].front = src_now;
				belt[dst].end = src_now;
			}
			else {
				if (src_next == -1 || dst_next == -1) {
					if (src_next == -1) {
						change_nonzero(src, dst);
					}
					else {
						change_nonzero(dst, src);
					}
				}
				else {
					box[dst_now].belt_n = src;
					box[dst_now].next = src_next;
					box[src_next].prev = dst_now;
					belt[src].front = dst_now;

					box[src_now].belt_n = dst;
					box[src_now].next = dst_next;
					box[dst_next].prev = src_now;
					belt[dst].front = src_now;
				}
			}
		}

		return belt[dst].size;
	}
}

int divide(int src, int dst) {
	if (belt[src].size > 1) {
		int size = belt[src].size;
		int floor = size / 2;

		int now_n = belt[src].front;
		int dst_front = belt[dst].front;
		belt[dst].front = now_n;
		for (int i = 0; i < floor; i++) {
			box[now_n].belt_n = dst;
			now_n = box[now_n].next;
		}
		belt[src].front = now_n;
		box[now_n].prev = -1;
		now_n = box[now_n].prev;
		if (dst_front == -1) {
			box[now_n].next = -1;
			belt[dst].end = now_n;
		}
		else {
			box[now_n].next = dst_front;
			box[dst_front].prev = now_n;
		}
		
		belt[src].size -= floor;
		belt[dst].size += floor;
	}

	return belt[dst].size;
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
			int m_src, m_dst;
			cin >> m_src >> m_dst;
			int result = move(m_src, m_dst);
			cout << result << "\n";
		}
		else if (cmd == 300) {
			int m_src, m_dst;
			cin >> m_src >> m_dst;
			int result = change(m_src, m_dst);
			cout << result << "\n";
		}
		else if (cmd == 400) {
			int m_src, m_dst;
			cin >> m_src >> m_dst;
			int result = divide(m_src, m_dst);
			cout << result << "\n";
		}
		else if (cmd == 500) {
			int p_num;
			cin >> p_num;
			int a = box[p_num].prev;
			int b = box[p_num].next;
			cout << a + 2 * b << "\n";
		}
		else {
			int b_num;
			cin >> b_num;
			int a = belt[b_num].front;
			int b = belt[b_num].end;
			int c = belt[b_num].size;
			cout << a + 2 * b + 3 * c << "\n";
		}
	}
	return 0;
}