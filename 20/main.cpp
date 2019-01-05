#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <sstream>
#include <unordered_set>

using namespace std;

string genKey(int x, int y)
{
    stringstream ss;
    ss << x << "_" << y;
    return ss.str();
}

struct node
{
    int val;
    vector<int> leafs;
};

struct stacknode
{
    int val;
    int x;
    int y;
};

int processString(string& s)
{
    unordered_set<string> memo;
    int answer = 0;
    int curLevel = 0;
    vector<stacknode> stack;
    int x = 0;
    int y = 0;
    int roomsOver1000 = 0;
    for (int i = 1; i < s.size(); i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
        {
            if (s[i] == 'N') y--;
            else if (s[i] == 'S') y++;
            else if (s[i] == 'W') x--;
            else if (s[i] == 'E') x++;

            if (memo.find(genKey(x, y)) == memo.end())
            {
                memo.insert(genKey(x,y));
                curLevel++;

                if (curLevel >= 1000) roomsOver1000++;
            }
            else
            {
                curLevel--;
            }

            //cout << s[i] << " " << curLevel << " " << x << ", " << y << endl;

            if (curLevel > answer)
            {
                answer = curLevel;
            }
        }
        else if (s[i] == '(')
        {
            //cout << "push " << curLevel << endl;
            stacknode n;
            n.val = curLevel;
            n.x = x;
            n.y = y;
            stack.push_back(n);
        }
        else if (s[i] == ')')
        {
            stacknode n = stack.back();
            curLevel = n.val;
            x = n.x;
            y = n.y;
            stack.pop_back();
        }
        else if (s[i] == '|')
        {
            if (stack.size() > 0)
            {
                curLevel = stack[stack.size() - 1].val;
                x = stack[stack.size() - 1].x;
                y = stack[stack.size() - 1].y;
            }
            else
            {
                curLevel = 0;
            }
        }
        else if (s[i] == '$')
        {
            break;
        }
    } 
    cout << "over 1000: " << roomsOver1000 << endl;
    cout << "return: " << answer << endl;
    return answer;
}

int main()
{
    string test0 = "^ENWWW(NEEE|SSE(EE|N))$";
    assert(10 == processString(test0));
    string test1 = "^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))$";
    assert(23 == processString(test1));
    string test2 = "^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$";
    assert(31 == processString(test2));
    string test3 = "^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$";
    assert(18 == processString(test3));

    string input;
    cin >> input;
    int answer = processString(input);

    //cout << answer << endl;

    return 0;
}
