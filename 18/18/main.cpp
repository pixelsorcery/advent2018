/*AMDG*/
#include <iostream>
#include <string>

using namespace std;

constexpr int WIDTH = 52;
constexpr int HEIGHT = 52;
char grid[WIDTH][HEIGHT];
char grid2[WIDTH][HEIGHT];

void swapMem(char (**grid1)[WIDTH][WIDTH], char (**grid2)[WIDTH][WIDTH])
{
    char (*tmp)[WIDTH][WIDTH];
    tmp = *grid1;
    *grid1 = *grid2;
    *grid2 = tmp;
}
int main()
{
    string line;
    int i = 1;
    while (cin >> line)
    {
        memcpy(grid[i]+1, line.c_str(), WIDTH-2);
        i++;
    }

    char (*pCurGrid)[WIDTH][WIDTH] = &grid;
    char (*pNextGrid)[WIDTH][WIDTH] = &grid2;

#if a
    for (int i = 0; i < 10; i++)
#else
    // stabilizes at 560 and cycle is 56
    for (int i = 0; i < 560 + (1000000000 % 56); i++)
#endif
    {
        for (int y = 1; y < HEIGHT-1; y++)
        {
            for (int x = 1; x < WIDTH-1; x++)
            {
                int trees = 0;
                int lumberyards = 0;
                int empty = 0;
                for (int k = y - 1; k < y + 2; k++)
                {
                    for (int l = x - 1; l < x + 2; l++)
                    {
                        if (k == y && l == x) continue;
                        if ((*pCurGrid)[k][l] == '|') trees++;
                        else if ((*pCurGrid)[k][l] == '.') empty++;
                        else if ((*pCurGrid)[k][l] == '#') lumberyards++;
                    }
                }
                if ((*pCurGrid)[y][x] == '.' && trees >= 3) (*pNextGrid)[y][x] = '|';
                else if ((*pCurGrid)[y][x] == '|' && lumberyards >= 3) (*pNextGrid)[y][x] = '#';
                else if ((*pCurGrid)[y][x] == '#' && (lumberyards == 0 || trees == 0)) (*pNextGrid)[y][x] = '.';
                else if ((*pCurGrid)[y][x] == '#' && lumberyards > 0 && trees > 0) (*pNextGrid)[y][x] = '#';
                else (*pNextGrid)[y][x] = (*pCurGrid)[y][x];
            }
        }
        // swap grids
        swapMem(&pCurGrid, &pNextGrid);
    }

    int trees = 0;
    int lumberyards = 0;

    for (int y = 1; y < HEIGHT-1; y++)
    {
        for (int x = 1; x < WIDTH-1; x++)
        {
            if ((*pCurGrid)[y][x] == '#') lumberyards++;
            else if ((*pCurGrid)[y][x] == '|') trees++;
        }
    }

    cout << lumberyards * trees << endl;

    return 0;
}