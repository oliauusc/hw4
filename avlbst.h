#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rightRotate(AVLNode<Key,Value>* root);
    void leftRotate(AVLNode<Key,Value>* root);
    void leftRightRotate(AVLNode<Key,Value>* root);
    void rightLeftRotate(AVLNode<Key,Value>* root);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    Key k = (new_item.first);
    Value val = (new_item.second);

  if (!this->root_) {
    this->root_=(new AVLNode<Key, Value>(k, val, nullptr));
    return; 
  }
  
  AVLNode<Key, Value> *curr = static_cast<AVLNode<Key, Value>*>(this->root_);
  AVLNode<Key, Value> *prev;

  while (curr) {
    prev = curr;
    if (k < curr->getKey()) {
      curr = curr->getLeft();
    }
    else if (k > curr->getKey()) {
      curr = curr->getRight();
    }
    else if (k == curr->getKey()) {
      curr->setValue(val);
      return;
    }
  }

     AVLNode<Key, Value> *newNode = new AVLNode<Key, Value>(k, val, nullptr);
    if (k < prev->getKey()) {
      prev->setLeft(newNode); 
    }
    else if (k > prev->getKey()) {
      prev->setRight(newNode);
    }
    newNode->setParent(prev);


    curr = newNode;

while(curr) {
  // if (curr->getParent()) {

    if (curr->getRight() && curr->getLeft())
      curr->setBalance(this->getHeight(curr->getLeft()) - this->getHeight(curr->getRight()));
    else if (curr->getRight())
      curr->setBalance(-1);
    else if (curr->getLeft())
      curr->setBalance(1);

  int state = 0;

  if (curr->getParent()) {
    if (curr->getParent()->getLeft() == curr)
      curr->getParent()->updateBalance(1);
    else curr->getParent()->updateBalance(-1);
    state = curr->getParent()->getBalance();
  }
  

  if (state == 0)
    break;

  else if (state == -1 || state == 1) {
    curr = curr->getParent();

    continue;
  }
  
  if (state > 1) {

    if (curr->getBalance() == 1) //right is taller 
      rightRotate(curr->getParent());
    else if (curr->getBalance() == -1) 
       leftRightRotate(curr->getParent());

    break;
  } 
  if (state < -1) {
     if (curr->getBalance() == -1) //left is taller 
      leftRotate(curr->getParent());
    else if (curr->getBalance() == 1) //
      rightLeftRotate(curr->getParent());

    break;
  }
    // curr->setBalance(curr->getBalance() + (k < curr->getKey() ? 1 : -1));
    // curr = curr->getParent();
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value> *curr = nullptr;
    AVLNode<Key, Value> *found = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (!found) {
      if (!this->root_)return;
    
      curr = static_cast<AVLNode<Key, Value>*>(this->root_);
        // curr->setBalance(this->getHeight(curr->getLeft()) - this->getHeight(curr->getRight()));

      if (curr->getBalance() > 1) {
      while (curr->getLeft())
        curr = curr->getLeft();
        }
        else {
      while (curr->getRight())
        curr = curr->getRight();
        } 

      if (curr->getParent())
        curr = curr->getParent();

    }


    else if (found->getRight() && !found->getLeft()) {
      if (found == this->root_) {
        this->root_=(found->getRight());
        (found->getRight())->setParent(nullptr);
      }

      else {
      // nodeSwap(found, found->getRight());

      // if (found->getParent()) {
        if ((found->getParent())->getRight() == found) {
          (found->getParent())->setRight(found->getRight());
        }
        else (found->getParent())->setLeft(found->getRight());
      //  }
      // found->setParent(nullptr);
      }
      found->getRight()->setParent(found->getParent());
    }

    else if (!found->getRight() && found->getLeft()) {
      if (found == this->root_) {
        this->root_=(found->getLeft());
        (found->getLeft())->setParent(nullptr);
      }

      else {
        // nodeSwap(found, found->getLeft());

      // if (found->getParent()) {
      if ((found->getParent())->getRight() == found) {
        (found->getParent())->setRight(found->getLeft());
      }
      else (found->getParent())->setLeft(found->getLeft());
       }
               found->getLeft()->setParent(found->getParent());

    }

    else if (found->getRight() && found->getLeft()) {
      AVLNode<Key, Value> *pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(found));
      nodeSwap(pred, found);

      if (found->getLeft()) {
        (found->getLeft())->setParent(found->getParent());

        if (!found->getParent()) {
          if (found->getLeft()) this->root_=(found->getLeft());
          else this->root_=(nullptr);
        }
        else if (found->getParent()) {
          if ((found->getParent())->getRight() == found) {
          (found->getParent())->setRight(found->getLeft());
          }
          else (found->getParent())->setLeft(found->getLeft());
        }
        
      }

      else  {
        if (found->getParent()) {
        if ((found->getParent())->getLeft() == found)
          (found->getParent())->setLeft(nullptr);
        else (found->getParent())->setRight(nullptr);
        }
      }
    }

    else if (!found->getRight() && !found->getLeft()) {
      if (found == this->root_) {
        this->root_=(nullptr);

        // if (found->getParent()) curr = found->getParent();

        // delete found;
        // return;
      }

      else {
        if ((found->getParent() != nullptr)) {
          // if ((found->getParent())->getLeft()) {
            if ((found->getParent())->getLeft() == found)
              (found->getParent())->setLeft(nullptr);
            else (found->getParent())->setRight(nullptr);
        }
      }
    }
      // found->setParent(nullptr);
      if (found && !curr && found->getParent()) {
          curr = found->getParent();
      }

      delete found;

      // printRoot(this->root_);
    
