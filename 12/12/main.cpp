/*AMDG*/
#include <iostream>
#include <string>
#include <unordered_map>
#include <regex>

using namespace std;

static char input[] = "#...####.##..####..#.##....##...###.##.#..######..#..#..###..##.#.###.#####.##.#.#.#.##....#..#..#..";
static char test[] = "#..#.#..##......###...###";
char answer[sizeof(input)*10];
char answer2[sizeof(input) * 10];
unordered_map<string, char> m;

int main()
{
    memset(answer, '.', sizeof(answer));
    memset(answer2, '.', sizeof(answer2));
    memcpy(&answer[sizeof(answer)/4], input, sizeof(input)-1);

    string algo;
    string z;
    char result;

    while (cin >> algo)
    {
        cin >> z;
        cin >> result;
        m[algo] = result;
    }

    char* pCur = answer;
    char* pNxt = answer2;

    for (int i = 0; i < 20; i++)
    {
        for (int i = 0; i < sizeof(answer) - 5; i++)
        {
            for (auto k : m)
            {
                if (strncmp(k.first.c_str(), &pCur[i], 5) == 0)
                {
                    // match
                    pNxt[i+2] = k.second;
                }
            }
        }
        char* t = pCur;
        pCur = pNxt;
        pNxt = t;
        memset(pNxt, '.', sizeof(answer));

        /*
        int sum = 0;
        static int pSum = 0;
        int zeroIdx = sizeof(answer) / 4;
        for (int i = 0; i < sizeof(answer); i++)
        {
            if (pCur[i] == '#')
                sum += i - zeroIdx;
        }
        //cout <<i+1 << ": " << sum << " diff: " << sum - pSum << endl;

        pSum = sum;
        */
    }

    //cout << pCur << endl;
    int sum = 0;
    int zeroIdx = sizeof(answer) / 4;
    for (int i = 0; i < sizeof(answer); i++)
    {
        if (pCur[i] == '#')
            sum += i - zeroIdx;
    }
    cout << sum << endl;

    //part 2 114: 10600 diff: 80
    // after 114 the score stabilizes and goes up by 80 each round,
    // so the solution is to multiply 5 billion turns - 114 by 80
    // and add score at 114 which is 10600
    uint64_t max = 10600 + ((50000000000 - 114) * 80);
    cout << max << endl;

    return 0;
}