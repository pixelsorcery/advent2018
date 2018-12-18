/*AMDG*/
#include <iostream>
#include <cassert>

using namespace std;

constexpr int serial = 9445;

int grid[301][301];

int powerLevel(int x, int y, int s)
{
    return ((((((x + 10) * y) + s) * (x + 10)) / 100) % 10) - 5;
}

int main()
{
    assert(powerLevel(122, 79, 57)  == -5);
    assert(powerLevel(217, 196, 39) == 0);
    assert(powerLevel(101, 153, 71) == 4);

    for (int y = 1; y <= 300; y++)
    {
        for (int x = 1; x <= 300; x++)
        {
            grid[y][x] = powerLevel(x, y, serial);
        }
    }

    pair<int, int> answer;
    int highest = 0;
    int maxSize = 0;

#if a
    int size = 3;
#else
    for (int size = 1; size < 300; size++)
#endif
        for (int y = 1; y <= 300 - size; y++)
        {
            for (int x = 1; x <= 300 - size; x++)
            {
                int sum = 0;
                for (int y1 = 0; y1 < size; y1++)
                {
                    for (int x1 = 0; x1 < size; x1++)
                    {
                        sum += grid[y + y1][x + x1];
                    }
                }

                if (sum > highest)
                {
                    highest = sum;
                    answer = make_pair(x, y);
                    maxSize = size;
                }

            }
        }

    // part 1
    cout << answer.first << "," << answer.second << endl;
    // part 2
    cout << answer.first << "," << answer.second << "," << maxSize << endl;

    return 0;
}