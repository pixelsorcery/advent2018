/*AMDG*/
#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <cassert>

using namespace std;

int r[4];

void addi(int a, int b, int c)
{
    r[c] = r[a] + b;
}

void addr(int a, int b, int c)
{
    r[c] = r[a] + r[b];
}

void muli(int a, int b, int c)
{
    r[c] = r[a] * b;
}

void mulr(int a, int b, int c)
{
    r[c] = r[a] * r[b];
}

void bani(int a, int b, int c)
{
    r[c] = r[a] & b;
}

void banr(int a, int b, int c)
{
    r[c] = r[a] & r[b];
}

void bori(int a, int b, int c)
{
    r[c] = r[a] | b;
}

void borr(int a, int b, int c)
{
    r[c] = r[a] | r[b];
}

void setr(int a, int b, int c)
{
    r[c] = r[a];
}

void seti(int a, int b, int c)
{
    r[c] = a;
}

void gtir(int a, int b, int c)
{
    r[c] = ((a > r[b]) ? 1 : 0);
}

void gtri(int a, int b, int c)
{
    r[c] = ((r[a] > b) ? 1 : 0);
}

void gtrr(int a, int b, int c)
{
    r[c] = ((r[a] > r[b]) ? 1 : 0);
}

void eqir(int a, int b, int c)
{
    r[c] = ((a == r[b]) ? 1 : 0);
}

void eqri(int a, int b, int c)
{
    r[c] = ((r[a] == b) ? 1 : 0);
}

void eqrr(int a, int b, int c)
{
    r[c] = ((r[a] == r[b]) ? 1 : 0);
}

vector<void(*)(int, int, int)> opcodes;

int main()
{
    // opcodes in proper order (see comment on bottom)
    opcodes.push_back(bori);
    opcodes.push_back(borr);
    opcodes.push_back(addi);
    opcodes.push_back(muli);
    opcodes.push_back(addr);
    opcodes.push_back(bani);
    opcodes.push_back(gtri);
    opcodes.push_back(setr);
    opcodes.push_back(gtrr);
    opcodes.push_back(seti);
    opcodes.push_back(eqir);
    opcodes.push_back(eqrr);
    opcodes.push_back(mulr);
    opcodes.push_back(eqri);
    opcodes.push_back(gtir);
    opcodes.push_back(banr);

    // part 1
#if a
    opcodes.push_back(eqir);
    opcodes.push_back(eqri);
    opcodes.push_back(gtri);
    opcodes.push_back(gtir);
    opcodes.push_back(muli);
    opcodes.push_back(addi);
    opcodes.push_back(bori);
    opcodes.push_back(bani);
    opcodes.push_back(seti);

    opcodes.push_back(addr);
    opcodes.push_back(mulr);
    opcodes.push_back(banr);
    opcodes.push_back(borr);
    opcodes.push_back(eqrr);
    opcodes.push_back(gtrr);
    opcodes.push_back(setr);

    string ops[] = {
        "eqir",
        "eqri",
        "gtri",
        "gtir",
        "muli",
        "addi",
        "bori",
        "bani",
        "seti",
        "addr",
        "mulr",
        "banr",
        "borr",
        "eqrr",
        "gtrr",
        "setr" };

    regex rgex("[a-zA-Z]+:\\s*\\[(\\d+), (\\d+), (\\d+), (\\d+)\\]");
    smatch m;

    string line;
    int part1 = 0;
    vector<string> matches;
    while (getline(cin, line))
    {
        if (regex_match(line, m, rgex))
        {
            int a1 = stoi(m[1]);
            int b1 = stoi(m[2]);
            int c1 = stoi(m[3]);
            int d1 = stoi(m[4]);

            int op, aa, bb, cc;
            cin >> op >> aa >> bb >> cc;

            getline(cin, line);
            getline(cin, line);
            regex_match(line, m, rgex);

            int a2 = stoi(m[1]);
            int b2 = stoi(m[2]);
            int c2 = stoi(m[3]);
            int d2 = stoi(m[4]);

            int opMatches = 0;
            for (int i = 0; i < opcodes.size(); i++)
            {
                // load up registers
                r[0] = a1; r[1] = b1; r[2] = c1; r[3] = d1;
                opcodes[i](aa, bb, cc);

                if (r[0] == a2 && r[1] == b2 && r[2] == c2 && r[3] == d2)
                {
                    opMatches++;
                    //cout << i << " matches " << ops[i] << endl;
                    matches.push_back(ops[i]);
                }
            }

            if (opMatches >= 3) part1++;

            cout << op << ": ";
            for(int i = 0; i < opMatches; i++)
            {
                cout << matches[i] << ", ";
            }
            cout << endl;

            // read blank line
            getline(cin, line);
            matches.clear();
        }
    }

    cout << part1 << endl;
#endif

    int op, aa, bb, cc;
    while (cin >> op)
    {
        cin >> aa >> bb >> cc;
        opcodes[op](aa, bb, cc);
    }

    cout << r[0] << endl;
    return 0;
}

// eyeballing results:
// 0: bori
// 1: borr
// 2: addi
// 3: muli
// 4: addr
// 5: bani,
// 6: gtri
// 7: setr
// 8: gtrr
// 9: seti
// 10: eqir
// 11: eqrr
// 12: mulr
// 13: eqri
// 14: gtir
// 15: banr