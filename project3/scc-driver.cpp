#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "digraphMatrix.h"

int main()
{
  DigraphMatrix g("sample-graph1.txt");
  vector<int> component = g.stronglyConnectedComponents();

  cout << "SCC IDs: ";
  for (int cid : component)
    cout << ' ' << cid;
  cout << '\n';

  return 0;
}