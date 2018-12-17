#include <iostream>
#include <string>

using namespace std;

int** table;
int* ids;

int main()
{
	string junk;
	char cjunk;
	int x;
	int y;
	int w;
	int h;
	int id;
	int answer = 0;

	table = new int*[1000];
	for (int i = 0; i < 1000; i++)
	{
		table[i] = new int[1000];
	}

	ids = new int[1000];

	for (int i = 0; i < 1000; i++)
	{
		memset(table[i], 0, sizeof(int) * 1000);
	}
	memset(ids, 0, sizeof(int) * 1000);

	while (cin >> cjunk)
	{
		cin >> id;
		cin >> cjunk;
		cin >> x;
		cin >> cjunk;
		cin >> y;
		cin >> junk;
		cin >> w;
		cin >> cjunk;
		cin >> h;

		for (int i = y; i < y + h; i++)
		{
			for (int j = x; j < x + w; j++)
			{
#if A
				table[i][j]++;
#else
				if (table[i][j] != 0)
				{
					ids[table[i][j]] = 1;
					ids[id] = 1;
				}
				table[i][j] = id;
#endif
			}
		}
	}

#if A
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			if (table[i][j] > 1) answer++;
		}
	}
#else
	for (int i = 1; i < 1000; i++)
	{
		if (ids[i] == 0)
		{
			answer = i;
			break;
		}
	}
#endif
	cout << answer << endl;
	return 0;
}