#include <iostream>
#include <memory>
#include <algorithm>

using namespace std;

constexpr int numPlayers = 419;
#if a
constexpr int lastPts = 72164;
#else
constexpr int lastPts = 7216400;
#endif

struct node
{
	int val;
	node* pNext;
	node* pPrev;
};
int main()
{
	// alloc all nodes ahead of time
	unique_ptr<node[]> vals = make_unique<node[]>(lastPts+1);
	unique_ptr<uint64_t[]> scores = make_unique<uint64_t[]>(numPlayers);
	memset(vals.get(), 0, sizeof(node) * (lastPts + 1));
	memset(scores.get(), 0, sizeof(uint64_t) * (numPlayers));

	// avoid base case by inserting one node here
	node* head = &vals[0];
	head->val = 0;
	head->pNext = head;
	head->pPrev = head;
	node* pCur = head;

	int curElve = 1;
	for (int i = 1; i <= lastPts; i++)
	{
		if (i % 23 == 0)
		{
			// walk back 7 nodes
			for (int j = 0; j < 7; j++)
			{
				pCur = pCur->pPrev;
			}
			// remove node
			pCur->pPrev->pNext = pCur->pNext;
			pCur->pNext->pPrev = pCur->pPrev;
			scores[curElve] += pCur->val + i;
			pCur = pCur->pNext;
		}
		else
		{
			// walk over 1 node from current and insert
			pCur = pCur->pNext;
			// save next node
			node* pTemp = pCur->pNext;
			// 1. make next node a new node
			pCur->pNext = &vals[i];
			// 2. set new node's next node
			pCur->pNext->pNext = pTemp;
			// 3. set pTemp's prev node to new node
			pTemp->pPrev = pCur->pNext;
			// 4. set previous node of new node to pCur
			pCur->pNext->pPrev = pCur;

			// switch to new node
			pCur = pCur->pNext;
			
			// set values
			pCur->val = i;
		}
		curElve = ((curElve + 1) % numPlayers);
	}

	auto highScore = max_element(&scores[0], &scores[numPlayers]);
	cout << *highScore << endl;

	return 0;
}