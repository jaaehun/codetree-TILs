#include <iostream>
#include <unordered_map>
#include <vector>

#define MAX_BELT 15
#define MAX_BOX 100010

using namespace std;

struct BELT {
	int num;
	int front;
	int back;
	int state;
};

struct BOX {
	int weight;
	int belt_id;
	int prev;
	int next;
};

int n, m;
int input[MAX_BOX];
unordered_map<int, BOX> box;
unordered_map<int, BELT> belt;

void FUNC(int Q)
{
	if (Q == 100)
	{
		cin >> n >> m;

		int cnt = 0;
		int belt_id = 1;
		int pivot = n/m;
		for (int i = 0; i < n; i++)
		{
			int id;
			cin >> id;
			// BOX ID의 input 배열
			input[i] = id;
			// belt 해쉬맵 만들기
			if (cnt == 0)
			{
				belt[belt_id].front = id;
				belt[belt_id].back = id;
				belt[belt_id].state = 0;
				belt[belt_id].num = pivot;

				box[id].belt_id = belt_id;
				box[id].prev = -1;
				box[id].next = -1;
			}
			else if (cnt == pivot - 1)
			{
				belt[belt_id].back = id;
				
				box[id].belt_id = belt_id;
				box[id].prev = input[i - 1];
				box[id].next = -1;
				box[input[i - 1]].next = id;
			}
			else {
				box[id].belt_id = belt_id;
				box[id].prev = input[i - 1];
				box[input[i - 1]].next = id;
			}
			cnt++;
			if (cnt == pivot)
			{
				cnt = 0;
				belt_id += 1;
			}
		}
		for (int i = 0; i < n; i++)
		{
			int w = 0;
			cin >> w;
			box[input[i]].weight = w;
		}
	}
	else if (Q == 200)
	{
		int w_max;
		cin >> w_max;
		int cnt = 0;
		// m개 벨트 탐색
		for (int i = 1; i <= m; i++)
		{
			if (belt[i].num == 0)
				continue;
			
			int id = belt[i].front;
			// 하차
			if (box[id].weight <= w_max)
			{
				cnt += box[id].weight;
				// box가 1개인 경우
				if (belt[i].num == 1)
				{
					belt[i].front = -1;
					belt[i].back = -1;

					box[id] = { 0, 0, -1, -1 };
				}
				// box가 여러개인 경우
				else
				{
					int next = box[id].next;
					box[id] = { 0, 0, -1, -1 };
					box[next].prev = -1;

					belt[i].front = next;
				}
				belt[i].num -= 1;
			}
			else
			{
				// box가 1개인 경우
				if (belt[i].num == 1) continue;
				// box가 여러개인 경우
				int next = box[id].next;
				int back = belt[i].back;

				box[id].prev = back;
				box[id].next = -1;

				box[next].prev = -1;
				box[back].next = id;

				belt[i].front = next;
				belt[i].back = id;
			}
		}
		cout << cnt << endl;
	}
	else if (Q == 300)
	{
		int r_id;
		cin >> r_id;
		// box가 있으면
		if (box[r_id].weight != 0)
		{
			int belt_id = box[r_id].belt_id;
			int prev = box[r_id].prev;
			int next = box[r_id].next;
			belt[belt_id].num -= 1;

			// 벨트 위의 유일한 박스이면
			if (prev == -1 && next == -1)
			{
				belt[belt_id].front = -1;
				belt[belt_id].back = -1;
			}
			// 맨 앞이면
			else if (prev == -1)
			{
				belt[belt_id].front = next;
				box[next].prev = -1;
			}
			// 맨 뒤면
			else if (next == -1)
			{
				belt[belt_id].back = prev;
				box[prev].next = -1;
			}
			else
			{
				box[prev].next = next;
				box[next].prev = prev;
			}
			box[r_id] = { 0, 0, -1, -1 };
			cout << r_id << endl;
		}
		// box가 없으면
		else 
		{
			box[r_id] = { 0, 0, -1, -1 };
			cout << -1 << endl;
		}
	}
	else if (Q == 400)
	{
		int f_id;
		cin >> f_id;
		// box가 없다면
		if (box[f_id].weight == 0)
		{
			cout << -1 << endl;
		}
		// box가 있다면
		else
		{
			int prev = box[f_id].prev;
			int next = box[f_id].next;
			int belt_id = box[f_id].belt_id;
			int front = belt[belt_id].front;
			int back = belt[belt_id].back;
			
			cout << belt_id << endl;
			if (belt[belt_id].num == 1) return;
			if (prev == -1) return ;
			else if (next == -1)
			{
				belt[belt_id].front = f_id;
				belt[belt_id].back = prev;

				box[f_id].prev = -1;
				box[prev].next = -1;

				box[front].prev = f_id;
				box[f_id].next = front;
			}
			else
			{
				belt[belt_id].front = f_id;
				belt[belt_id].back = prev;

				box[f_id].prev = -1;
				box[prev].next = -1;

				box[back].next = front;
				box[front].prev = back;
			}
		}

	}
	else if (Q == 500)
	{
		int b_num;
		cin >> b_num;
		// 벨트가 이미 고장
		if (belt[b_num].state == -1)
		{
			cout << -1 << endl;
			return;
		}
		else cout << b_num << endl;
		
		// 고장 벨트에 박스가 0개
		if (belt[b_num].num == 0)
		{
			// 사용할 수 없게 됨
			belt[b_num] = { 0, -1, -1, -1 };
			return;
		}
		// 정상 벨트 조회
		int flag = 0;
		for (int i = b_num + 1 ; i <= m; i++)
		{
			if (belt[i].state == -1) continue;
			// 정상 벨트 찾음
			flag = 1;
			belt[i].num += belt[b_num].num;
			// 정상 벨트에 박스가 0개
			if (belt[i].num == 0)
			{
				belt[i].front = belt[b_num].front;
				belt[i].back = belt[b_num].back;
				// 박스이동
				int k = belt[b_num].front;
				while (box[k].next != -1)
				{
					box[k].belt_id = i;
					k = box[k].next;
				}
				box[k].belt_id = i;
			}
			else {
				int from_front = belt[b_num].front;
				int to_back = belt[i].back;
				
				box[to_back].next = from_front;
				box[from_front].prev = to_back;
				
				belt[i].back = belt[b_num].back;
				// 박스이동
				while (box[from_front].next != -1)
				{
					box[from_front].belt_id = i;
					from_front = box[from_front].next;
				}
				box[from_front].belt_id = i;
				belt[i].back = from_front;
			}
			// belt[i].num += belt[b_num].num;
			// 사용할 수 없게 됨
			belt[b_num] = { 0, -1, -1, -1 };
			if (flag != 0) break;
		}
		// 못찾은 경우 1번부터 다시 조회
		if (flag == 0)
		{
			for (int i = 1; i < b_num; i++)
			{
				if (belt[i].state == -1) continue;
				flag = 1;
				// 정상 벨트에 박스가 0개
				if (belt[i].num == 0)
				{
					belt[i].front = belt[b_num].front;
					belt[i].back = belt[b_num].back;
					// 박스이동
					int k = belt[b_num].front;
					while (box[k].next != -1)
					{
						box[k].belt_id = i;
						k = box[k].next;
					}
					box[k].belt_id = i;
				}
				else {
					int from_front = belt[b_num].front;
					int to_back = belt[i].back;

					box[to_back].next = from_front;
					box[from_front].prev = to_back;

					belt[i].back = belt[b_num].back;
					// 박스이동
					while (box[from_front].next != -1)
					{
						box[from_front].belt_id = i;
						from_front = box[from_front].next;
					}
					box[from_front].belt_id = i;
					belt[i].back = from_front;
				}
				belt[i].num += belt[b_num].num;
				// 사용할 수 없게 됨
				belt[b_num] = { 0, -1, -1, -1 };
				if (flag != 0) break;
			}
		}
		
	}
}

int main()
{
	int Q;
	cin >> Q;
	for (int i = 0; i < Q; i++)
	{
		int order;
		cin >> order;
		FUNC(order);
	}

	return 0;
}