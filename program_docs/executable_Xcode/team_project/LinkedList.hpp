// Specification file for the LinkedList class
//
//  Group 3- Project
//  c++
//
//  Programmer: Ruichun Chen, Xin Jing, Wenhui Li, Wenzhe Xu, De Yi Huang
//  IDE: xcode/VS
//  LinkedList can do the traverse the data from List, and print the data in the same bucket

//  In this program:
//  1,traverseList function: use to zipCode * to get the zipCode in the LinkedList
//  2, searchList function: the function is used to search the data by the input zipCode


#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "ListNode.hpp"

template<class ItemType>
class LinkedList
{
    private:
        ListNode<ItemType>* head;
        int count;
    public:
        LinkedList();   // Constructor
        //~LinkedList();  // Destructor

        // Linked list operations
        ListNode<ItemType>* getHead() const { return head; }
        int getCount() const {return count;}
        void insertNode(const ItemType &, int compareByZip(ItemType z1, ItemType z2));
        bool deleteNode(ItemType , int compareByZip(ItemType z1, ItemType z2));
        void traverseList(void visit(ItemType &)) const;
        //void displayList() const;
        bool searchList(const ItemType &target, ItemType &dataout, int compareByZip(ItemType z1, ItemType z2)) const;
        void destroyList();
};
//**************************************************
// Constructor
// This function allocates and initializes a sentinel node
//      A sentinel (or dummy) node is an extra node added before the first data record.
//      This convention simplifies and accelerates some list-manipulation algorithms,
//      by making sure that all links can be safely dereferenced and that every list
//      (even one that contains no data elements) always has a "first" node.
//**************************************************
template<class ItemType>
LinkedList<ItemType>::LinkedList()
{
    head = new ListNode<ItemType>;
    //head->next = NULL;
    head->setNext(NULL);
    count = 0;
}

//**************************************************
// displayList shows the value
// stored in each node of the linked list
// pointed to by head.
//**************************************************
template<class ItemType>
void LinkedList<ItemType>::traverseList(void visit(ItemType &item)) const
{
    ListNode<ItemType>* cur = head->getNext();
    ItemType temp;
    while (cur != nullptr)
    {
        temp = cur->getItem();
        visit(temp);
        cur = cur->getNext();
    }
}

//**************************************************
// The searchList function looks for a target city
// in the sorted linked list: if found, returns true
// and copied the data in that node to the output parameter
//**************************************************
template<class ItemType>
bool LinkedList<ItemType>::searchList(const ItemType &target, ItemType &dataout, int compareByZip(ItemType z1, ItemType z2)) const
{
    bool found = false;
    ListNode<ItemType> *pCur = head->getNext();
    while(pCur != NULL &&compareByZip(pCur->getItem(), target) < 0)
        pCur = pCur->getNext();
    if(pCur !=NULL && compareByZip(pCur->getItem(), target) == 0)
    {
        dataout = pCur->getItem();
        found = true;
    }
    return found;
}

//**************************************************
// The insertNode function inserts a new node in a
// sorted linked list
//**************************************************
template<class ItemType>
void LinkedList<ItemType>::insertNode(const ItemType &dataIn, int compareByZip(ItemType z1, ItemType z2))
{
    ListNode<ItemType>* newNode;
    ListNode<ItemType>* pPre;
    ListNode<ItemType>* pCur;

    newNode = new ListNode<ItemType>;
    //newNode->city = dataIn;
    newNode->setItem(dataIn);
    pPre = head;
    pCur = head->getNext();
    while(pCur != NULL && compareByZip(pCur->getItem(), dataIn) < 0)
    {
        pPre = pCur;
        pCur = pCur->getNext();
    }

    pPre->setNext(newNode);
    newNode->setNext(pCur);

    count++;
}

//**************************************************
// The deleteNode function searches for a node
// in a sorted linked list; if found, the node is
// deleted from the list and from memory.
//**************************************************
template<class ItemType>
bool LinkedList<ItemType>::deleteNode(ItemType target,  int compareByZip(ItemType z1, ItemType z2))
{
    ListNode<ItemType>* pCur;
    ListNode<ItemType>* pPre;
    bool deleted = false;
    pPre = head;
    pCur = head->getNext();
    while(pCur != NULL && compareByZip(pCur->getItem(), target) < 0)
    {
        pPre = pCur;
        pCur = pCur->getNext();
    }

    if(pCur != NULL && compareByZip(pCur->getItem(), target) == 0)
    {
        pPre->setNext(pCur->getNext());
        delete pCur;
        deleted = true;
        count--;
    }
    return deleted;
}

template<class ItemType>
void LinkedList<ItemType>::destroyList()
{
    ListNode<ItemType>* pCur;
    ListNode<ItemType>* pNext;

    pCur = head->getNext();
    while(pCur != NULL)
    {
        pNext = pCur->getNext();
        delete pCur;
        pCur = pNext;
    }
    delete head;
}
//**************************************************
// Destructor                                      *
// This function deletes every node in the list.   *
//**************************************************
/*template<class ItemType>
LinkedList<ItemType>::~LinkedList()
{
    ListNode<ItemType>* pCur;
    ListNode<ItemType>* pNext;

    pCur = head->getNext();
    while(pCur != NULL)
    {
        pNext = pCur->getNext();
        //cout << "DEBUG - Destructor: Now deleting " << pCur->getItem() << endl;
        delete pCur;
        pCur = pNext;
    }
    //cout << "DEBUG - Destructor: Now deleting the sentinel node: " << head->getZip() << endl;
    delete head;
}*/

#endif
