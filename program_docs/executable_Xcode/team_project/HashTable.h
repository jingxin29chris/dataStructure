//
//  HashTable.h
//
//  Group 3- Project
//  c++
//
//  Programmer: Ruichun Chen, Xin Jing, Wenhui Li, Wenzhe Xu, De Yi Huang
//  IDE: xcode/VS
//  HashTable has the LinkedList, and do the rehash, getTableSize, getBucketCount, getCollision, getLoadFactor, insertHashTable, deleteHashTable, searchHashTable, printHashTable.

//  In this program:
//  1,insertHashTable function: use to insert the zipCode * and update the bucket number and collision number
//  2, deleteHashTable function: this function delete the item from hashtable and update the bucket number and collision number.
//  3, searchHashTable function: search the item from hashTable.
#ifndef hashTable_h
#define hashTable_h
#include "LinkedList.hpp"
#include <iomanip>
const double MAX_COLLISION = 0.75;

template<class ItemType>
class hashTable
{
private:
    int size;
    LinkedList<ItemType> *array;
    int bucketCount; //num of buckets
    int collision;

    void _rehash(int compareByZip(ItemType z1, ItemType z2), int hash(ItemType zip, int tableSize));
    int _getNextPrime(int value);
    bool _isPrime(int value);
public:
    hashTable(){ size = _getNextPrime(10); array = new LinkedList<ItemType>[size]; bucketCount = 0; collision = 0; } ///***
    ~hashTable(){ destroyHashTable(); }
    bool isEmpty() const { return bucketCount == 0; }
    int getTableSize() const { return size; }
    int getBucketCount() const {return bucketCount; }
    int getCollision() const { return collision; }
    double getLoadFactor() const { return ( (double)bucketCount / size);}
    void insertHashTable(const ItemType &item, int compareByZip(ItemType z1, ItemType z2), int hash(ItemType zip, int tableSize));
    bool deleteHashTable(const ItemType &item, int hash(ItemType zip, int tableSize), int compareByZip(ItemType z1, ItemType z2));
    bool searchHashTable(const ItemType &target, ItemType &dataout, int compareByZip(ItemType z1, ItemType z2),int hash(ItemType zip, int tableSize));
    void printHashTable(void visit(ItemType &));
    void destroyHashTable();
};

/***************************************************************
this function insert the item into the hashTable and update
the bucket number and collision number. if the collision greater
than max collision, rehash function will be called to reallocated
a new hash table.
******************************************************************/
template<class ItemType>
void  hashTable<ItemType>::insertHashTable(const ItemType &item, int compareByZip(ItemType z1, ItemType z2), int hash(ItemType zip, int tableSize))
{
    //if(getLoadFactor() > MAX_COLLISION)
       // _rehash(compareByZip, hash);
    int addr = hash(item, size);
    if (array[addr]. getCount() == 0)
        bucketCount++;
    else
        collision++;
    array[addr].insertNode(item,compareByZip); ///***
    if(getLoadFactor() > MAX_COLLISION)
        _rehash(compareByZip, hash);
}

/***************************************************************
this function delete the item from hashtable and update the
 bucket number and collision number.
******************************************************************/
template<class ItemType>
bool hashTable<ItemType>::deleteHashTable(const ItemType &item, int hash(ItemType zip, int tableSize), int compareByZip(ItemType z1, ItemType z2))
{
    int addr = hash(item, size);
    bool success = array[addr].deleteNode(item, compareByZip);
    if (array[addr].getCount() == 0)
    {
        bucketCount--;
    }
    if (array[addr].getCount() > 0 && success)
    {
        collision--;
    }
    return success;
}

/***************************************************************
this function search the item from hashTable. if found ,return
true and pass out the target information by using the reference
parameter. Else, return false.
******************************************************************/

template<class ItemType>
bool hashTable<ItemType>::searchHashTable(const ItemType &target, ItemType &dataout, int compareByZip(ItemType z1, ItemType z2), int hash(ItemType zip, int tableSize))
{
    int addr = hash(target, size);
    if(array[addr].searchList(target, dataout,compareByZip))
        return true;
    else
        return false;
}

/********************************************************
this function print the buckest which alrady occupied
 by the item in the hashtable.
***********************************************************/
template<class ItemType>
void hashTable<ItemType>::printHashTable(void visit(ItemType &item))
{
    //cout << "======= ============== ==============  ================" << endl;
    //cout << "Zipcode      City          County      Total Population" << endl;
    //cout << "======= ============== ==============  ================" << endl;
    for ( int i = 0; i < size; i++)
    {
        if ( array[i].getCount() != 0 )
        {
            cout << "Bucket [" << i << "]: " << endl;
            array[i].traverseList(visit);
        }
    }
    cout << "======= ============== ==============  ================" << endl << endl;
    //cout << "=============== " << endl;
    //cout << "Hash Statstics: " << endl;
    //cout << "=============== " << endl;
    //cout << "Node count: " << bucketCount << endl;
    //cout << "Collision count: " << collision << endl;
    //cout << "Load factor: " << setprecision(2) << getLoadFactor() << endl;
    //cout << "===============" << endl << endl;
}

/********************************************************
this function check the number is prime or not. if prime
 return true. else, return false.
***********************************************************/
template<class ItemType>
bool hashTable<ItemType>::_isPrime(int value)
{
    for (int i = 2; i * i <= value; i++)
        if (value % i == 0)  // value is odd num, check the odd value is prime or not.
            return false;
  return true;
}

/***************************************************************
this function accept an even integer( double input file line or
double hashTable size) as argument and return the next prime number.
******************************************************************/
template<class ItemType>
int hashTable<ItemType>::_getNextPrime(int value)
{
    int nextPrime = value + 1; // value = 2* size ,which is even num.
    while (!_isPrime(nextPrime))
        nextPrime += 2; // nextPrime always be the odd num, because even num is not prime.
    return nextPrime;
}

/********************************************************
The funtion rehash the hashtable which from old smaller
 one to new larger one to void the max collision.
***********************************************************/
template<class ItemType>
void hashTable<ItemType>::_rehash(int compareByZip(ItemType z1, ItemType z2), int hash(ItemType zip, int tableSize))
{
    LinkedList<ItemType>* oldArray = array;
    int oldSize = size;
    //cout << "DEBUG:*REHASH" << endl;
    size = _getNextPrime(size * 2);
    collision = 0;
    bucketCount = 0;
    array = new LinkedList<ItemType>[size];
    for (int i = 0; i < oldSize; i++)
    {
        LinkedList<ItemType> list = oldArray[i];
        ListNode<ItemType>* pCur = list.getHead()->getNext();
        while (pCur != nullptr)
        {
            insertHashTable(pCur->getItem(), compareByZip, hash);
            pCur = pCur->getNext();
        }
    }
    delete [] oldArray;
}

/********************************************************
this function destroy hashtable
***********************************************************/
template<class ItemType>
void hashTable<ItemType>::destroyHashTable()
{
    for ( int i = 0; i < 53; i++)
        array[i].destroyList();
    bucketCount = 0;
    collision = 0;
        //delete array[i];
}
#endif /* hashTable_h */
