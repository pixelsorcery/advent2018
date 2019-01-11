/*AMDG*/
#include <iostream>
#include <memory>
#include <cassert>
#include <unordered_set>
#include <sstream>
#include <algorithm>
#include <queue>
#include <cstring>
#include <unordered_map>

using namespace std;

// input:
// depth: 6084
// target: 14, 709

int64_t calcGrid(int64_t* grid, int64_t sizex, int64_t sizey, int64_t xtarget, int64_t ytarget, int64_t depth)
{
    int64_t sum = 0;
    for (int64_t y = 0; y < sizey; y++)
    {
        for (int64_t x = 0; x < sizex; x++)
        {
            if (x == 0 && y == 0)
            {
                grid[0] = 0;
            }
            else if (x == xtarget - 1 && y == ytarget - 1)
            {
                grid[y * sizex + x] = 0;
            }
            else if (y == 0)
            {
                grid[y * sizex + x] = ((x * 16807) + depth) % 20183;
            }
            else if (x == 0)
            {
                grid[y * sizex + x] = ((y * 48271) + depth) % 20183;
            }
            else
            {
                grid[y * sizex + x] = (grid[(y - 1) * sizex + x] * grid[y * sizex + (x - 1)] + depth) % 20183;
            }
            if (y < ytarget && x < xtarget)
            {
                sum += grid[y * sizex + x] % 3;
            }
        }
    }

    return sum;
}

string makeKey(int x, int y, int z)
{
    stringstream ss;
    ss << x << "_" << y << "_" << z;
    return ss.str();
}

enum Type : int32_t
{
    Rocky = 0,
    Wet = 1,
    Narrow = 2,
    NumTypes
};

enum Gear : int32_t
{
    Cg = 0,
    Torch,
    Neither,
    NumGear
};

struct node
{
    Gear g;
    int x;
    int y;
    int64_t t;
    int64_t h; // a* heuristic
};

bool canUse(Gear g, Type t)
{
    switch (g)
    {
    case Cg:
    {
        if (t == Rocky)
        {
            return true;
        }
        else if (t == Wet)
        {
            return true;
        }
        else if (t == Narrow)
        {
            return false;
        }
        break;
    }
    case Torch:
    {
        if (t == Rocky)
        {
            return true;
        }
        else if (t == Wet)
        {
            return false;
        }
        else if (t == Narrow)
        {
            return true;
        }
        break;
    }
    case Neither:
    {
        if (t == Rocky)
        {
            return false;
        }
        else if (t == Wet)
        {
            return true;
        }
        else if (t == Narrow)
        {
            return true;
        }
        break;
    }
    default:
        assert(!"wrong combination of gear and terrain type");
        break;
    }

    return false;
}

void genLocations(int x, int y, pair<int, int> p[4])
{
    p[0] = make_pair(x + 1, y);
    p[1] = make_pair(x - 1, y);
    p[2] = make_pair(x, y + 1);
    p[3] = make_pair(x, y - 1);
}


void printGrid(int64_t* grid, int xx, int yy, int sizex)
{
    for (int y = 0; y < yy; y++)
    {
        for (int x = 0; x < xx; x++)
        {
            cout << grid[y * sizex + x] << "\t";
        }
        cout << endl;
    }
}

int64_t fastPath(int64_t* grid, int64_t xmax, int64_t ymax, int64_t tx, int64_t ty)
{
    unordered_set<string> visited = {};
    unordered_map<string, int64_t> memo = {};

    // set up priority queue for djikstra
    auto cmp = [](node& a, node& b) { return a.t + a.h > b.t + b.h; };
    priority_queue<node, std::vector<node>, decltype(cmp)> unvisited(cmp);
    int result = 0;

    unvisited.push({ Torch, 0, 0, 0 });
    memo[makeKey(0, 0, Torch)] = 0;

    while (unvisited.size() > 0)
    {
        node n = unvisited.top();
        unvisited.pop();

        if (visited.find(makeKey(n.x, n.y, n.g)) != visited.end())
        {
            continue;
        }

        // mark it as visited with what gear
        visited.insert(makeKey(n.x, n.y, n.g));
        memo[makeKey(n.x, n.y, n.g)] = n.t;

        if (n.x == tx && n.y == ty && n.g == Torch)
        {
            return memo[makeKey(n.x, n.y, n.g)];
        }

        pair<int, int> locations[4];
        genLocations(n.x, n.y, locations);

        // visit neighbors
        for (int i = 0; i < sizeof(locations) / sizeof(pair<int, int>); i++)
        {
            int x = locations[i].first;
            int y = locations[i].second;
            if (x < xmax &&
                x >= 0 &&
                y < ymax &&
                y >= 0 &&
                visited.find(makeKey(x, y, n.g)) == visited.end())
            {
                Type t = static_cast<Type>(grid[y * xmax + x] % 3);
                if (canUse(static_cast<Gear>(n.g), t))
                {
                    if (visited.find(makeKey(x, y, n.g)) == visited.end())
                    {
                        unvisited.push({ n.g, x, y, n.t + 1, (abs(tx-x) + abs(ty-y)) });
                   }
                }
            }
        }
        // switch gear
        for (int g = 0; g < NumGear; g++)
        {
            Type t = static_cast<Type>(grid[n.y * xmax + n.x] % 3);
            if (visited.find(makeKey(n.x, n.y, g)) == visited.end() &&
                g != n.g && canUse(static_cast<Gear>(g), t))
            {
                unvisited.push({ static_cast<Gear>(g), n.x, n.y, n.t + 7, (abs(tx-n.x) + abs(ty-n.y)) });
            }
        }
    }

    return 0;
}

int main()
{
    int64_t answer = 0;

    int64_t depth = 6084;
    int64_t x = 14;
    int64_t y = 709;

    // guesstimate these values for part 2
    int64_t sizex = 1000;
    int64_t sizey = 1400;

    unique_ptr<int64_t[]> grid = make_unique<int64_t[]>(sizex * sizey * sizeof(int64_t));

    uint64_t test = calcGrid(grid.get(), sizex, sizey, 11, 11, 510);
    assert(test == 114);
    //printGrid(grid.get(), 11, 11, sizex);
    uint64_t test2 = fastPath(grid.get(), sizex, sizey, 10, 10);
    //cout << test2 << endl;
    assert(test2 == 45);

    memset(grid.get(), 0, x * y * sizeof(int64_t));

    answer = calcGrid(grid.get(), sizex, sizey, x + 1, y + 1, depth);
    cout << "part1: " << answer << endl; //10603

    answer = fastPath(grid.get(), sizex, sizey, x, y);
    cout << "part2: " << answer << endl;

    return 0;
}