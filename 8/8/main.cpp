#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cassert>
#include <numeric>

using namespace std;
using id = int;
vector<int> input;
vector<int> l; // num leafs tracker
vector<pair<id, int>> m; // id and metadata
int answer = 0;
unordered_map<id, vector<int>> id2leafs;
unordered_map<id, vector<int>> id2metadata;
vector<int> node_stack;
int answer2 = 0;

int main()
{
	string in;
	getline(cin, in);
	stringstream ss(in);
	id nodeId = 0;
	int i;
	while (ss >> i)
	{
		input.push_back(i);
	}

	l.push_back(input[0]);
	m.push_back(make_pair(nodeId, input[1]));
	nodeId++;
	i = 2;
	while(l.size() > 0)
	{
		if (l.back() == 0)
		{
			// read metadata
			int metadata = 0;
			for (int j = 0; j < m.back().second; j++, i++)
			{
				metadata += input[i];
				id2metadata[m.back().first].push_back(input[i]);
			}
				
			// pop off top
			id val = m.back().first;
			l.pop_back();
			m.pop_back();

			// subtract 1 from child node
			if (l.size() > 0)
			{
				id2leafs[m.back().first].push_back(val);
				l.back() -= 1;
			}
		}
		else
		{
			l.push_back(input[i]);
			i++;
			if (input[i] > 0)
			{
				assert(i < input.size());
				m.push_back(make_pair(nodeId, input[i]));
				i++;
				nodeId++;
			}
		}
	}

	// part 1
	cout << answer << endl;

	// part 2
	// root id is 0 so throw in stack
	node_stack.push_back(0);
	while (node_stack.size() > 0)
	{
		id cur = node_stack.back();
		node_stack.pop_back();
		auto leaves = id2leafs[cur];
		auto metadata = id2metadata[cur];

		// if no children, sum up values
		if (leaves.size() == 0)
		{
			answer2 += accumulate(metadata.begin(), metadata.end(), 0);
		}
		// else add referenced node index to stack
		else
		{
			for (int i = 0; i < metadata.size(); i++)
			{
				if (metadata[i] - 1 < leaves.size())
				{
					node_stack.push_back(leaves[metadata[i] - 1]);
				}
			}
		}
	}
	cout << answer2 << endl;
	return 0;
}