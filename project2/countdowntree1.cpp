#include <iostream>
#include "countdowntree.h"
using namespace std;

//optional: rotates this Node left or right
//source: lecture 11
void Node::rotateLeft()
{
    parent->right = left; 
    if(left != nullptr)
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
    if(right != nullptr)
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

//optional: returns whether or not this Node is a left or right child of its parent
bool Node::isLeftChild() const
{
    if(parent == nullptr)
        return false;
    return (data < parent->data);
}
bool Node::isRightChild() const
{
    if(parent == nullptr)
        return false;
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

//required: recursively restores the balance of the tree
void Node::rebalance()
{
    /*--post-order traversal--*/

    //recur in the left subtree
    if(left!= nullptr)
        left->rebalance();
    //recur on the right subtree
    if(right != nullptr)
        right->rebalance();

    //deal with the node
    //calculate balance from left and right height
    int balance;
    if(right == nullptr && left == nullptr) //leaf
        balance = 0;
    else if (right == nullptr && left)
        balance = 0 - left->height;
    else if (left == nullptr && right)
        balance = right->height;
    else if (right && left)
        balance = right->height - left->height;

    //rotation case
    if(balance >= 2 || balance <= -2)
    {
        cout << "fix! "; //debug
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

//optional: decrements the countdown and rebalances the tree
void CountdownTree::countdown()
{
    count--;
    cout << count << " "; //debug
    if(count == 0)
    {
        cout << endl << "*----reach countdown ----*" << endl; //debug
        root->rebalance();

        //update root
        while(root->parent)
            root = root->parent;

        //update count
        (getHeight() > MINIMUM_COUNTDOWN)? count = getHeight() : count = MINIMUM_COUNTDOWN;
        
        //debug
        cout << "\ncurrent root = " << root->data << endl;
        cout << "height = " << getHeight() << endl;
        cout << "size = " << getSize() << endl;
        cout << "next count = " << getCount() << endl;
        Node* tmp = root->max(); 
        cout << "max = " << tmp->data << "; h = " << tmp->height << endl;
        tmp = root->min(); 
        cout << "min = " << tmp->data << "; h = " << tmp->height << endl;
    }
}

//Adds the given value to the tree 
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

//optional: returns a pointer to the Node with the largest or smallest value in this subtree
//source: lecture 8
Node* Node::max(){
    if (right == nullptr)
        return this;  
    else
        return right->max();
}

Node* Node::min(){
    if (left == nullptr)
        return this;  
    else
        return left->min();
}

//Removes the given value to the tree
//source: lecture 10 - bst-soln.cpp
void Node* Node::remove()
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
        Node* swap = left->findMax();    
        data = swap->data;    
        swap->remove(); //recursively delete other node    
        return this; //still the root of the subtree  
    }
 }

void CountdownTree::remove(int x)
{
    if (root == nullptr)    
      return;
   CountdownTree Node* victim = root->search(x);  
      if (victim == nullptr)    
        return; //doesn't contain x  
      if (victim == root) //update root if deleting    
         root = victim->remove();   
     else    
        victim->remove();
    size--;
    countdown();
}



//Returns whether the tree contains the given value
//source: lecture 10 - bst-soln.cpp
bool CountdownTree::contains(int x) const
{
   if (root == nullptr)
     return false;
   else if (root -> search(x) != nullptr)
     return true;
   else
     return false;
      
}
