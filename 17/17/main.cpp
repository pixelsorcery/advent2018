/*AMDG*/
/*this code is really bad I apologize to anyone reading it*/
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <memory>
#include <iomanip>

using namespace std;

constexpr int WIDTH = 2000;

using loc = pair<int, int>;

bool isEmpty(char* grid, loc l)
{
    if (grid[l.second * WIDTH + l.first] != '.')
        return false;
    else
        return true;
}

void printGrid(char* grid)
{
    for (int y = 0; y < 1900; y++)
    {
        cout << setw(9) << setfill(' ') << y;
        for (int x = 460; x < 620; x++)
        {
            cout << grid[y * WIDTH + x];
        }
        cout << endl;
    }
}

int main()
{
    unique_ptr<char[]> grid = make_unique<char[]>(WIDTH * WIDTH);
    memset(grid.get(), '.', WIDTH * WIDTH);
    grid[500] = '+';

    string line;
    regex rgx("([xy])=(\\d+),\\s([xy])=(\\d+)\\.\\.(\\d+)");
    smatch match;

    int maxy = 0;
    int maxx = 0;
    int miny = 2000;

    while (getline(cin, line))
    {
        if (regex_match(line, match, rgx))
        {
            if (match[1] == 'x')
            {
                int x = stoi(match[2]);
                if (x > maxx) maxx = x;
                int b = stoi(match[4]);
                if (b < miny) miny = b;
                int e = stoi(match[5]);
                if (e > maxy) maxy = e;
                for (int i = b; i <= e; i++)
                {
                    grid[i * WIDTH + x] = '#';
                }
            }
            else
            {
                int y = stoi(match[2]);
                if (y > maxy) maxy = y;
                if (y < miny) miny = y;
                int b = stoi(match[4]);
                int e = stoi(match[5]);
                if (e > maxx) maxx = e;
                for (int i = b; i <= e; i++)
                {
                    grid[y * WIDTH + i] = '#';
                }
            }
        }
        else
        {
            cerr << "Input not formatted correctly." << endl;
        }
    }

    vector<loc> q;
    q.push_back({ 500, 0 });
    int level = 0;
    vector<vector<loc>> downQueues = {};
    downQueues.push_back(q);

    while (downQueues.size() > 0)
    {
        vector<loc>& curq = downQueues.back();
        if (curq.size() == 0)
        {
            downQueues.pop_back(); 
            continue;
        }
        loc cur = curq.back();
        bool down = false;

        if (cur.second == maxy)
        {
            downQueues.clear();
            //printGrid(grid.get());

            // mark evey visited cell so far
            for (int i = 0; i < WIDTH * WIDTH; i++)
            {
                if (grid[i] == '|' || grid[i] == '~')
                    grid[i] = '&';
            }

            q.clear();
            //printGrid(grid.get());

            // find next source
            for (int y = 0; y < WIDTH; y++)
            {
                for (int x = 0; x < WIDTH; x++)
                {
                    if (grid[y * WIDTH + x] == '*')
                    {
                        q.push_back({ x, y });
                        downQueues.push_back(q);
                        x = WIDTH; y = WIDTH;
                    }
                }
            }
            continue;
        }
        // go down
        if (grid[cur.second * WIDTH + cur.first] == '*')
            grid[cur.second * WIDTH + cur.first] = '|';
        if (isEmpty(grid.get(), { cur.first, cur.second + 1 }))
        {
            while(isEmpty(grid.get(), { cur.first, cur.second + 1 }) && cur.second < maxy)
            {
                loc next = { cur.first, cur.second + 1 };
                grid[(next.second) * WIDTH + next.first] = '|';
                curq.push_back(next);
                cur = next;
            }
        }

        if (grid[(cur.second + 1) * WIDTH + cur.first] == '&')
        {
            //printGrid(grid.get());
            downQueues.clear();
            // mark evey visited cell so far
            for (int i = 0; i < WIDTH * WIDTH; i++)
            {
                if (grid[i] == '|' || grid[i] == '~')
                    grid[i] = '&';
            }
            q.clear();
            // find next source
            for (int y = 0; y < WIDTH; y++)
            {
                for (int x = 0; x < WIDTH; x++)
                {
                    if (grid[y * WIDTH + x] == '*')
                    {
                        q.push_back({ x, y });
                        downQueues.push_back(q);
                        x = WIDTH; y = WIDTH;
                    }
                }
            }
            continue;
        }

        // otherwise start filling
        while (down == false && curq.size() > 0 && cur.second < maxy)
        {
            grid[(cur.second) * WIDTH + cur.first] = '~';
            // fill left
            loc next = { cur.first - 1, cur.second };
            while (isEmpty(grid.get(), next))
            {
                // check down 
                if (isEmpty(grid.get(), { next.first, next.second + 1 }))
                {
                    down = true;
                    vector<loc> nq;
                    nq.push_back(next);
                    downQueues.push_back(nq);
                    grid[(next.second) * WIDTH + next.first] = '*';
                    break;
                }
                grid[(next.second) * WIDTH + next.first] = '~';
                next = { next.first - 1, next.second };
            }
            // fill right
            next = { cur.first + 1, cur.second };
            while (isEmpty(grid.get(), next))
            {
                // check down 
                if (isEmpty(grid.get(), { next.first, next.second + 1 }))
                {
                    down = true;
                    vector<loc> nq;
                    nq.push_back(next);
                    downQueues.push_back(nq);
                    grid[(next.second) * WIDTH + next.first] = '*';
                    break;
                }
                grid[(next.second) * WIDTH + next.first] = '~';
                next = { next.first + 1, next.second };
            }

            if (down == true)
                for (int i = 0; i < WIDTH; i++)
                    if (grid[(next.second) * WIDTH + i] == '~')
                        grid[(next.second) * WIDTH + i] = '|';

            // pop back up one level
            if (curq.size() > 0)
            {
                curq.pop_back();
                if (curq.size() > 0)
                {
                    cur = curq.back();
                }
            }
        }
    }
    cout << endl;
    //printGrid(grid.get());

    int answer = 0;
    for (int i = WIDTH*miny; i < 2000 * 2000; i++)
    {
        if (grid[i] == '&')
        {
            answer++;
        }
    }

    cout << answer << endl;

    // trace the water flow paths
    vector<loc> paths;
    paths.push_back({ 500, 0 });

    while (paths.size() > 0)
    {
        loc cur = paths.back();
        paths.pop_back();
        grid[cur.second * WIDTH + cur.first] = '|';

        loc next;
        if (grid[(cur.second) * WIDTH + cur.first + 1] != '&' &&
            grid[(cur.second) * WIDTH + cur.first - 1] != '&')
        {
            next = { cur.first, cur.second + 1 };

            while (grid[(next.second) * WIDTH + next.first + 1] != '&' &&
                   grid[(next.second) * WIDTH + next.first - 1] != '&' &&
                   grid[next.second * WIDTH + next.first] == '&')
            {
                grid[(next.second) * WIDTH + next.first] = '|';
                next = { next.first, next.second + 1 };
            }
            cur = next;
            grid[(cur.second) * WIDTH + cur.first] = '|';
        }

        // go right
        next = { cur.first + 1, cur.second };
        while (grid[(next.second) * WIDTH + next.first] == '&')
        {
            grid[(next.second) * WIDTH + next.first] = '|';
            next = { next.first + 1, next.second };

            if (grid[(next.second) * WIDTH + next.first] != '&' &&
                grid[(next.second) * WIDTH + next.first] != '#')
            {
                paths.push_back({ next.first - 1, next.second });
                break;
            }
        }

        next = { cur.first - 1, cur.second };
        while (grid[(next.second) * WIDTH + next.first] == '&')
        {
            grid[(next.second) * WIDTH + next.first] = '|';
            next = { next.first - 1, next.second };

            if (grid[(next.second) * WIDTH + next.first] != '&' && 
                grid[(next.second) * WIDTH + next.first] != '#')
            {
                paths.push_back({ next.first + 1, next.second });
                break;
            }
        }

    end:
        ;
    }

    //printGrid(grid.get());

    int answer2 = 0;
    for (int i = WIDTH * miny; i < 2000 * 2000; i++)
    {
        if (grid[i] == '&')
        {
            answer2++;
        }
    }

    cout << answer2 << endl;
    return 0;
}

// 349 too low
// 78517 too high
// 31953 correct

// pt 2:
// 25441 too low