#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

constexpr int SIZE = 150;
char track[SIZE * SIZE];

enum TURN
{
    L,
    S,
    R,
    NUM_TURN
};

enum DIR
{
    UP,
    LEFT,
    DOWN,
    RIGHT,
    NUM_DIR
};

struct t
{
    TURN t;
    DIR d;
    int x;
    int y;
    bool crashed;
};

vector<t> trains;

void move(t& train)
{
    // check if we're on a straight tile
    if (track[train.y * SIZE + train.x] == '-' ||
        track[train.y * SIZE + train.x] == '|')
    {
        // do nothing?
    }
    else if (track[train.y * SIZE + train.x] == '+')
    {
        // change dir
        if (train.t == L) train.d = static_cast<DIR>((train.d + 1) % NUM_DIR);
        else if (train.t == R)
        {
            if (train.d == 0) train.d = RIGHT;
            else train.d = static_cast<DIR>(train.d - 1);
        }

        // update change
        train.t = static_cast<TURN>((train.t + 1) % NUM_TURN);
    }
    else if (track[train.y * SIZE + train.x] == '\\')
    {
        if (train.d == UP) train.d = LEFT;
        else if (train.d == DOWN) train.d = RIGHT;
        else if (train.d == LEFT) train.d = UP;
        else if (train.d == RIGHT) train.d = DOWN;
    }
    else if (track[train.y * SIZE + train.x] == '/')
    {
        if (train.d == LEFT) train.d = DOWN;
        else if (train.d == RIGHT) train.d = UP;
        else if (train.d == UP) train.d = RIGHT;
        else if (train.d == DOWN) train.d = LEFT;
    }
    else
    {
        assert(!"we've gone off the rails!");
    }

    if (train.d == UP) train.y--;
    else if (train.d == DOWN) train.y++;
    else if (train.d == LEFT) train.x--;
    else if (train.d == RIGHT) train.x++;
}

int main()
{
    string l;
    int i = 0;
    while (getline(cin, l))
    {
        for (int j = 0; j < l.size(); j++)
        {
            switch (l[j])
            {
                case '^':
                    trains.push_back({ L, UP, j, i, false });
                    l[j] = '|';
                    break;
                case '<':
                    trains.push_back({ L, LEFT, j, i, false });
                    l[j] = '-';
                    break;
                case 'v':
                    trains.push_back({ L, DOWN, j, i, false });
                    l[j] = '|';
                    break;
                case '>':
                    trains.push_back({ L, RIGHT, j, i, false });
                    l[j] = '-';
                    break;
            }
        }
        memcpy(&track[i * SIZE], l.c_str(), l.size());
        i++;
    }

    bool collision = false;

    int moves = 0;
    int crashed = 0;
    while (crashed < trains.size() - 1)
    {
        sort(trains.begin(), trains.end(), [](t a, t b) {if (a.y < b.y) return true; else if (a.y > b.y) return false; else if (a.y == b.y) if (a.x > b.x) return true; else return false; });
        for (int i = 0; i < trains.size(); i++)
        {
            if (trains[i].crashed)
            {
                continue;
            }
            else
            {
                move(trains[i]);
            }

            for (int j = 0; j < trains.size(); j++)
            {
                if (trains[j].crashed) continue;
                for (int k = 0; k < trains.size(); k++)
                {
                    if (trains[k].crashed) continue;
                    if (k == j) continue;
                    if (trains[j].x == trains[k].x &&
                        trains[j].y == trains[k].y)
                    {
                        if (collision == false)
                        {
                            cout << trains[j].x << "," << trains[j].y << endl;
                            collision = true;
                        }
                        crashed += 2;
                        trains[j].crashed = true;
                        trains[k].crashed = true;
                    }
                }
            }
        }
        moves++;
    }

    for (auto& t : trains)
    {
        if (t.crashed == false)
        {
            cout << "part2: " << t.x << "," << t.y << endl;
        }
    }

    return 0;
}
