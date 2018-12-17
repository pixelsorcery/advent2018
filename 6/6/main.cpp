#include <iostream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

pair<int, int> coords[50];
int grid[400][400];
int infinite[50];

int main()
{
	int i = 0;
	int mx = 0, my = 0;
	while (cin >> coords[i].first)
	{
		char j;
		cin >> j;
		cin >> coords[i].second;
		if (coords[i].first > mx) mx = coords[i].first;
		if (coords[i].second > my) my = coords[i].second;
		i++;
	}

#if a
	for (int i = 0; i <= mx; i++)
	{
		for (int j = 0; j <= my; j++)
		{
			int smallest = numeric_limits<int>::max();
			for (int k = 0; k < 50; k++)
			{
				int dk = abs(i - coords[k].first) + abs(j - coords[k].second);
				if (smallest > dk)
				{
					smallest = dk;
					grid[i][j] = k;
				}
				else if (smallest == dk)
				{
					grid[i][j] = -1;
					continue;
				}
			}

			// mark infinite ones
			if (i == 0 || j == 0 || i == mx - 1 || j == my - 1)
			{
				infinite[grid[i][j]] = 1;
			}
		}
	}

	int answer = 0;
	for (int k = 0; k < 50; k++)
	{
		if (infinite[k]) continue;
		int highest = 0;
		for (int i = 0; i <= mx; i++)
		{
			for (int j = 0; j <= my; j++)
			{
				if (grid[i][j] == k)
				{
					highest++;
				}
			}
		}

		if (highest > answer)
		{
			answer = highest;
		}
	}

	cout << answer << endl;
#else
	for (int i = 0; i <= mx; i++)
	{
		for (int j = 0; j <= my; j++)
		{
			int smallest = numeric_limits<int>::max();
			for (int k = 0; k < 50; k++)
			{
				int dk = abs(i - coords[k].first) + abs(j - coords[k].second);
				grid[i][j] += dk;
			}
		}
	}

	int answer = 0;
	for (int i = 0; i <= mx; i++)
	{
		for (int j = 0; j <= my; j++)
		{
			if (grid[i][j] < 10000)
			{
				answer++;
			}
		}
	}

	cout << answer << endl;
#endif
	return 0;
}