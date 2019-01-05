#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <unordered_set>

using namespace std;

int64_t r[6];

#define DPF 0

void addi(int64_t a, int64_t b, int64_t c)
{
#if DPF
    int64_t aval = r[a];
    int64_t bval = b;
#endif
    r[c] = r[a] + b;
#if DPF
    //cout << "addi " << "r[" << c << "]" << " = " << "r[" << a << "]" << " + " << b << endl;
    cout << "addi " << "r[" << c << "]" << " = " << aval << " + " << bval << endl;
#endif
}

void addr(int64_t a, int64_t b, int64_t c)
{
#if DPF
    int64_t aval = r[a];
    int64_t bval = r[b];
#endif
    r[c] = r[a] + r[b];
#if DPF
    //cout << "addr " << "r[" << c << "]" << " = " << "r[" << a << "]" << " + " "r[" << b << "]" << endl;
    cout << "addr " << "r[" << c << "]" << " = " << aval << " + " << bval << endl;
#endif
}

void muli(int64_t a, int64_t b, int64_t c)
{
#if DPF
    int64_t aval = r[a];
    int64_t bval = b;
#endif
    r[c] = r[a] * b;
#if DPF
    cout << "muli " << "r[" << c << "]" << " = " << aval << " * " << bval << endl;
#endif
}

void mulr(int64_t a, int64_t b, int64_t c)
{
#if DPF
    int64_t aval = r[a];
    int64_t bval = r[b];
#endif
    r[c] = r[a] * r[b];
#if DPF
    cout << "mulr " << "r[" << c << "]" << " = " << aval << " * " << bval << endl;
#endif
}

void bani(int64_t a, int64_t b, int64_t c)
{
    r[c] = r[a] & b;
}

void banr(int64_t a, int64_t b, int64_t c)
{
    r[c] = r[a] & r[b];
}

void bori(int64_t a, int64_t b, int64_t c)
{
    r[c] = r[a] | b;
}

void borr(int64_t a, int64_t b, int64_t c)
{
    r[c] = r[a] | r[b];
}

void setr(int64_t a, int64_t b, int64_t c)
{
#if DPF
    int64_t aval = r[a];
#endif

    r[c] = r[a];
#if DPF
    cout << "setr " << "r[" << c << "]" << " = " << aval << endl;
#endif
}

void seti(int64_t a, int64_t b, int64_t c)
{
#if DPF
    int64_t aval = a;
#endif
    r[c] = a;
#if DPF
    cout << "seti " << "r[" << c << "]" << " = " << aval << endl;
#endif
}

void gtir(int64_t a, int64_t b, int64_t c)
{
    r[c] = ((a > r[b]) ? 1 : 0);
}

void gtri(int64_t a, int64_t b, int64_t c)
{
    r[c] = ((r[a] > b) ? 1 : 0);
}

void gtrr(int64_t a, int64_t b, int64_t c)
{
#if DPF
    int64_t aval = r[a];
    int64_t bval = r[b];
#endif
    r[c] = ((r[a] > r[b]) ? 1 : 0);
#if DPF
    cout << "gtrr " << "r[" << c << "]" << " = " << aval << " > " << bval << endl;
#endif
}

void eqir(int64_t a, int64_t b, int64_t c)
{
    r[c] = ((a == r[b]) ? 1 : 0);
}

void eqri(int64_t a, int64_t b, int64_t c)
{
    r[c] = ((r[a] == b) ? 1 : 0);
#if DPF
    cout << "eqri " << "r[" << c << "]" << " = " << "r[" << a << "]" << " == " << b << endl;
#endif
}

void eqrr(int64_t a, int64_t b, int64_t c)
{
#if DPF
    int64_t aval = r[a];
    int64_t bval = r[b];
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
    else if (s == "eqri")
        return EQRI;
    else if (s == "gtir")
        return GTIR;
    else if (s == "banr")
        return BANR;
    else
        assert(!"opcode not found");

    return ERR;
}

