/*
Driver: Oanh Le
Navigator: Michael Bodzenski
*/

#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

#include "instruction_list.h"

//prints the InstructionList
ostream& operator<<(ostream& out, const InstructionList& list)
{
    //iterates through the list printing out each instruction
    for(auto iter = list.begin(); iter != list.end(); ++iter)
    {
        if(*iter == FORWARD)
            out << "F ";
        else if (*iter == TURN_LEFT)
            out << "L ";
        else if(*iter == TURN_RIGHT)
            out << "R ";
        else if(*iter == ODD)
            out << "O ";
        else if(*iter == EVEN)
            out << "E ";
    }
    return out;
}

//Add the given instruction to the end of the list
void InstructionList::append(hilbert_t instruction)
{
    //create a new node
    node_t* n_node = new node_t;
    n_node->move = instruction;
    n_node->next = NULL;
    
    //if the list is empty
    if(head == nullptr)
    {
        head = n_node;
    }
    //if the list has only one node (head)
    else if (tail == nullptr)
    {
        head->next = n_node;
        tail = n_node;
    }
    else
    {
        tail->next = n_node;
        tail = n_node;
    }
}

//Iterates through InstructionList, replacing instructions 
//in the list according to the substitution rules given in the map
void InstructionList::substitute(const unordered_map<hilbert_t, const InstructionList*>& map)
{
    node_t* cur = head;
    node_t* pre = nullptr;

    while(cur != nullptr)
    {
        if(cur->move == ODD || cur->move == EVEN){
        
            //create a copy of sublist
            InstructionList* sub = new InstructionList;
            sub->head = new node_t;
            sub->head->move = map.find(cur->move)->second->head->move;
            node_t* after = map.find(cur->move)->second->head->next;
            node_t* current = sub->head;
            while(after)
            {
                node_t* n_node = new node_t;
                n_node->move = after->move;
                current->next = n_node;
                current = n_node;
                after = after->next;
            }
            sub->tail = current;
            //---- finish creating a copy of the sublist ---->

            node_t* temp = cur;
            if(cur != tail)
            {
                sub->tail->next = cur->next;
            }
            //if current node == tail node of main list
            else{ 
                sub->tail->next = nullptr;
                tail = sub->tail; //update the tail
            }

            if(cur == head)       //if current node is head
            {
                head = sub->head; //update head of main list = head of sublist
            }
            else 
            {
                pre->next = sub->head;  //else point pre next to sublist head
            }
            cur = sub->tail; //update cur = tail of sublist
            delete (temp);
        }
        pre = cur;
        cur = cur->next;
    }
}

//Destructor
InstructionList::~InstructionList()
{
    node_t* temp = head;
    while(head != nullptr)
    {
        temp = head;
        head = head->next;
        delete temp; 
    }
}