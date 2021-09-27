/*
Driver: Michael Bodzenski
Navigator: Oanh Le
*/

#include <iostream>
#include <unordered_map>
#include <math.h>
using namespace std;

#include "hilbert.h"

//initializes an InstructionList and Turtle to draw a hilbert curve of the given level and size
HilbertCurve::HilbertCurve(int level, int size) : turtle(size, size)
{
    //initialize the rules to ODD
    rules.append(ODD);

    //set the distance the turtle will travel on a FORWARD command
    rules.setDistance(size / pow(2, level));

    //create a list for ODD and EVEN to create a map
    //O -> L, E, F, R, O, F, O, R, F, E, L
    InstructionList odd;
    odd.append(TURN_LEFT);
    odd.append(EVEN);
    odd.append(FORWARD);
    odd.append(TURN_RIGHT);
    odd.append(ODD);
    odd.append(FORWARD);
    odd.append(ODD);
    odd.append(TURN_RIGHT);
    odd.append(FORWARD); 
    odd.append(EVEN);
    odd.append(TURN_LEFT);

    //E -> R, O, F, L, E, F, E, L, F, O, R;
    InstructionList even;
    even.append(TURN_RIGHT);
    even.append(ODD);
    even.append(FORWARD);
    even.append(TURN_LEFT);
    even.append(EVEN);
    even.append(FORWARD);
    even.append(EVEN);
    even.append(TURN_LEFT);
    even.append(FORWARD);
    even.append(ODD);
    even.append(TURN_RIGHT);

    //create a map for the substitution of ODD and EVEN
    unordered_map<hilbert_t, const InstructionList*> map;
    map[ODD] = &odd;
    map[EVEN] = &even;

    //call the substitute function for the rules once for each level
    int i;
    for(i = 0; i < level; i++)
    {
        rules.substitute(map);
    }

    //call the follow function that executes all the instructions in the instruction list
    turtle.follow(rules);
}