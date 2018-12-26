/*AMDG*/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <sstream>
#include <deque>

using namespace std;

string makeKey(int x, int y)
{
	stringstream ss;
	ss << x << "_" << y;
	return ss.str();
}
enum RACE
{
	ELF,
	GOBLIN
};
struct dude
{
	RACE r;
	int hp;
	int x;
	int y;
};
vector<dude> dudes;

int numG = 0;
int numE = 0;

char* graph = nullptr;
int width = 0;

struct qNode
{
	int x;
	int y;
	vector<pair<int, int>> path;
};

bool inRange(int x, int y, RACE r, dude** d = nullptr)
{
	bool inRange = false;
	for (int i = 0; i < dudes.size(); i++)
	{
		if (dudes[i].r != r && dudes[i].hp > 0)
		{
			if ((x == dudes[i].x + 1 && y == dudes[i].y) ||
				(x == dudes[i].x - 1 && y == dudes[i].y) ||
				(x == dudes[i].x && y == dudes[i].y + 1) ||
				(x == dudes[i].x && y == dudes[i].y - 1))
			{
				if (d != nullptr)
				{
					// find lowest hp dude
					if (*d == nullptr || (*d)->hp > dudes[i].hp)
					*d = &dudes[i];
				}
				inRange = true;
			}
		}
	}
	return inRange;
}

bool isEmpty(int x, int y)
{
	if (graph[y * width + x] == '#')
		return false;
	for (int i = 0; i < dudes.size(); i++)
	{
		if (dudes[i].hp > 0 && x == dudes[i].x && y == dudes[i].y)
			return false;
	}
	return true;
}

bool canMove()
{
	for (int i = 0; i < dudes.size(); i++)
	{
		if (isEmpty(dudes[i].x + 1, dudes[i].y) == true) return true;
		else if (isEmpty(dudes[i].x - 1, dudes[i].y) == true) return true;
		else if (isEmpty(dudes[i].x, dudes[i].y + 1) == true) return true;
		else if (isEmpty(dudes[i].x, dudes[i].y - 1) == true) return true;
	}
	return false;
}

vector<pair<int, int>> generateNext(int x, int y)
{
	vector<pair<int, int>> moves;
	moves.push_back(make_pair(x, y - 1));
	moves.push_back(make_pair(x - 1, y));
	moves.push_back(make_pair(x + 1, y));
	moves.push_back(make_pair(x, y + 1));

	return moves;
}

// find shortest paths
vector<vector<pair<int, int>>> shortestPaths(dude d)
{
	deque<qNode> q;
	unordered_set<string> visited;
	vector<pair<int, int>> nm = generateNext(d.x, d.y);
	vector<vector<pair<int, int>>> shortestPaths;
	for (int i = 0; i < nm.size(); i++)
	{
		if (isEmpty(nm[i].first, nm[i].second) &&
			inRange(nm[i].first, nm[i].second, d.r) == false &&
			visited.find(makeKey(nm[i].first, nm[i].second)) == visited.end())
		{
			visited.insert(makeKey(nm[i].first, nm[i].second));
			qNode next;
			next.x = nm[i].first;
			next.y = nm[i].second;
			next.path.push_back(make_pair(nm[i].first, nm[i].second));
			q.push_back(next);
		}
		else if (isEmpty(nm[i].first, nm[i].second) && 
				 inRange(nm[i].first, nm[i].second, d.r) == true)
		{
			vector<pair<int,int>> path;
			path.push_back(make_pair(nm[i].first, nm[i].second));
			shortestPaths.push_back(path);
		}
	}
	
	while (q.size() > 0 && shortestPaths.size() == 0)
	{
		int size = q.size();
		for (int i = 0; i < size; i++)
		{
			vector<pair<int, int>> nm = generateNext(q.front().x, q.front().y);
			for (int i = 0; i < nm.size(); i++)
			{
				if (isEmpty(nm[i].first, nm[i].second) &&
					inRange(nm[i].first, nm[i].second, d.r) == false &&
					visited.find(makeKey(nm[i].first, nm[i].second)) == visited.end())
				{
					visited.insert(makeKey(nm[i].first, nm[i].second));
					qNode next;
					next.x = nm[i].first;
					next.y = nm[i].second;
					next.path = q.front().path; // copy path
					next.path.push_back(make_pair(nm[i].first, nm[i].second)); // add new node to it
					q.push_back(next);
				}
				else if (isEmpty(nm[i].first, nm[i].second) && 
					     inRange(nm[i].first, nm[i].second, d.r) == true)
				{
					shortestPaths.push_back(q.front().path);
					shortestPaths.back().push_back(make_pair(nm[i].first, nm[i].second));
				}
			}
			q.pop_front();
		}
	}

	return shortestPaths;
}

