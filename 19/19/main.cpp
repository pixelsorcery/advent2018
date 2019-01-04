#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

int r[6];

#define DPF 0

void addi(int a, int b, int c)
{
#if DPF
    int aval = r[a];
    int bval = b;
#endif
    r[c] = r[a] + b;
#if DPF
    //cout << "addi " << "r[" << c << "]" << " = " << "r[" << a << "]" << " + " << b << endl;
    cout << "addi " << "r[" << c << "]" << " = " << aval << " + " << bval << endl;
#endif
}

void addr(int a, int b, int c)
{
#if DPF
    int aval = r[a];
    int bval = r[b];
#endif
    r[c] = r[a] + r[b];
#if DPF
    //cout << "addr " << "r[" << c << "]" << " = " << "r[" << a << "]" << " + " "r[" << b << "]" << endl;
    cout << "addr " << "r[" << c << "]" << " = " << aval << " + " << bval << endl;
#endif
}

void muli(int a, int b, int c)
{
#if DPF
    int aval = r[a];
    int bval = b;
#endif
    r[c] = r[a] * b;
#if DPF
    cout << "muli " << "r[" << c << "]" << " = " << aval << " * " << bval << endl;
#endif
}

void mulr(int a, int b, int c)
{
#if DPF
    int aval = r[a];
    int bval = r[b];
#endif
    r[c] = r[a] * r[b];
#if DPF
    cout << "mulr " << "r[" << c << "]" << " = " << aval << " * " << bval << endl;
#endif
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
#if DPF
    int aval = r[a];
#endif

    r[c] = r[a];
#if DPF
    cout << "setr " << "r[" << c << "]" << " = " << aval << endl;
#endif
}

void seti(int a, int b, int c)
{
#if DPF
    int aval = a;
#endif
    r[c] = a;
#if DPF
    cout << "seti " << "r[" << c << "]" << " = " << aval << endl;
#endif
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
#if DPF
    int aval = r[a];
    int bval = r[b];
#endif
    r[c] = ((r[a] > r[b]) ? 1 : 0);
#if DPF
    cout << "gtrr " << "r[" << c << "]" << " = " << aval << " > " << bval << endl;
#endif
}

void eqir(int a, int b, int c)
{
    r[c] = ((a == r[b]) ? 1 : 0);
}

void eqri(int a, int b, int c)
{
    r[c] = ((r[a] == b) ? 1 : 0);
#if DPF
    cout << "eqri " << "r[" << c << "]" << " = " << "r[" << a << "]" << " == " << b << endl;
#endif
}

void eqrr(int a, int b, int c)
{
#if DPF
    int aval = r[a];
    int bval = r[b];
#endif
    r[c] = ((r[a] == r[b]) ? 1 : 0);
#if DPF
    cout << "eqrr " << "r[" << c << "]" << " = " << aval << " == " << bval << endl;
#endif
}

enum OP
{
    BORI = 0,
    BORR,
    ADDI,
    MULI,
    ADDR,
    BANI,
    GTRI,
    SETR,
    GTRR,
    SETI,
    EQIR,
    EQRR,
    MULR,
    EQRI,
    GTIR,
    BANR,
    ERR
};

OP str2op(string& s)
{
    if (s == "bori")
        return BORI;
    else if (s == "borr")
        return BORR;
    else if (s == "addi")
        return ADDI;
    else if (s == "muli")
        return MULI;
    else if (s == "addr")
        return ADDR;
    else if (s == "bani")
        return BANI;
    else if (s == "gtri")
        return GTRI;
    else if (s == "setr")
        return SETR;
    else if (s == "gtrr")
        return GTRR;
    else if (s == "seti")
        return SETI;
    else if (s == "eqir")
        return EQIR;
    else if (s == "eqrr")
        return EQRR;
    else if (s == "mulr")
        return MULR;
    else if (s == "borr")
        return EQRI;
    else if (s == "eqri")
        return GTIR;
    else if (s == "banr")
        return BANR;
    else
        assert(!"opcode not found");

    return ERR;
}

vector<void(*)(int, int, int)> opcodes;

struct instr
{
    OP op;
    int a;
    int b;
    int c;
};

vector<instr> instructions;

#define A 0
#if A
int main()
{
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

    string junk;
    int ipReg;
    cin >> junk >> ipReg;

#if A
#else
    r[0] = 1;
#endif

    string op;
    while (cin >> op)
    {
        int a, b, c;
        cin >> a >> b >> c;

        instructions.push_back({ str2op(op), a, b, c });
    }

    while (r[ipReg] < instructions.size())
    {
        instr i = instructions[r[ipReg]];
#if DPF
        cout << "ip: " << r[ipReg] << " ";
#endif
        opcodes[i.op](i.a, i.b, i.c);

        r[ipReg]++;
#if DPF
        cout << "" << r[0] << " " << r[1] << " " << r[2] << " " << r[3] << " " << r[4] << " " << r[5] << endl;
#endif
    }

    r[ipReg]--;

    cout << r[0] << endl;

    return 0;
}
#else
// rewrite in c
int main()
{
    // get initial state of registers before going into loop
    uint64_t r0 = 0;
    uint64_t r1 = 10551260;
    uint64_t r3 = 1;
    uint64_t r2 = 1;
    uint64_t r5 = 0;

    while (r3 <= r1)
    {
        r2 = 1;
        while (r2 <= r1)
        {
            if (r1 % r3 != 0) break; // OPTIMIZE
            r5 = r3 * r2;
            r5 = (r5 == r1) ? 1 : 0;
            if (r5 == 1)
            {
                r0 = r3 + r0;
            }
            r2 = r2 + 1;
        }

        r3 = r3 + 1;
    }

    cout << r0 << endl;
    return 0;
}

// 22157688

#endif
