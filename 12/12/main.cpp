/*AMDG*/
#include <iostream>
#include <string>
#include <unordered_map>
#include <regex>

using namespace std;

static char input[] = "#...####.##..####..#.##....##...###.##.#..######..#..#..###..##.#.###.#####.##.#.#.#.##....#..#..#..";
static char test[] = "#..#.#..##......###...###";
char answer[200];
unordered_map<string, char> m;

int main()
{
    memset(answer, '.', sizeof(answer));
    memcpy(&answer[50], input, sizeof(input));



    for (int i = 0; i < 200-5; i++)
    {
        if (strncmp(&input[i], test, 5) == 0)
        {
            // match
        }
    }
    return 0;
}