int main()
{
	string line;
	int lineNum = 0;
	while (cin >> line)
	{
		if (lineNum == 0) graph = new char[line.size() * line.size()];
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] == 'G')
			{
				dudes.push_back({ GOBLIN, 200, static_cast<int>(i%line.size()), lineNum });
				numG++;
				line[i] = '.';
			}
			else if (line[i] == 'E')
			{
				dudes.push_back({ ELF, 200, static_cast<int>(i%line.size()), lineNum });
				numE++;
				line[i] = '.';
			}
		}
		memcpy(graph + lineNum * line.size(), line.c_str(), line.size());
		lineNum++;
	}
	width = static_cast<int>(line.size());
	int rounds = 0;
	while (numE > 0 && numG > 0)
	{
		sort(dudes.begin(), dudes.end(), [](dude a, dude b) {if (a.y < b.y) return true; else if (a.y > b.y) return false; else if (a.y == b.y) if (a.x < b.x) return true; else return false; return false; });
		for (int i = 0; i < dudes.size(); i++)
		{
			// otherwise check if any spots in range are open
			if (canMove() && 
				inRange(dudes[i].x, dudes[i].y, dudes[i].r) == false)
			{
				vector<vector<pair<int, int>>> paths;
				paths = shortestPaths(dudes[i]);

				if (paths.size() > 0)
				{
					// sort and use the first path
					sort(paths.begin(), paths.end(), [](auto& a, auto& b) {if (a[0].second < b[0].second) return true; else if (a[0].second > b[0].second) return false; else if (a[0].second == b[0].second) if (a[0].first < b[0].first) return true; else return false; return false; });
					dudes[i].x = paths[0][0].first;
					dudes[i].y = paths[0][0].second;
				}
			}
			// attack lowest hp dude if possible
			dude* d = nullptr;
			if (inRange(dudes[i].x, dudes[i].y, dudes[i].r, &d))
			{
				if (d->hp > 0 && dudes[i].hp > 0)
				{
					// comment this first one out for part 1
					if (dudes[i].r == ELF)
					{
						d->hp = max(d->hp - 16, 0);
					}
					else
					{
						d->hp = max(d->hp - 3, 0);
					}
					
					if (d->hp == 0)
					{
						if (d->r == ELF)
						{
							cout << "elf died!" << endl;
							numE--;
						}
						else
						{
							//cout << "goblin died!" << endl;
							numG--;
						}
					}
					continue;
				}
			}
		}

		char* map = new char[width * width];
		memcpy(map, graph, width*width);
		for (int i = 0; i < dudes.size(); i++) 
		{
			if (dudes[i].hp > 0)
				map[dudes[i].y * width + dudes[i].x] = dudes[i].r == ELF ? 'E' : 'G';
		}
		//cout << "round " << rounds+1 <<":" << endl;
		//for (int i = 0; i < width; i++)
		//{
		//	for (int j = 0; j < width; j++)
		//	{
		//		cout << map[i * width + j];
		//	}
		//	cout << endl;
		//}
		rounds++;
	}

	RACE winner;
	int remHp = 0;
	for (auto& d : dudes)
	{
		if (d.hp > 0)
		{
			winner = d.r;
			remHp += d.hp;
		}
	}

	// subtract 1 from rounds for part 1 answer...need to figure out why
	cout << remHp * (rounds) << endl;
	cout << ((winner == ELF) ? "Elves win" : "Goblins win") << endl;
	return 0;
}