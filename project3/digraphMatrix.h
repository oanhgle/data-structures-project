#ifndef __DIGRAPH_MATRIX_H
#define __DIGRAPH_MATRIX_H

#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
using namespace std;

enum mark_t { UNDISCOVERED, DISCOVERED, EXPLORED };


class DigraphMatrix
{
private:
  int n;
  vector<vector<bool>> A;
  unordered_map<int, mark_t> marks;

public:
  //Reads in digraph from given file
  //Stores graph as an adjacency matrix
  DigraphMatrix(const string&);

  //Returns number of vertices
  int getOrder() const { return n; }

  //Returns vector containing component ID for each vertex
  vector<int> stronglyConnectedComponents();

  //Recursive DFS function
  void DFS(int, stack<int>&);

  //Return the out neighbors of a vertex
  vector<int> getOutNeighbors(int);
};

#endif