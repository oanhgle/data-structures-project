#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#include "digraphMatrix.h"

// constructor: reads in digraph from given file; stores graph as an adjacency matrix
DigraphMatrix::DigraphMatrix(const string& file)
{
    string str;
    int col, row, m;

    // open input file
    ifstream in;
    in.open(file);

    // read n and m (# of vertice and edge)
    in >> n >> m;  
    in.ignore(); // ignore \n  

    // resize the vectors (inner and outner), then initialize all entries to false
    A.resize(n, vector<bool>(n, false));
    cout << "# of vertex: " << n << endl; //debug

    // read vertex and its neighbor
    for (int i = 0; i < m; i++)  
    {    
        getline(in, str);    
        istringstream iss(str); 
        iss >> row >> col;
        // mark true (1) to indicate edge
        A[row][col] = true;
        cout << "["<< row << "][" << col << "] = " << A[row][col] << endl;  //debug
    }
    cout << A.size() << " ";
}

// returns vector containing component ID for each vertex
vector<int> DigraphMatrix::stronglyConnectedComponents()
{
    vector<int> componentID(n);
    unordered_map<int, vector<int>> edge; //key: vertex , val : outgoing neighbor(s)
    // unordered_map<int, mark_t> marks; //key: vertex, val : enum mark
    stack<int> firstStack, secondStack;
    int count = 0;

    /*- mark all vertices initially undiscovered -*/
    for (int i = 0; i < n; i++)
        marks[i] = UNDISCOVERED;

    /*- first dfs pass | perform dfs @start, in sorted order -*/
    cout << "----perform first DFS pass----" << endl; //debug
    for (int i = 0; i < n; i++)
    { 
        if (marks[i] == UNDISCOVERED)
            DFS(i, firstStack);
    }

    /*- 
        reverse the edges in graph 
        A[v][u] = true -> A[v][u] = false && A[u][v] = true
    -*/

    for (int v = 0; v < n; v++)
    {
        // store the list of out neighbors of v | key : vertex; val: out neighbors
        edge[v] = getOutNeighbors(v);
        for(int u : edge[v])
            A[v][u] = false; // reset them to false
    }

    // iterate through edge map
    for(auto e : edge)
    {
        // iterate through out neighbors of a vertex (e.first)
        for(int u : e.second)
            A[u][e.first] = true; // set A[u][v] = true
    }

    // debug
    cout << "after reverse edge: " << endl;
    for (int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
            if(A[i][j] == true)
                cout << "["<< i << "][" << j << "] = " << A[i][j] << endl;
    }

    /*-  mark all the vertices undiscovered again -*/
    for (int i = 0; i < n; i++)
        marks[i] = UNDISCOVERED;

    /*- second dfs pass | perform dfs @top of first stack -*/
    cout << "----perform second DFS pass----" << endl; //debug
    while(!firstStack.empty())
    {
        int top = firstStack.top();
        if (marks[top] == UNDISCOVERED)
        {
            // call recursive call on current undiscovered vertex
            // push explored vertices into the second stack
            DFS(top, secondStack);

            // second stack then combines all the vertices that are encountered in the recursive function
            // assign the same component ID to all of the vertices in the second stack
            // pop all the vertices to empty the second stack before moving on to the next component
            cout << "SCC " << count << ": ";
            while(!secondStack.empty()) //debug
            {
                int element = secondStack.top();
                cout << element << " "; //debug
                componentID[element] = count;
                secondStack.pop();
            }
            cout << endl; //debug
            count++;
        }
        // pop the current top of the first stack
        firstStack.pop();
    }
    return componentID;
}

// recursive DFS function
void DigraphMatrix::DFS(int v, stack<int>& st)
{  
    marks[v] = DISCOVERED;   
    cout << v << " = DISCOVERED" << endl; //debug
    for (int u = 0; u < n; u++)
        if(A[v][u] == true)
            if (marks[u] == UNDISCOVERED)      
                DFS(u, st); 
    
    // mark vertex as explored
    marks[v] = EXPLORED;
    cout << v << " = EXPLORED" << endl; //debug

    // add the vertex of the graph to a stack as you explore them
    st.push(v);
}

// return the out neighbors of a vertex
vector<int> DigraphMatrix::getOutNeighbors(int v)
{
    vector<int> outNeighbors;
    for(int u = 0 ; u < n; u++)
        if(A[v][u] == true)
            outNeighbors.push_back(u);
    return outNeighbors;
}