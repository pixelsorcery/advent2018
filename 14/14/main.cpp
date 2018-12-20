#include <iostream>
#include <vector>

using namespace std;

int input = 380621;
char sinput[] = "380621";
vector<char> recipes;

bool findMatch()
{
    bool found = true;
    if (recipes.size() >= strlen(sinput))
    {
        for (int i = recipes.size() - strlen(sinput), j = 0; i < recipes.size(); i++, j++)
        {
            if (recipes[i] != sinput[j] - '0')
            {
                found = false;
                break;
            }
        }

        if (found == true)
        {
            // part 2
            cout << recipes.size() - strlen(sinput) << endl;
        }
    }
    else
    {
        found = false;
    }

    return found;
}

int main()
{
    recipes.push_back(3);
    recipes.push_back(7);

    int elf1 = 0;
    int elf2 = 1;

    bool loop = true;
    while (loop || recipes.size() < input+10)
    {
        char n = recipes[elf1] + recipes[elf2];
        if (n > 9)
        {
            recipes.push_back(n / 10);
            if (findMatch()) loop = false;

            recipes.push_back(n % 10);
            if (findMatch()) loop = false;
        }
        else
        {
            recipes.push_back(n);
            if (findMatch()) loop = false;
        }

        elf1 = (elf1 + recipes[elf1] + 1) % recipes.size();
        elf2 = (elf2 + recipes[elf2] + 1) % recipes.size();
    }

    for (int i = input; i < input + 10; i++)
    {
        // part 1
        printf("%c", recipes[i] + '0');
    }
    cout << endl;
    return 0;
}