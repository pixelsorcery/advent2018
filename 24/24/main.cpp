/*AMDG*/
#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_set>

using namespace std;

using int64 = int64_t;

enum ATTACK_TYPE
{
    cold        = 0x1,
    radiation   = 0x2,
    bludgeoning = 0x4,
    fire        = 0x8,
    slashing    = 0x10,
};

enum GROUP_TYPE
{
    inf = 0,
    imm = 1,
};

GROUP_TYPE winner;

int char2attack(char c)
{
    switch (c)
    {
    case 'c':
        return cold;
        break;
    case 'r':
        return radiation;
        break;
    case 'b':
        return bludgeoning;
        break;
    case 'f':
        return fire;
        break;
    case 's':
        return slashing;
        break;
    default:
        assert(!"invalid value");
        return -1;
        break;
    }
}

struct group
{
    int id;
    GROUP_TYPE type;
    int64 numUnits;
    int64 hp;
    int initiative;
    int weak;
    int immune;
    ATTACK_TYPE attack;
    int64 attackAmt;
    bool alive;
};

struct attack
{
    group* a;
    group* t;
};

group testGroup1 = { 1, imm, 17, 5390, 2, radiation | bludgeoning, 0, fire, 4507, true };
group testGroup2 = { 2, imm, 989, 1274, 3, bludgeoning | slashing, fire, slashing, 25, true };

group testGroup3 = { 1, inf, 801, 4706, 1, radiation, 0, bludgeoning, 116, true };
group testGroup4 = { 2, inf, 4485, 2961, 4, fire | cold, radiation, slashing, 12, true };

auto sortForTargetSelection = [](const group*& a, const group*& b) {
    if (a->numUnits * a->attackAmt < b->numUnits * b->attackAmt) return true;
    else if (a->numUnits * a->attackAmt > b->numUnits * b->attackAmt) return false;
    else if (a->initiative < b->initiative) return true;
    else return false;
};

struct sortByAttackPower
{
    inline bool operator() (const group* a, const group* b)
    {
        if (a->attackAmt * a->numUnits > b->attackAmt * b->numUnits)
            return true;
        else if (a->attackAmt * a->numUnits < b->attackAmt * b->numUnits)
            return false;
        else if (a->initiative > b->initiative)
            return true;
        else
            return false;
    }
};

struct sortForMaxDamage
{
    inline bool operator() (const pair<group*, int64>& a,
                            const pair<group*, int64>& b)
    {
        if (a.second > b.second)
            return true;
        else if (a.second < b.second)
            return false;
        else if (a.second == b.second)
        {
            if (a.first->attackAmt * a.first->numUnits > b.first->attackAmt * b.first->numUnits)
                return true;
            else if (a.first->attackAmt * a.first->numUnits < b.first->attackAmt * b.first->numUnits)
                return false;
            else if (a.first->attackAmt * a.first->numUnits == b.first->attackAmt * b.first->numUnits)
            {
                if (a.first->initiative > b.first->initiative)
                    return true;
                else if (a.first->initiative < b.first->initiative)
                    return false;
                else
                    assert(!"you misunderstood target selection");
            }
        }
        return false;
    }
};

struct sortForInitiative
{
    inline bool operator() (const attack& a, const attack& b)
    {
        if (a.a->initiative > b.a->initiative) return true;
        else return false;
    }
};

void findAttacks(vector<group*>& a,
                 vector<group*>& t,
                 vector<attack>& attacks)
{
    // target selection
    unordered_set<int> targetCandidates;

    sort(a.begin(), a.end(), sortByAttackPower());

    for (int i = 0; i < static_cast<int>(a.size()); i++)
    {
        vector<pair<group*, int64>> damageGroups = {};
        // go through every infection and figure out damage
        for (const auto& target : t)
        {
            int64 damage = a[i]->attackAmt * a[i]->numUnits;

            // apply modifier
            if (target->weak & a[i]->attack)
                damage *= 2;
            else if (target->immune & a[i]->attack)
                damage = 0;

            if (damage > 0)
                damageGroups.push_back(make_pair(target, damage));
        }

        if (damageGroups.size() > 0)
        {
            sort(damageGroups.begin(), damageGroups.end(), sortForMaxDamage());
        }

        // find next one
        for (int j = 0; j < damageGroups.size(); j++)
        {
            if (targetCandidates.find(damageGroups[j].first->id) == targetCandidates.end())
            {
                attacks.push_back({ a[i], damageGroups[j].first });
                // note which one we picked
                targetCandidates.insert(damageGroups[j].first->id);
                break;
            }
        }
    }
}

