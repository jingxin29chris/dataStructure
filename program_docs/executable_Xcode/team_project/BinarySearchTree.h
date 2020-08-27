// Binary Search Tree ADT
//
//  Group 3- Project
//  c++
//
//  Programmer: Ruichun Chen, Xin Jing, Wenhui Li, Wenzhe Xu, De Yi Huang
//  IDE: xcode/VS
//  BinarySearchTree inheritances from BinaryTree, and do the insert, delete, remove,removLeftMostNode, findNode, findSmallest, findLargest.

//  In this program:
//  1,insert function: use to insert the zipCode * into the tree. Add the function pointer into this function.
//  2, remove function: the original code would not workable for the pointer because compare two pointer. Use the function pointer to make the compare workable.
//  3, getEntry function: it used to find the target item and store the item in vector. As it may have duplicate items and we do not know how many it is. We prefer to use the vector to store them.

#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE

#include "BinaryTree.h"
#include <vector>

template <class ItemType> class BinarySearchTree : public BinaryTree<ItemType> {
private:
   // internal insert node: insert newNode in nodePtr subtree
   void _insert(BinaryNode<ItemType> *nodePtr, BinaryNode<ItemType> *newNode,
      int compare(ItemType, ItemType));

   // internal remove node: locate and delete target node under nodePtr subtree
   BinaryNode<ItemType> *_remove(BinaryNode<ItemType> *nodePtr,
      const ItemType target, bool &success, int compare(ItemType, ItemType));

   // delete target node from tree, called by internal remove node
   BinaryNode<ItemType> *deleteNode(BinaryNode<ItemType> *targetNodePtr);

   // remove the leftmost node in the left subtree of nodePtr
   BinaryNode<ItemType> *removeLeftmostNode(BinaryNode<ItemType> *nodePtr,
      ItemType &successor);

   // search for target node
   void _findNode(BinaryNode<ItemType> *treePtr, const ItemType &target,
      int compare(ItemType, ItemType), std::vector<ItemType>& output) const;

   // search for the smallest node
   BinaryNode<ItemType> *_findSmallest(BinaryNode<ItemType> *treePtr);

   // search for the largest node
   BinaryNode<ItemType> *_findLargest(BinaryNode<ItemType> *treePtr);

public:
   // insert a node at the correct location
   bool insert(const ItemType &newEntry, int compare(ItemType, ItemType)) override;
   // remove a node if found
   bool remove(const ItemType &anEntry,int compare(ItemType, ItemType)) override;
   // find a target node
   // output_length = 0 means no matching data
   // output_data is allocated in this function and must be deleted by caller
   void getEntry(const ItemType &target, int compare(ItemType, ItemType),
      std::vector<ItemType>& output) override;
   //    // find the smallest node
   //    bool findSmallest(ItemType &);
   //    // find the largest node
   //    bool findLargest(ItemType &);
};

///////////////////////// public function definitions
//////////////////////////////
// Inserting items within a tree
template <class ItemType>
bool BinarySearchTree<ItemType>::insert(const ItemType &newEntry,
   int compare(ItemType, ItemType)) {
   if (this->rootPtr == nullptr) {
      this->rootPtr = new BinaryNode<ItemType>(newEntry);
      this->count++;
      // cout << this->count << this->rootPtr->getItem() << endl;
   }
   else {
      BinaryNode<ItemType> *newNodePtr = new BinaryNode<ItemType>(newEntry);
      _insert(this->rootPtr, newNodePtr, compare);
   }
   return true;
}

// Removing items within a tree
template <class ItemType>
bool BinarySearchTree<ItemType>::remove(const ItemType &target, int compare(ItemType, ItemType)) {
   bool isSuccessful = false;
   this->rootPtr = _remove(this->rootPtr, target, isSuccessful, compare);
   return isSuccessful;
}

// Finding entries within a tree
template <class ItemType>
void BinarySearchTree<ItemType>::getEntry(const ItemType &target, int compare(ItemType, ItemType),
   std::vector<ItemType>& output) {
   _findNode(this->rootPtr, target, compare, output);
}

////Finding the smallest
// template<class ItemType>
// bool BinarySearchTree<ItemType>::findSmallest(ItemType &returnItem){
//    BinaryNode<ItemType> *node = _findSmallest(this->rootPtr);
//    if(node != nullptr){
//        returnItem = node->getItem();
//        return true;
//    }
//    return false;
//}
//
////Finding the largest
// template<class ItemType>
// bool BinarySearchTree<ItemType>::findLargest(ItemType &returnItem){
//    BinaryNode<ItemType> *node = _findLargest(this->rootPtr);
//    if(node != nullptr){
//        returnItem = node->getItem();
//        return true;
//    }
//    return false;
//}

//////////////////////////// private functions
///////////////////////////////////////////////

