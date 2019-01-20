/*AMDG*/
#include <iostream>
#include <algorithm>
#include <vector>
#include <algorithm>

using namespace std;

using int64 = int64_t;

struct r
{
    int64 x;
    int64 y;
    int64 z;
    int64 s;
};

struct b
{
    int64 maxx;
    int64 maxy;
    int64 maxz;
    int64 minx;
    int64 miny;
    int64 minz;
};

int64 distance(r a, r b)
{
    return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
}

int64 r2box(r a, b box)
{
    int64 dist2x = max(max(box.minx - a.x, 0ll), a.x - box.maxx);
    int64 dist2y = max(max(box.miny - a.y, 0ll), a.y - box.maxy);
    int64 dist2z = max(max(box.minz - a.z, 0ll), a.z - box.maxz);

    return dist2x + dist2y + dist2z;
}

constexpr int NUM_BOTS = 1000;
r robots[NUM_BOTS];

int64 minMax[8][6] = {{ 1, 1, 1, 0, 0, 0},
                      { 0, 1, 1,-1, 0, 0},
                      { 1, 1, 0, 0, 0,-1},
                      { 0, 1, 0,-1, 0,-1},
                      { 1, 0, 1, 0,-1, 0},
                      { 0, 0, 1,-1,-1, 0},
                      { 1, 0, 0, 0,-1,-1},
                      { 0, 0, 0,-1,-1,-1}};


int main()
{
    char junk;
    int64 maxPowerIdx = -1;
    int64 maxPower = 0;
    int64 maxQuadrantSize = 0;

    for (int i = 0; i < NUM_BOTS; i++)
    {
        cin >> junk >> junk >> junk >> junk >> junk
            >> robots[i].x >> junk
            >> robots[i].y >> junk
            >> robots[i].z
            >> junk >> junk >> junk >> junk
            >> robots[i].s;

        if (robots[i].s > maxPower)
        {
            maxPower = robots[i].s;
            maxPowerIdx = i;
        }

        int64 maxDim = max(max(abs(robots[i].x), abs(robots[i].y)), abs(robots[i].z));
        if (maxDim > maxQuadrantSize) maxQuadrantSize = maxDim;
    }

    int answer1 = 0;
    for (int i = 0; i < NUM_BOTS; i++)
    {
        int64 d = distance(robots[i], robots[maxPowerIdx]);
        if (d <= maxPower)
        {
            answer1++;
        }
    }
    cout << "part1: " << answer1 << endl;

    int64 answer2 = 0;

    struct node
    {
        b box;
        int64 quadSize;
        int numbots;
    };

    auto vecsort = [](const node& a, const node& b) 
    {
        if (a.numbots < b.numbots)
            return true;
        else //if (a.numbots > b.numbots) 
            return false;
    };

    struct info
    {
        r loc;
        int numbots;
        int64 distance;
    };

    auto locsort = [](const info& a, const info& b) 
    {
        if (a.numbots > b.numbots) return true;
        else if (a.numbots < b.numbots) return false;
        else //if (a.numbots == b.numbots)
        {
            r temp = { 0, 0, 0, 0 };
            if (distance(a.loc, temp) < distance(b.loc, temp))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    };

    vector<node> pq;
    maxQuadrantSize *= 2;
    b initialQuad = { maxQuadrantSize, maxQuadrantSize, maxQuadrantSize, -maxQuadrantSize, -maxQuadrantSize, -maxQuadrantSize };
    pq.push_back({ initialQuad, maxQuadrantSize * 2, NUM_BOTS });

    int64 answer2dist = INT64_MAX;
    int answer2bots = 0;

    while (pq.size() > 0)
    {
        node cur = pq.back();
        pq.pop_back();

        if (answer2bots != 0 && answer2bots > cur.numbots) 
            continue;

        if (cur.quadSize < 10)
        {
            vector<info> locPoints;
            for (int z = cur.box.minz; z <= cur.box.maxz; z++)
            {
                for (int y = cur.box.miny; y <= cur.box.maxy; y++)
                {
                    for (int x = cur.box.minx; x <= cur.box.maxx; x++)
                    {
                        r loc = { x, y, z, 0};
                        info nfo = { loc, 0, 0 };

                        for (int k = 0; k < NUM_BOTS; k++)
                        {
                            int64 d = distance(loc, robots[k]);
                            if (d <= robots[k].s)
                            {
                                nfo.numbots++;
                            }
                        }

                        nfo.distance = distance(loc, { 0,0,0,0 });
                        locPoints.push_back(nfo);
                    }
                }
            }

            if (locPoints.size() > 0)
            {
                sort(locPoints.begin(), locPoints.end(), locsort);
                if (answer2bots < locPoints[0].numbots)
                {
                    answer2dist = locPoints[0].distance;
                    answer2bots = locPoints[0].numbots;
                    //cout << "bots: " << locPoints[0].numbots << " ";
                    //cout << "distance: " << locPoints[0].distance << endl;
                }
            }
            continue;
        }

        // get centerpoint
        int cx = (cur.box.maxx + cur.box.minx) / 2;
        int cy = (cur.box.maxy + cur.box.miny) / 2;
        int cz = (cur.box.maxz + cur.box.minz) / 2;

        int64 newQuadSize = cur.quadSize / 2;

        // for each 8 sectors
        for (int i = 0; i < 8; i++)
        {
            b box = { minMax[i][0] * newQuadSize + cx,
                      minMax[i][1] * newQuadSize + cy,
                      minMax[i][2] * newQuadSize + cz,
                      minMax[i][3] * newQuadSize + cx,
                      minMax[i][4] * newQuadSize + cy,
                      minMax[i][5] * newQuadSize + cz };

            int numbots = 0;
            for (int r = 0; r < NUM_BOTS; r++)
            {
                int64 distance = r2box(robots[r], box);
                if (distance <= robots[r].s)
                {
                    numbots++;
                }
            }

            pq.push_back({ box, newQuadSize, numbots });
            sort(pq.begin(), pq.end(), vecsort);
        }
    }

    std::cout << "part2: " << answer2dist << endl;

    return 0;
}