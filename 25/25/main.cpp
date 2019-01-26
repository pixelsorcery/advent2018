/*AMDG*/
#include <iostream>

using namespace std;

constexpr int numc = 1378;
int c[numc][4];

struct cons
{
    int c[numc][4];
    int size;
};

int added[numc];

int distance(cons c, int pt[4])
{
    int minDist = INT_MAX;
    for (int i = 0; i < c.size; i++)
    {
        int d = abs(c.c[i][0] - pt[0]) + abs(c.c[i][1] - pt[1]) +
                abs(c.c[i][2] - pt[2]) + abs(c.c[i][3] - pt[3]);

        if (d < minDist)
            minDist = d;
    }

    return minDist;
}

cons found[numc];

int main()
{
    memset(found, 0, sizeof(cons) * numc);
    memset(added, 0, sizeof(added));

    char junk;
    for (int i = 0; i < 1378; i++)
    {
        cin >> c[i][0] >> junk >> c[i][1] >> junk >> c[i][2] >> junk >> c[i][3];
    }

    int foundIdx = 0;
    memcpy(found[foundIdx].c, c[0], sizeof(int) * 4);
    found[foundIdx].size++;
    added[0] = 1;

    int addedSomething = 0;
    bool outerLoop = true;

    while (outerLoop)
    {
        bool loop = true;
        while (loop)
        {
            int numFound = 0;
            for (int i = 0; i < numc; i++)
            {
                if (distance(found[foundIdx], c[i]) <= 3 && added[i] == 0)
                {
                    memcpy(found[foundIdx].c + found[foundIdx].size, c[i], sizeof(int) * 4);
                    found[foundIdx].size++;
                    numFound++;
                    added[i] = 1;
                }
            }

            if (numFound == 0) loop = false;
        }

        // seed next one and loop
        foundIdx++;
        addedSomething = 0;
        for (int i = 0; i < numc; i++)
        {
            if (added[i] == 0)
            {
                memcpy(found[foundIdx].c, c[i], sizeof(int) * 4);
                found[foundIdx].size = 1;
                added[i] = 1;
                addedSomething = 1;
                break;
            }
        }

        if (addedSomething == 0) outerLoop = false;
    }

    cout << "part1: " <<foundIdx << endl;

    return 0;
}

// 1314 too high