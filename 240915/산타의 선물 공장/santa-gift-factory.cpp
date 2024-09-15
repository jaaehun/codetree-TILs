#include <stdio.h>
#include <unordered_map>

using namespace std;

typedef long long ll;

#define MAX (200000 + 5000)

#define FACTORY (100)
#define UNLOAD (200)
#define DELETE (300)
#define CHECK (400)
#define BROKEN (500)

#define UNLOAD_BOX (600) // 이름 변경 
#define BROKEN_BELT (700)
#define EMPTY_BELT (-1)
#define CHECK_DELETE (-2)

int T;
int Q, COMMAND, N, M;
int ID[MAX];
int W[MAX];

typedef struct st1
{
	int weight;
	int unload;
}BOXINFO;

typedef struct st2
{
	int id;
	int weight;
	int unload;
	int prev;
	int next;
}BELT;

BOXINFO BOX[MAX];
BELT HEAD[10 + 5];
BELT TAIL[10 + 5];
BELT NODE[MAX];
int nIdx;

int checkBelt[10 + 5];

unordered_map<int, int> boxMap;
unordered_map<int, int> beltMap;

void printBelt(int beltNum)
{
	int index = HEAD[beltNum].next;

	printf("belt %d : ", beltNum);
	while (1)
	{
		if (index == -1) break;

		BELT tmp = NODE[index];

		printf("(%d : %d), ", tmp.id, tmp.weight);

		index = tmp.next;
	}

	printf("TAIL %d %d", NODE[TAIL[beltNum].prev].id, NODE[TAIL[beltNum].prev].weight);

	putchar('\n');
}

void printAll()
{
	for (int m = 1; m <= M; m++)
	{
		if (checkBelt[m] == BROKEN_BELT) continue;
		printBelt(m);
	}
	putchar('\n');
}

void changeBelt(int beltNum, int changeNum)
{
	int index = HEAD[beltNum].next;

	while (1)
	{
		if (index == -1) break;

		BELT tmp = NODE[index];
		beltMap[tmp.id] = changeNum;
		index = tmp.next;
	}
}

void input()
{
	scanf("%d %d", &N, &M);

	boxMap.clear();
	beltMap.clear();

	for (int m = 1; m <= M; m++) checkBelt[m] = 0;

	for (int n = 0; n < N; n++) scanf("%d", &ID[n]);
	for (int n = 0; n < N; n++) scanf("%d", &W[n]);

	for (int n = 0; n < N; n++)
	{
		boxMap[ID[n]] = n;
		BOX[n].unload = 0;
		BOX[n].weight = W[n];
	}

	nIdx = 0;

	int	index = 0;
	int count = N / M;
	for (int m = 1; m <= M; m++)
	{
		HEAD[m].prev = -1;
		TAIL[m].next = -1;

		int startIndex = index;
		for (int i = 0; i < count; i++)
		{
			NODE[nIdx].id = ID[index];
			NODE[nIdx].weight = W[index];
			NODE[nIdx].unload = 0;
			NODE[nIdx].prev = index - 1;
			NODE[nIdx++].next = index + 1;

			beltMap[ID[index++]] = m; // box의 belt 위치
		}

		NODE[startIndex].prev = -1;
		NODE[nIdx - 1].next = -1; // 마지막 node는 Tail로 설정
		HEAD[m].next = startIndex;
		TAIL[m].prev = index - 1;
	}
}

void unload(int w_max)
{
	ll totalWeight = 0;
	for (int m = 1; m <= M; m++)
	{
		int firstBoxIndex = HEAD[m].next;
		if (firstBoxIndex == -1 || checkBelt[m] == BROKEN_BELT) continue;

		int frontNextIndex = NODE[firstBoxIndex].next;

		HEAD[m].next = frontNextIndex;
		if (frontNextIndex == -1) TAIL[m].prev = -1;
		else NODE[frontNextIndex].prev = -1;

		int weight = NODE[firstBoxIndex].weight;
		if (weight <= w_max)
		{
			NODE[firstBoxIndex].unload = UNLOAD;
			totalWeight += weight;
		}
		else
		{
			int lastBoxIndex = TAIL[m].prev;
			if (lastBoxIndex == -1) // 상자가 1개인 경우.
			{
				HEAD[m].next = firstBoxIndex;
				NODE[firstBoxIndex].prev = -1;
				NODE[firstBoxIndex].next = -1;
				TAIL[m].prev = firstBoxIndex;
			}
			else
			{
				NODE[lastBoxIndex].next = firstBoxIndex;
				NODE[firstBoxIndex].prev = lastBoxIndex;
				NODE[firstBoxIndex].next = -1;
				TAIL[m].prev = firstBoxIndex;
			}
		}
	}

	printf("%lld\n", totalWeight);
}

