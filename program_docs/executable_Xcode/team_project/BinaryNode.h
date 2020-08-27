// Node for a binary tree
//
//  Group 3- Project
//  c++
//
//  Programmer: Ruichun Chen, Xin Jing, Wenhui Li, Wenzhe Xu, De Yi Huang
//  IDE: xcode/VS

//  In this program:
//  BinaryNode uses to save the Zipcode pointer in item, and it has the left and right pointer which points to the next BinaryNode

#ifndef _BINARY_NODE
#define _BINARY_NODE

template<class ItemType>
class BinaryNode
{
private:
	ItemType              item;         // Data portion
	BinaryNode<ItemType>* leftPtr;		// Pointer to left child
	BinaryNode<ItemType>* rightPtr;		// Pointer to right child

public:
	// constructors
	BinaryNode(const ItemType & anItem)	{item = anItem; leftPtr = 0; rightPtr = 0;}
	BinaryNode(const ItemType & anItem,
		       BinaryNode<ItemType>* left,
			   BinaryNode<ItemType>* right) {item = anItem; leftPtr = left; rightPtr = right;}
	// setters
	void setItem(const ItemType & anItem) {item = anItem;}
	void setLeftPtr(BinaryNode<ItemType>* left) {leftPtr = left;}
    void setRightPtr(BinaryNode<ItemType>* right) {rightPtr = right;}
	// getters
	ItemType getItem() const	 {return item;}
	BinaryNode<ItemType>* getLeftPtr() const  {return leftPtr;}
	BinaryNode<ItemType>* getRightPtr() const {return rightPtr;}


    bool isLeaf() const {return (leftPtr == 0 && rightPtr == 0);}

};

#endif
