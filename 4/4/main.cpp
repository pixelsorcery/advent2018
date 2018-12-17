#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <unordered_map>

using namespace std;

vector<string> lines;
int minutes[60];
unordered_map<int, int[60]> sleep_times;

int main()
{
	string line;
	while (getline(cin,line))
	{
		lines.push_back(line);
	}

	sort(lines.begin(), lines.end());

	regex guard("\\[\\d+-\\d+-\\d+ \\d+:\\d+\\] Guard #(\\d+) begins shift");
	regex sleep("\\[\\d+-\\d+-\\d+ \\d+:(\\d+)\\] falls asleep");
	regex wake("\\[\\d+-\\d+-\\d+ \\d+:(\\d+)\\] wakes up");
	smatch match;

	int gnum;
	int start_min;
	int end_min;
	for (string s : lines)
	{
		if (regex_match(s, match, guard))
		{
			gnum = stoi(match[1]);
			//cout << gnum << endl;
		}
		else if (regex_match(s, match, wake))
		{
			end_min = stoi(match[1]);
			//cout << end_min << endl;
			for (int i = start_min; i < end_min; i++)
			{
				sleep_times[gnum][i]++;
			}
		}
		else if (regex_match(s, match, sleep))
		{
			start_min = stoi(match[1]);
			//cout << start_min << endl;
		}
		//cout << s << endl;
	}
#if a
	int highestg = 0;
	int highestm = 0;
	for (auto key : sleep_times)
	{
		int mins = 0;
		for (int i = 0; i< 60; i++)
		{
			mins += key.second[i];
		}
		if (mins > highestm)
		{
			highestm = mins;
			highestg = key.first;
		}
	}

	int highestt = 0;
	highestm = 0;
	for (int i = 0; i < 60; i++)
	{
		if (sleep_times[highestg][i] > highestt)
		{
			highestt = sleep_times[highestg][i];
			highestm = i;
		}
	}

	cout << highestg * highestm << endl;
#else
	int highestg = 0;
	int highestt = 0;
	int highestm = 0;

	for (auto key : sleep_times)
	{
		for (int i = 0; i < 60; i++)
		{
			if (key.second[i] > highestt)
			{
				highestt = key.second[i];
				highestm = i;
				highestg = key.first;
			}
		}
	}

	cout << highestm * highestg;
#endif
	return 0;
}