// Implementation of the insert operation
template <class ItemType>
void BinarySearchTree<ItemType>::_insert(BinaryNode<ItemType> *nodePtr,
   BinaryNode<ItemType> *newNodePtr,
   int compare(ItemType, ItemType)) {
   if (compare(newNodePtr->getItem(), nodePtr->getItem()) < 0 ) {
      if (nodePtr->getLeftPtr() != nullptr) {
         _insert(nodePtr->getLeftPtr(), newNodePtr, compare);
      }
      else {
         nodePtr->setLeftPtr(newNodePtr);
         this->count++;
         // cout << this->count << nodePtr->getItem() << endl;
      }
   }
   else {
      if (nodePtr->getRightPtr() != nullptr) {
         _insert(nodePtr->getRightPtr(), newNodePtr, compare);
      }
      else {
         nodePtr->setRightPtr(newNodePtr);
         this->count++;
         // cout << this->count << endl;
      }
   }
}

// Implementation for the search operation
template <class ItemType>
void BinarySearchTree<ItemType>::_findNode(BinaryNode<ItemType> *nodePtr,
   const ItemType &target,
   int compare(ItemType, ItemType),
   std::vector<ItemType>& output) const {
   BinaryNode<ItemType> *pCur; // = new BinaryNode<ItemType>;
   pCur = nodePtr;
   ItemType tar = target; // constant
   while (pCur) {
      int r = compare(tar, pCur->getItem());
      if (r < 0)
         pCur = pCur->getLeftPtr();
      else if (r > 0)
         pCur = pCur->getRightPtr();
      else {
         output.push_back(pCur->getItem());
         pCur = pCur->getRightPtr();
      }
   }
}

// Implementation of the find the smallest node operation
template <class ItemType>
BinaryNode<ItemType> *
BinarySearchTree<ItemType>::_findSmallest(BinaryNode<ItemType> *root) {
   BinaryNode<ItemType> *pCur;
   if (root->getLeftPtr() != nullptr) {
      pCur = _findSmallest(root->getLeftPtr());
      return pCur;
   }
   else
      return root;
}

// Implementation of the find the largest node operation
template <class ItemType>
BinaryNode<ItemType> *
BinarySearchTree<ItemType>::_findLargest(BinaryNode<ItemType> *root) {
   BinaryNode<ItemType> *pCur;
   if (root->getRightPtr() != nullptr) {
      pCur = _findSmallest(root->getRightPtr());
      return pCur;
   }
   else
      return root;
}

////////////////////////////////////////////////////////////////////////
// The follwoing functions will be needed for the team project
// Implementation of the remove operation
template <class ItemType>
BinaryNode<ItemType> *
BinarySearchTree<ItemType>::_remove(BinaryNode<ItemType> *nodePtr,
   const ItemType target, bool &success, int compare(ItemType, ItemType))

{
   if (nodePtr == 0) {
      success = false;
      return 0;
   }
   int r = compare(target,nodePtr->getItem());
   if ( r<0)
      nodePtr->setLeftPtr(_remove(nodePtr->getLeftPtr(), target, success,compare));
   else if( r>0)
      nodePtr->setRightPtr(_remove(nodePtr->getRightPtr(), target, success,compare));
   else {
      nodePtr = deleteNode(nodePtr);
      success = true;
   }
   return nodePtr;
}
// Implementation of the delete operation
template <class ItemType>
BinaryNode<ItemType> *
BinarySearchTree<ItemType>::deleteNode(BinaryNode<ItemType> *nodePtr) {
   if (nodePtr->isLeaf()) {
      delete nodePtr;
      nodePtr = 0;
      return nodePtr;
   }
   else if (nodePtr->getLeftPtr() == 0) {
      BinaryNode<ItemType> *nodeToConnectPtr = nodePtr->getRightPtr();
      delete nodePtr;
      nodePtr = 0;
      return nodeToConnectPtr;
   }
   else if (nodePtr->getRightPtr() == 0) {
      BinaryNode<ItemType> *nodeToConnectPtr = nodePtr->getLeftPtr();
      delete nodePtr;
      nodePtr = 0;
      return nodeToConnectPtr;
   }
   else {
      ItemType newNodeValue;
      nodePtr->setRightPtr(
         removeLeftmostNode(nodePtr->getRightPtr(), newNodeValue));
      nodePtr->setItem(newNodeValue);
      return nodePtr;
   }
}
// Implementation to remove the left leaf
template <class ItemType>
BinaryNode<ItemType> *
BinarySearchTree<ItemType>::removeLeftmostNode(BinaryNode<ItemType> *nodePtr,
   ItemType &successor) {
   if (nodePtr->getLeftPtr() == 0) {
      successor = nodePtr->getItem();
      return deleteNode(nodePtr);
   }
   else {
      nodePtr->setLeftPtr(removeLeftmostNode(nodePtr->getLeftPtr(), successor));
      return nodePtr;
   }
}
#endif
