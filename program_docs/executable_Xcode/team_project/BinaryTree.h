// Binary tree abstract base class
//
//  Group 3- Project
//  c++
//
//  Programmer: Ruichun Chen, Xin Jing, Wenhui Li, Wenzhe Xu, De Yi Huang
//  IDE: xcode/VS
//  BinaryTree can do the traverse the code from tree, print the zipCode pointer inOrder, preOrder, postOrder, breathOrder, levelOrder, insert, delete, getEntry

//  In this program:
//  1,inOrder function: use to zipCode * to get the zipCode and cityName in the tree, and print out the data in zipCode and cityName sort in ascending order
//  2, levelOrder function: the function is used to print out the indented tree, by using the Queue


#ifndef _BINARY_TREE
#define _BINARY_TREE

#include "BinaryNode.h"
#include "Queue.h"
#include <vector>

template<class ItemType>
class BinaryTree
{
protected:
   BinaryNode<ItemType>* rootPtr;		// ptr to root node
   int count;							// number of nodes in tree

public:
   // "admin" functions
   BinaryTree() { rootPtr = 0; count = 0; }
   BinaryTree(const BinaryTree<ItemType> & tree) = delete;
   BinaryTree& operator=(const BinaryTree&) = delete;

   virtual ~BinaryTree() { destroyTree(rootPtr); }

   // common functions for all binary trees
   bool isEmpty() const { return count == 0; }
   int size() const { return count; }
   void clear() { destroyTree(rootPtr); rootPtr = nullptr; count = 0; }
   template <typename F>
   void preOrder(F&& visit) const { _preorder(visit, rootPtr); }

   template <typename F>
   void inOrder(F&& visit) const { _inorder(visit, rootPtr); }

   template <typename F>
   void postOrder(F&& visit) const { _postorder(visit, rootPtr); }
   // Tree Breadth-First Traversals
   void breadthOrder(void visit(ItemType &)) const;
   // Tree Level-First Traversals
   void levelOrder(void visit(ItemType &), int level = 0)const { _levelorder(visit, rootPtr, level); }

   // abstract functions to be implemented by derived class
   virtual bool insert(const ItemType & newData, int compare(ItemType, ItemType)) = 0;
   virtual bool remove(const ItemType & data, int compare(ItemType, ItemType)) = 0;
   virtual void getEntry(const ItemType &target, int compare(ItemType, ItemType),
      std::vector<ItemType>& output) = 0;

private:
   // delete all nodes from the tree
   void destroyTree(BinaryNode<ItemType>* nodePtr);

   // internal traverse
   template <typename F>
   void _preorder(F&& visit, BinaryNode<ItemType>* nodePtr) const {
      if (nodePtr != 0)
      {
         ItemType item = nodePtr->getItem();
         visit(item);
         _preorder(visit, nodePtr->getLeftPtr());
         _preorder(visit, nodePtr->getRightPtr());
      }
   }
   template <typename F>
   void _inorder(F&& visit, BinaryNode<ItemType>* nodePtr) const {
      if (nodePtr != 0)
      {
         _inorder(visit, nodePtr->getLeftPtr());
         ItemType item = nodePtr->getItem();
         visit(item);
         _inorder(visit, nodePtr->getRightPtr());
      }
   }
   template <typename F>
   void _postorder(F&& visit, BinaryNode<ItemType>* nodePtr) const {
      if (nodePtr != 0)
      {
         _postorder(visit, nodePtr->getLeftPtr());
         _postorder(visit, nodePtr->getRightPtr());
         ItemType item = nodePtr->getItem();
         visit(item);
      }
   }
   void _levelorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr, int) const;
};

//Destroy the entire tree
template<class ItemType>
void BinaryTree<ItemType>::destroyTree(BinaryNode<ItemType>* nodePtr)
{
   BinaryNode<ItemType>* pCur = nodePtr;
   ItemType item = pCur->getItem();
   if (pCur == nullptr)
      return;
   if (pCur->getLeftPtr() != nullptr)
      destroyTree(pCur->getLeftPtr());
   if (pCur->getRightPtr() != nullptr)
      destroyTree(pCur->getRightPtr());
   delete pCur;
}

//BreathOrder Traversal
template<class ItemType>
void BinaryTree<ItemType>::breadthOrder(void visit(ItemType &)) const
{
   BinaryNode<ItemType> *pCur; //= rootPtr;
   if (rootPtr != nullptr)
   {
      Queue<BinaryNode<ItemType>*> q;
      q.enqueue(rootPtr);
      while (!q.isEmpty()) {
         q.dequeue(pCur);
         ItemType item = pCur->getItem();
         visit(item);
         if (pCur->getLeftPtr()) {
            q.enqueue(pCur->getLeftPtr());
         }
         if (pCur->getRightPtr()) {
            q.enqueue(pCur->getRightPtr());
         }
      }
   }
}

//LevelOrder Traversal
template<class ItemType>
void BinaryTree<ItemType>::_levelorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr, int level) const
{
   level++;
   if (nodePtr != 0)
   {
      for (int i = 0; i < level - 1; i++) {
         cout << "\t";
      }
      ItemType item = nodePtr->getItem();
      cout << "Level " << level << ".";
      visit(item);
      _levelorder(visit, nodePtr->getRightPtr(), level);
      _levelorder(visit, nodePtr->getLeftPtr(), level);
   }
}
#endif