void deleteBox(int r_id)
{
	if (boxMap.count(r_id) == 0)
	{
		printf("%d\n", -1);
		return;
	}

	int boxIndex = boxMap[r_id];
	if (NODE[boxIndex].unload == UNLOAD)
	{
		printf("%d\n", -1);
		return;
	}

	int beltNum = beltMap[r_id];

	int prevBoxIndex = NODE[boxIndex].prev;
	int nextBoxIndex = NODE[boxIndex].next;

	if (prevBoxIndex != -1) NODE[prevBoxIndex].next = nextBoxIndex;
	else HEAD[beltNum].next = nextBoxIndex;

	if (nextBoxIndex != -1) NODE[nextBoxIndex].prev = prevBoxIndex;
	else TAIL[beltNum].prev = prevBoxIndex;

	NODE[boxIndex].unload = UNLOAD;

	printf("%d\n", r_id);
}

void checkBox(int f_id)
{
	if (boxMap.count(f_id) == 0)
	{
		printf("%d\n", -1);
		return;
	}

	int boxIndex = boxMap[f_id];
	if (NODE[boxIndex].unload == UNLOAD)
	{
		printf("%d\n", -1);
		return;
	}

	int beltNum = beltMap[f_id];
	int prevBoxIndex = NODE[boxIndex].prev;  // 선택된 상자의 바로 앞 상자
	if (prevBoxIndex == -1) // 가장 앞에 있는 경우는 옮길 필요가 없다.
	{
		printf("%d\n", beltNum);
		return;
	}

	int firstBoxIndex = HEAD[beltNum].next; // 가장 앞의 상자
	int lastBoxIndex = TAIL[beltNum].prev; // 가장 뒤의 상자

	HEAD[beltNum].next = boxIndex;
	NODE[boxIndex].prev = -1;

	NODE[firstBoxIndex].prev = lastBoxIndex;
	NODE[prevBoxIndex].next = -1;

	NODE[lastBoxIndex].next = firstBoxIndex;
	TAIL[beltNum].prev = prevBoxIndex;

	printf("%d\n", beltNum);
}

void brokenBelt(int b_num)
{
	if (checkBelt[b_num] == BROKEN_BELT)
	{
		printf("%d\n", -1);
		return;
	}

	checkBelt[b_num] = BROKEN_BELT;

	int nextBeltNum = -1;
	for (int m = b_num + 1; m <= M; m++)
	{
		if (checkBelt[m] == BROKEN_BELT) continue;
		else
		{
			nextBeltNum = m;
			break;
		}
	}

	if (nextBeltNum == -1) // 고장난 벨트 오른쪽에 정상 벨트가 없는 경우
	{
		for (int m = 1; m < b_num; m++)
		{
			if (checkBelt[m] == BROKEN_BELT) continue;
			else
			{
				nextBeltNum = m;
				break;
			}
		}
	}

	int brokenFirstBoxIndex = HEAD[b_num].next; // 고장난 벨트의 앞 상자
	int brokenLastBoxIndex = TAIL[b_num].prev; // 고장난 벨트의 뒤 상자
	int lastBoxIndex = TAIL[nextBeltNum].prev; // 옮길 벨트의 뒤 상자

	if (brokenFirstBoxIndex == -1) // 고장난 벨트가 빈 경우
	{
		printf("%d\n", b_num);
		return;
	}

	// 벨트 변경
	changeBelt(b_num, nextBeltNum);

	if (lastBoxIndex == -1) // 옮길 벨트가 빈 경우
	{
		HEAD[nextBeltNum].next = brokenFirstBoxIndex;
		TAIL[nextBeltNum].prev = brokenLastBoxIndex;
	}
	else
	{
		NODE[lastBoxIndex].next = brokenFirstBoxIndex;
		NODE[brokenFirstBoxIndex].prev = lastBoxIndex;

		NODE[brokenLastBoxIndex].next = -1;
		TAIL[nextBeltNum].prev = brokenLastBoxIndex;
	}

	printf("%d\n", b_num);
}

int main(void)
{
	scanf("%d", &Q);
	for (int q = 0; q < Q; q++)
	{
		scanf("%d", &COMMAND);

		if (COMMAND == FACTORY) input();
		else if (COMMAND == UNLOAD)
		{
			int w_max;
			scanf("%d", &w_max);
			unload(w_max);
		}
		else if (COMMAND == DELETE)
		{
			int r_id;
			scanf("%d", &r_id);
			deleteBox(r_id);
		}
		else if (COMMAND == CHECK)
		{
			int f_id;
			scanf("%d", &f_id);
			checkBox(f_id);
		}
		else if (COMMAND == BROKEN)
		{
			int b_num;
			scanf("%d", &b_num);
			brokenBelt(b_num);
		}
	}

	return 0;
}