int64 run(vector<group*>& immune, vector<group*>& infection)
{
    while (immune.size() > 0 && infection.size() > 0)
    {
        // target selection
        vector<attack> attacks;
        findAttacks(immune, infection, attacks);
        findAttacks(infection, immune, attacks);

        if (attacks.size() == 0) break;

        // attack phase
        sort(attacks.begin(), attacks.end(), sortForInitiative());

        for (int i = 0; i < attacks.size(); i++)
        {
            if (attacks[i].a->alive == false) continue;

            assert(attacks[i].t->alive == true);

            int64 damage = attacks[i].a->attackAmt * attacks[i].a->numUnits;

            // apply modifier
            if (attacks[i].t->weak & attacks[i].a->attack)
                damage *= 2;
            else if (attacks[i].t->immune & attacks[i].a->attack)
                damage = 0;

            int64 unitsKilled = damage / attacks[i].t->hp;
            unitsKilled = min(unitsKilled, attacks[i].t->numUnits);
            attacks[i].t->numUnits -= unitsKilled;

            //cout << ((attacks[i].a->type == inf) ? "infection " : "immune system ")
            //    << "group " << attacks[i].a->id << " attacks defending group "
            //    << attacks[i].t->id << ", killing " << unitsKilled << " units" << endl;

            if (attacks[i].t->numUnits <= 0)
            {
                if (attacks[i].t->type == inf)
                {
                    auto itr = infection.begin();
                    for (; itr != infection.end(); itr++)
                    {
                        if ((*itr)->id == attacks[i].t->id) break;
                    }

                    attacks[i].t->alive = false;
                    iter_swap(itr, infection.end() - 1);
                    infection.pop_back();
                }
                else
                {
                    auto itr = immune.begin();
                    for (; itr != immune.end(); itr++)
                    {
                        if ((*itr)->id == attacks[i].t->id) break;
                    }

                    attacks[i].t->alive = false;
                    iter_swap(itr, immune.end()-1);
                    immune.pop_back();
                }
            }
        }
        //cout << endl;
    }

    int64 answer = 0;

    if (immune.size() > 0 && infection.size() > 0)
    {
        winner = inf;
        return -1;
    }
    if (immune.size() > 0)
    {
        for (const auto& u : immune)
        {
            answer += u->numUnits;
            winner = imm;
        }
    }
    else
    {
        for (const auto& u : infection)
        {
            answer += u->numUnits;
            winner = inf;
        }
    }

    return answer;
}

int main()
{
    vector<group*> immuneTest;
    immuneTest.push_back(&testGroup1);
    immuneTest.push_back(&testGroup2);

    vector<group*> infectionTest;
    infectionTest.push_back(&testGroup3);
    infectionTest.push_back(&testGroup4);

    int64 test = run(immuneTest, infectionTest);
    cout << "test: " << test << endl;
    assert(test == 5216);

    string tempStr;
    char c;

    string line;

    vector<group*> immune;
    vector<group*> infection;

    vector<group*> *gPtr = &immune;
    GROUP_TYPE type = imm;

    group groups[20];
    group originalInput[20];

    int id = 0;
    while (getline(cin, line))
    {
        if (line == "Immune System:" || line == "") continue;
        if (line == "Infection:")
        {
            gPtr = &infection;
            type = inf;
            continue;
        }

        group* g = &groups[id];
        memset(g, 0, sizeof(group));
        g->alive = true;
        g->id = id;
        g->type = type;
        stringstream ss(line);

        ss >> g->numUnits;
        ss >> tempStr >> tempStr >> tempStr;
        ss >> g->hp;
        ss >> tempStr >> tempStr;
        ss >> c;

        if (c == '(')
        {
            ss >> tempStr;
            if (tempStr == "weak")
            {
                ss >> tempStr;

                do {
                    ss >> tempStr;
                    g->weak |= char2attack(tempStr[0]);
                } while (tempStr[tempStr.size() - 1] == ',');
            }

            if (tempStr[tempStr.size() - 1] == ')')
                goto exitParenthesis;

            if (tempStr[tempStr.size() - 1] == ';')
                ss >> tempStr;

            if (tempStr == "immune")
            {
                ss >> tempStr;

                do {
                    ss >> tempStr;
                    g->immune |= char2attack(tempStr[0]);
                } while (tempStr[tempStr.size() - 1] == ',');
            }

            if (tempStr[tempStr.size() - 1] == ';' && g->weak == 0)
            {
                ss >> tempStr;
                if (tempStr == "weak")
                {
                    ss >> tempStr;

                    do {
                        ss >> tempStr;
                        g->weak |= char2attack(tempStr[0]);
                    } while (tempStr[tempStr.size() - 1] == ',');
                }
            }
        }

    exitParenthesis:
        ss >> tempStr >> tempStr >> tempStr >> tempStr >> tempStr;
        ss >> g->attackAmt;
        ss >> tempStr;
        g->attack = static_cast<ATTACK_TYPE>(char2attack(tempStr[0]));
        ss >> tempStr >> tempStr >> tempStr;
        ss >> g->initiative;

        gPtr->push_back(g);

        id++;
    }

    memcpy(originalInput, groups, sizeof(groups));

    int64 answer = run(immune, infection);
    cout << "part1: " << answer << endl;
    assert(answer == 22244);

    winner = inf;
    int64 boost = 0;
    while (winner == inf)
    {
        group testGroup1 = { 1, imm, 17, 5390, 2, radiation | bludgeoning, 0, fire, 4507 + boost, true };
        group testGroup2 = { 2, imm, 989, 1274, 3, bludgeoning | slashing, fire, slashing, 25 + boost, true };

        group testGroup3 = { 1, inf, 801, 4706, 1, radiation, 0, bludgeoning, 116, true };
        group testGroup4 = { 2, inf, 4485, 2961, 4, fire | cold, radiation, slashing, 12, true };

        vector<group*> immuneTest;
        immuneTest.push_back(&testGroup1);
        immuneTest.push_back(&testGroup2);

        vector<group*> infectionTest;
        infectionTest.push_back(&testGroup3);
        infectionTest.push_back(&testGroup4);

        int64 test = run(immuneTest, infectionTest);

        if (winner == imm)
        {
            cout << "test2: " << test << " with boost " << boost << endl;
            assert(test == 51);
        }

        boost++;
    }

    winner = inf;
    boost = 1;
    while (winner == inf)
    {
        //cout << boost << endl;
        infection.clear();
        immune.clear();
        memcpy(groups, originalInput, sizeof(groups));
        for (int i = 0; i < 10; i++)
        {
            groups[i].attackAmt += boost;
            immune.push_back(&groups[i]);
        }

        for (int i = 10; i < 20; i++)
        {
            infection.push_back(&groups[i]);
        }

        int64 answer2 = run(immune, infection);

        if (winner == imm)
        {
            cout << "part2: " << answer2 <<" with boost " << boost << endl;
        }
        boost++;
    }

    return 0;
}