while(curr) {
  int state = this->getHeight(curr->getLeft()) - this->getHeight(curr->getRight());
  curr->setBalance(state);

  // if (state == 0) 
  //   break;

  // else if (state == -1 || state == 1) {
  //   curr = curr->getParent();
  //   continue;
  // }
  
  if (state > 1) {
    if (curr->getLeft()->getBalance() >= 0) 
      rightRotate(curr);
    else leftRightRotate(curr);

    // break;
  } 
  if (state < -1) {
    if (curr->getRight()->getBalance() <= 0)
      leftRotate(curr);
    else rightLeftRotate(curr);

    // break;
  }
      curr = curr->getParent();

}
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key,Value>* curr)
{
  // this->printRoot(this->root_);
 if (!curr || !curr->getLeft()) {
    return;
  }
  AVLNode<Key, Value>* right1 = curr->getLeft();
  AVLNode<Key, Value>* right2 = (right1->getRight());

  right1->setRight(curr);
  curr->setLeft(right2);

  if (right2) {
    right2->setParent(curr);
  }

  right1->setParent(curr->getParent());

  if (curr == this->root_) this->root_=(right1);
  else if (curr->getParent()->getLeft() == curr)
    curr->getParent()->setLeft(right1);
  else curr->getParent()->setRight(right1);

  curr->setParent(right1);

  curr->setBalance(this->getHeight(curr->getLeft()) - this->getHeight(curr->getRight()));
  right1->setBalance(this->getHeight(right1->getLeft()) - this->getHeight(right1->getRight()));
} 
    
template<class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key,Value>* curr)
{
  if (!curr || !curr->getRight()) {
    return;
  }

  AVLNode<Key, Value>* left1 = curr->getRight();
  AVLNode<Key, Value>* left2 = (left1->getLeft());

  left1->setLeft(curr);
  curr->setRight(left2);

  if (left2) {
    left2->setParent(curr);
  }
  left1->setParent(curr->getParent());

  if (curr == this->root_) this->root_=(left1);
  else if (curr->getParent()->getRight() == curr)
    curr->getParent()->setRight(left1);
  else curr->getParent()->setLeft(left1);

  curr->setParent(left1);

curr->setBalance(this->getHeight(curr->getLeft()) - this->getHeight(curr->getRight()));
  left1->setBalance(this->getHeight(left1->getLeft()) - this->getHeight(left1->getRight()));

}

template<class Key, class Value>
void AVLTree<Key, Value>::leftRightRotate(AVLNode<Key,Value>* curr)
{
  //  if (!curr || !curr->getLeft()) {
  //   return;
  // }

  leftRotate(curr->getLeft());
  rightRotate(curr);
}

template<class Key, class Value>
void AVLTree<Key, Value>::

rightLeftRotate(AVLNode<Key,Value>* curr)
{
  //  if (!curr || !curr->getRight()) {
  //   return;
  // }

  rightRotate(curr->getRight());
  leftRotate(curr);
}
   
#endif
