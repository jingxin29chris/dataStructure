//
//  Group 3- Project
//  c++
//
//  Programmer: Ruichun Chen, Xin Jing, Wenhui Li, Wenzhe Xu, De Yi Huang
//  IDE: xcode/VS

//  In this program:
//  ListNode uses to save the Zipcode pointer in item, and it has the next pointer which points to the next ListNode

#ifndef ListNode_H
#define ListNode_H
#include <string>

template<class ItemType>
class ListNode
{
    private:
        ItemType item;         // Data portion
        //string key;
        ListNode<ItemType>* next;
    public:
        //constructors
        ListNode() :next(nullptr) {}
        ListNode (const ItemType &data, std::string s, ListNode<ItemType>* Nptr) {item= data; next = Nptr; }
        //setters
        void setItem(const ItemType & data)  { item = data; }
        void setNext(ListNode<ItemType>* Nptr) { next = Nptr; }
        //void setKey(const string s) {key = s}
        //getters
        ItemType getItem () const { return item; }
        ListNode<ItemType>* getNext() const { return next; }
        //string getKey () const { return key; }
};
#endif
