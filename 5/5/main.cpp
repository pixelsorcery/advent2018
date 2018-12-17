#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <limits>
#include <vector>

using namespace std;

int main()
{
	string in;
	cin >> in;

#if a
	stack<char> s;

	for (int i = 0; i < in.size(); i++)
	{
		if (s.empty() == false &&
			abs(s.top() - in[i]) == 32)
		{
			s.pop();
		}
		else
		{
			s.push(in[i]);
		}
	}

	cout << s.size() << endl;
#else
	int smallest = numeric_limits<int>::max();
	for (char c = 'a'; c <= 'z'; c++)
	{
		vector<char> s = {};
		string n = "";
		for (int i = 0; i < in.size(); i++)
		{
			if (tolower(in[i]) == c) continue;

			if (s.empty() == false &&
				abs(s.back() - in[i]) == 32)
			{
				s.pop_back();
			}
			else
			{
				s.push_back(in[i]);
			}
		}

		if (s.size() < smallest)
		{
			smallest = s.size();
		}
	}
	cout << smallest;
#endif
	return 0;
}
