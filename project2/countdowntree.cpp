#include <iostream>
#include "countdowntree.h"
using namespace std;

/*
    optional: rotates this Node left or right 
    source: lecture 11
    driver: Oanh Le 
    navigator: Saishriya Pathak
*/
void Node::rotateLeft()
{
    parent->right = left; 
    if(left)
        left->parent = parent;    
    left = parent;
    
    if(parent->parent)
    {
        parent = parent->parent;
        if(left->isLeftChild())
            parent->left = this;
        else if (left->isRightChild())
            parent->right = this;
    }
    else
        parent = nullptr;
    left->parent = this;   
}

void Node::rotateRight()
{
    parent->left = right; 
    if(right)
        right->parent = parent;    
    right = parent;

    if(parent->parent)
    {
        parent = parent->parent;
        if(right->isLeftChild())
            parent->left = this;
        else if (right->isRightChild())
            parent->right = this;
    }
    else 
        parent = nullptr;
    right->parent = this;
}

/*
    optional: returns whether or not this Node is a left or right child of its parent
    driver: Oanh Le 
    navigator: Saishriya Pathak
*/
bool Node::isLeftChild() const
{
    if(!parent)
        return false;
    return (data < parent->data);
}
bool Node::isRightChild() const
{
    if(!parent)
        return false;
    return(data > parent->data);
}

/*
    optional: calculates the height of this Node based on its children
    driver: Oanh Le 
    navigator: Saishriya Pathak
*/
void Node::updateHeight()
{
    if(left && right)
        height = 1 + std::max(left->height, right->height);
    else if(!left && right)
        height = right->height + 1;
    else if(!right && left)
        height = left->height + 1;
    else
        height = 1;
}

/*
    required: recursively restores the balance of the tree
    driver: Oanh Le 
    navigator: Saishriya Pathak
*/
void Node::rebalance()
{
    /*--post-order traversal--*/
    //recur in the left subtree
    if(left)
        left->rebalance();
    //recur on the right subtree
    if(right)
        right->rebalance();

    //deal with the node- base case
    //calculate balance from left and right height
    int balance;
    if(!right && !left) //leaf
        balance = 0;
    else if (!right && left)
        balance = 0 - left->height;
    else if (!left && right)
        balance = right->height;
    else if (right && left)
        balance = right->height - left->height;

    //rotation case
    if(balance >= 2 || balance <= -2)
    {
        int child_balance;

        //right heavy
        if(balance >= 2)
        {
            //calculate child balance
            child_balance = (right->left) ? right->right->height - right->left->height : right->height;
            //RR: ++ +0
            if(child_balance >= 0)
            {
                right->rotateLeft();
                //update height
                this->updateHeight();
                parent->right->updateHeight();
                parent->updateHeight();
            }
            //RL: +-
            else if(child_balance < 0)
            {
                right->left->rotateRight();
                right->rotateLeft();
                //update height
                this->updateHeight();
                parent->right->updateHeight();
                parent->updateHeight();
            }
        }

        //left heavy
        else if (balance <= -2)
        {
            //calculate child balance
            child_balance = (left->right) ? left->right->height - left->left->height : 0 - left->height;
    
            //LL: -- -0
            if(child_balance <= 0)
            {
                left->rotateRight();
                //update height
                this->updateHeight();
                parent->left->updateHeight();
                parent->updateHeight();
            }
            //LR: -+
            else if(child_balance > 0)
            {
                left->right->rotateLeft();
                left->rotateRight();
                //update height
                this->updateHeight();
                parent->left->updateHeight();
                parent->updateHeight();
            }
        }
    }
    //if no rotation needed (0,+/-1)
    else
        updateHeight();
}

/*
    optional: decrements the countdown and rebalances the tree
    driver: Oanh Le 
    navigator: Saishriya Pathak
*/
void CountdownTree::countdown()
{
    count--;
    if(count == 0)
    {
        root->rebalance();
        //update root
        while(root->parent)
            root = root->parent;
        //update count
        count = (getHeight() > MINIMUM_COUNTDOWN)? getHeight() : MINIMUM_COUNTDOWN;
    }
}

/*
    optional: adds the given value to the tree 
    source: lecture 8
    driver: Oanh Le
    navigator: Saishriya Pathak
*/
void Node::insert(int x)
{
    if(x > data)
    {
        if(right)
            right->insert(x);
        else
        {
            right = new Node(x);
            right->parent = this;
        }
    }
    if(x < data)
    {
        if(left)
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
/*
    optional: returns a pointer to the Node with the largest or smallest value in this subtree
    source: lecture 8
    driver: Oanh Le
    navigator: Saishriya Pathak
*/
Node* Node::max(){
    if (!right)
        return this;  
    else
        return right->max();
}

Node* Node::min(){
    if (!left)
        return this;  
    else
        return left->min();
}

/*
    required: removes the given value to the tree
    source: lecture 10 - bst-soln.cpp
    driver: Saishriya Pathak
    navigator: Oanh Le
*/

Node* Node::remove()
{  
   int numChildren = (left != nullptr) + (right != nullptr);  //Separate cases based on # of children  
   if (numChildren == 0)  
    {    
        if (isLeftChild())      
            parent->left = nullptr;    
        else if (isRightChild())      
            parent->right = nullptr;    
        delete this;    
        return nullptr; //tree has no root  
    }  
    else if (numChildren == 1)  
    {    
      Node* child;    
        if (left != nullptr)      
          child = left;    
        else      
          child = right;    //Point child to parent    
          child->parent = parent;    //Point parent to child    
        if (isLeftChild())      
            parent->left = child;    
        else if (isRightChild())      
            parent->right = child;    
        left = right = nullptr;  
        delete this;    
        return child; //child is now root  
    }  
    else //2 children  
    {    
        Node* swap = left->max();    
        data = swap->data;    
        swap->remove(); //recursively delete other node    
        return this; //still the root of the subtree  
    }
 }

void CountdownTree::remove(int x)
{
    if (root == nullptr)    
      return;
   Node* victim = root->search(x);  
      if (victim == nullptr)    
        return; //doesn't contain x  
      if (victim == root) //update root if deleting    
         root = victim->remove();   
     else    
        victim->remove();
    size--;
    countdown();
}

/*
    required:  returns a pointer to the Node with the given value
    source: lecture 8
    driver: Oanh Le
    navigator: Saishriya Pathak
*/
Node* Node::search(int x){  
    if (x == data)    
        return this;  
    else if (x < data)  
    {    
        if (left != nullptr)      
            return left->search(x);    
        else      
            return nullptr;  
    }  
    else  {
        if (right != nullptr)      
            return right->search(x);    
        else      
            return nullptr;  
    }
}
/*
    required:  returns whether the tree contains the given value
    source: lecture 10 - bst-soln.cpp
    driver: Saishriya Pathak
    navigator: Oanh Le
*/
bool CountdownTree::contains(int x) const
{
   if (root == nullptr)
     return false;
   else if (root->search(x) != nullptr)
     return true;
   else
     return false;
}

