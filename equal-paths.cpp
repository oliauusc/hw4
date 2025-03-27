#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool checkLength(Node *root, int count, int& comp);
// bool checkChildren(Node *root);


bool equalPaths(Node * root)
{
    // Add your code below
  int comp = -1;
  if (root == nullptr) return true;  
  return checkLength(root, 0, comp);
}

bool checkChildren(Node *root){
  if ((root->left == nullptr) && (root->right != nullptr))
    return true;
  else if ((root->left != nullptr) && (root->right == nullptr))
    return true;
  return false;
}

bool checkLength(Node *curr, int curcount, int& comp) {
  if (curr == nullptr) {
    return (true);  }

  if ((curr->left == nullptr) && (curr->right==nullptr)){
    if (comp == -1) {
      comp = curcount;
      return true;
    }
    else return (comp == curcount);
  }
  // if (checkChildren(curr)) return false;

  return (checkLength(curr->left, curcount+1, comp) && checkLength(curr->right, curcount+1, comp));
}


