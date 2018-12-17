#include <unordered_map>
#include <iostream>

using namespace std;
int frequencies[2048];

int main()
{
  int in;
  int sum = 0;
  unordered_map<int, int> m;
  int dup = 0;

  int total = 0;
  while ( cin >> in)
  {
    frequencies[total] = in;
    total++;
  }

  bool found = false;
  while(found == false)
  {
    for (int i = 0; i < total; i++)
    {
      sum += frequencies[i];
      if (m.find(sum) != m.end())
      {
        dup = in;
        found = true;
        break;
      }

      m[sum] = 1;
    }
  }

  cout << sum << endl;
  cout << dup;

  return 0;
}
