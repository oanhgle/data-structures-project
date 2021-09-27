#include <iostream>
#include "countdowntree.h"
using namespace std;

// optional: rotates this Node left or right
// source: lecture 11
void Node::rotateLeft()
{
    parent->right = left; 
    if(left != nullptr)
        left->parent = parent;    
    left = parent;
    if(parent->parent != nullptr)
    {
        parent = parent->parent;
        if(left->isLeftChild())
            parent->left = this;
        else if (left->isRightChild())
            parent->right = this;
    }
    left->parent = this;
}

void Node::rotateRight()
{
    parent->left = right; 
    if(right != nullptr)
        right->parent = parent;    
    right = parent;
    if(parent->parent != nullptr)
    {
        parent = parent->parent;
        if(right->isLeftChild())
            parent->left = this;
        else if (right->isRightChild())
            parent->right = this;
    }
    right->parent = this;
}

//optional: returns whether or not this Node is a left or right child of its parent
bool Node::isLeftChild() const
{
    return (data < parent->data);
}
bool Node::isRightChild() const
{
    return(data > parent->data);
}

//optional: calculates the height of this Node based on its children
void Node::updateHeight()
{
    if(left != nullptr && right != nullptr)
        this->height = 1 + std::max(left->height, right->height);
    else if(left == nullptr && right != nullptr)
        this->height = right->height + 1;
    else if(right == nullptr && left != nullptr)
        this->height = left->height + 1;
    else
        this->height = 1;
}

//recursively restores the balance of the tree
void Node::rebalance()
{
        
}

//optional: decrements the countdown and rebalances the tree
void CountdownTree::countdown()
{
    count--;
    if(count == 0)
    {
        //cout << "reach 10" << endl; //debug
        root->rebalance();
        (getHeight() >= MINIMUM_COUNTDOWN)? count = getHeight() : count = MINIMUM_COUNTDOWN;
    }
}

//adds the given value to the tree 
//source: lecture 8
void Node::insert(int x)
{
    if(x > data)
    {
        if(right != nullptr)
            right->insert(x);
        else
        {
            right = new Node(x);
            right->parent = this;
        }
    }
    if(x < data)
    {
        if(left != nullptr)
            left->insert(x);
        else
        {
            left = new Node(x);
            left->parent = this;
        }
    }
}

void CountdownTree::insert(int x)
{
    if(size == 0)
        root = new Node(x);
    else 
        root->insert(x);
    size++;
    countdown();
}

//removes the given value to the tree
void CountdownTree::remove(int x)
{

}

//returns whether the tree contains the given value
bool CountdownTree::contains(int x) const
{
    
}
