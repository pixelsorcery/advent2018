#include <iostream>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <vector>
#include <limits>
#include <regex>
#include "stb_image_write.h"

using namespace std;

struct v
{
	int x, y, vx, vy;
};

int main()
{
	regex r("position=<([-|\\s]?\\d+), ([-|\\s]?\\d+)> velocity=<([-|\\s]?\\d), ([-|\\s]?\\d)>");
	smatch m;
	string l;
	vector<v> nodes;
	int* image = new int[1024 * 1024 * sizeof(int)];

	while (getline(cin, l))
	{
		if (regex_match(l, m, r))
		{
			nodes.push_back({stoi(m[1]),stoi(m[2]),stoi(m[3]),stoi(m[4])});
		}
	}

	bool begin = false;
	for (int i = 0; i < 15000; i++)
	{
		for (int j = 0; j < nodes.size(); j++)
		{
			nodes[j].x += nodes[j].vx;
			nodes[j].y += nodes[j].vy;

			if (nodes[j].x < 20 && nodes[j].x > 0)
			{
				begin = true;
			}
		}

		if (begin)
		{
			for (int k = 0; k < 1024 * 1024; k++)
			{
				image[k] = 0xff000000;
			}

			for (int k = 0; k < nodes.size(); k++)
			{
				if (nodes[k].x > 0 && nodes[k].y > 0 && nodes[k].x < 1024 && nodes[k].y < 1024)
				{
					image[1024 * nodes[k].y + nodes[k].x] = 0xffffffff;
				}
			}
			string filename = "output";
			filename += to_string(i);
			filename += ".bmp";
			stbi_write_bmp(filename.c_str(), 1024, 1024, 4, image);
		}
	}

	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].x > 0 && nodes[i].y > 0 && nodes[i].x < 1024 && nodes[i].y < 1024)
		{
			image[1024 * nodes[i].y + nodes[i].x];
		}
	}

	delete[] image;
	return 0;
}