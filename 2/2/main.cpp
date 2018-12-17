#include <iostream>
#include <string>
#include <unordered_map>

#define B 1

using namespace std;

int main()
{
	string in;
	int two = 0;
	int three = 0;
	string strings[1024];
	int size = 0;
	unordered_map<string, int> map;
	string answer = "";
#if B
	while (cin >> strings[size])
	{
		size++;
	}

	for (int i = 0; i < strings[0].size() && answer == ""; i++)
	{
		unordered_map<string, int> map = {};
		for (int j = 0; j < size; j++)
		{
			string tmp = "";
			for (int k = 0; k < strings[0].size(); k++)
			{
				if (k != i)
				{
					tmp += strings[j][k];
				}
				else
				{
					tmp += ".";
				}
			}

			if (map.find(tmp) != map.end())
			{
				answer = tmp;
				break;
			}
			else
			{
				map[tmp] = 1;
			}
		}
	}

	cout << answer << endl;
#endif 

#if A
	while (cin >> in)
	{
		unsigned char map[26];
		memset(map, 0, sizeof(char) * 26);
		for (unsigned int i = 0; i < in.size(); i++)
		{
			map[in[i] - 'a']++;
		}

		for (int i = 0; i < 26; i++)
		{
			if (map[i] == 2)
			{
				two++;
				break;
			}
		}
		for (int i = 0; i < 26; i++)
		{
			if (map[i] == 3)
			{
				three++;
				break;
			}
		}
	}

	cout << two * three << endl;
#endif
	return 0;
}