vector<void(*)(int64_t, int64_t, int64_t)> opcodes;

struct instr
{
    OP op;
    int64_t a;
    int64_t b;
    int64_t c;
};

vector<instr> instructions;

#if 1
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

    string op;
    while (cin >> op)
    {
        int64_t a, b, c;
        cin >> a >> b >> c;

        instructions.push_back({ str2op(op), a, b, c });
    }
    int64_t timestamp = 0;
    r[0] = 0;
    int64_t instrs = 0;
    uint64_t max = 0;
    unordered_set<int64_t> r0s;
    while (r[ipReg] < instructions.size())
    {
        instr i = instructions[r[ipReg]];
        instrs++;
#if DPF
        cout << "ip: " << r[ipReg] << " ";
#endif
        opcodes[i.op](i.a, i.b, i.c);

        r[ipReg]++;

        if (r[ipReg] == 17) //OPTIMIZE
        {
            r[5] >>= 8;
            instrs += r[5] * 6;
            instrs++;
            r[ipReg] = 27;
        }

        if (r[ipReg] == 28)
        {
            if (r0s.find(r[4]) == r0s.end())
            {
                r0s.insert(r[4]);
                // first one is part a, last one is part b
                cout << "r4:" << r[4] << endl;
            }
        }
        
#if DPF
        //cout << "" << r[0] << " " << r[1] << " " << r[2] << " " << r[3] << " " << r[4] << " " << r[5] << endl;
#endif
    }

    //r[ipReg]--;

    cout << r[0] << endl;

    return 0;
}
#else

// rewrite in c to understand what's going on

int main()
{
    uint64_t r0 = 0;
    uint64_t r1 = 0; 
    uint64_t r3 = 0;
    uint64_t r4 = 0; 
    uint64_t r5 = 0;

tag0:

    //seti 123 0 4
    r4 = 123;
    //bani 4 456 4
    r4 &= 456;
    //eqri 4 72 4
    r4 = (r4 == 72 ? 1 : 0);

    //addr 4 2 2
    //seti 0 0 2
    if (r4 != 1)
    {
        goto tag0;
    }
    //seti 0 5 4
    r4 = 0;
    do
    {
        //bori 4 65536 5
        r5 = r4 | 65536;
        //seti 1765573 9 4
        r4 = 1765573;
        do
        {
            //bani 5 255 1
            r1 = r5 & 255;
            //addr 4 1 4
            r4 = r4 + r1;
            //bani 4 16777215 4
            r4 = r4 & 16777215;
            //muli 4 65899 4
            r4 = r4 * 65899;
            //bani 4 16777215 4
            r4 = r4 & 16777215;
            //gtir 256 5 1
            r1 = (256 > r5 ? 1 : 0);
            if (r1 == 1) break;
            //addr 1 2 2
            //addi 2 1 2
            //seti 27 0 2
            //seti 0 8 1
            r1 = 0;
            int i = 0;
            //do
            //{
            //    //addi 1 1 3
            //    r3 = r1 + 1;
            //    //muli 3 256 3
            //    r3 = r3 * 256;
            //    //gtrr 3 5 3
            //    r3 = (r3 > r5 ? 1 : 0);
            //    //addr 3 2 2
            //   //addi 2 1 2
            //    //seti 25 1 2
            //    //addi 1 1 1
            //    if (r3 == 1) break;
            //    r1++;
            //    i++;
            //} while (1); 
                //setr 1 4 5
            r5 >>= 8;
            //r5 = r1;
        } while (1);
        //seti 7 6 2
        //eqrr 4 0 1
        r1 = (r4 == r0 ? 1 : 0);
        //addr 1 2 2
        //seti 5 2 2
        cout << r4 << endl;
    } while (r4 != r0); 
    return 0;
}
#endif

// part a
// 12213578

// part b
// 5310683