#include <iostream>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <deque>
#include <vector>

using namespace std;

unordered_map<char, vector<char>> m;
int isLeaf[26];
int isParent[26];
int assembled[26];
vector<char> order;
int delta = 60;
static const int num_elves = 5;
pair<int, char> elves[num_elves];
int pushed[26];

int main()
{
	string l;
	while (getline(cin, l))
	{
        if (l.size() == 0) break;
		m[l[36]].push_back(l[5]);
		isParent[l[36] - 'A'] = 1;
		isLeaf[l[5] - 'A'] = 1;
		if (m.find(l[5]) == m.end())
		{
			m[l[5]] = {};
		}
	}

	// start with bottom of tree
	deque<char> steps;
	for (int i = 0; i < 26; i++)
	{
		if (isParent[i] == 0 && isLeaf[i] == 1)
		{
			steps.push_back(i + 'A');
			pushed[i] = 1;
		}
	}

	while (steps.size() > 0)
	{
		sort(steps.rbegin(), steps.rend());
		auto n = steps.back();
		steps.pop_back();
		order.push_back(n);
		assembled[n - 'A'] = 1;

		for (auto key : m)
		{
			bool add = true;
			for (unsigned int i = 0; i < key.second.size(); i++)
			{
				if (assembled[key.second[i] - 'A'] == 0)
				{
					add = false;
					break;
				}
			}
			if (add == true && key.second.size() > 0 && pushed[key.first - 'A'] == 0)
			{
				steps.push_back(key.first);
				pushed[key.first - 'A'] = 1;
			}
		}
	}

	for (char c : order)
	{
		cout << c;
	}
	cout << endl;
	// BFLNGIRUSJXEHKQPVTYOCZDWMA

    memset(pushed, 0, sizeof(int) * 26);
    memset(assembled, 0, sizeof(int) * 26);
    int time = 0;
    for (int i = 0; i < num_elves; i++)
    {
        elves[i] = { make_pair(-1, 0) };
    }

    while ([]() { for (pair<int, char> e : elves) { if (e.first > 0 || e.first < 0) { return true; } } return false; }())
    {
        for (pair<int, char>& e : elves)
        {
            if (e.first > 0)
            {
                e.first--;
            }
            if (e.first == 0)
            {
                assembled[e.second - 'A'] = 1;
            }
        }

        for (pair<int, char>& e : elves)
        {
            if (e.first <= 0)
            {
                vector<char> available = {};
                for (auto key : m)
                {
                    if (pushed[key.first - 'A'] == 0 && [&]() { for (auto v : key.second) {
                            if (assembled[v - 'A'] == 0)
                                return false;
                        }
                        return true; }())
                    {
                        available.push_back(key.first);
                    }
                }

                if (available.size() > 0)
                {
                    sort(available.begin(), available.end());
                    e = make_pair((available[0] - 'A' + 1) + delta, available[0]);
                    pushed[available[0] - 'A'] = 1;
                    m.erase(available[0]);
                }
            }
        }

        time++;
    }

    cout << time - 1 << endl;

	return 0;
}