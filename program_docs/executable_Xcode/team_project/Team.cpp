//
//  Group 3- Project
//  c++
//
//  Programmer: Ruichun Chen, Xin Jing, Wenhui Li, Wenzhe Xu, De Yi Huang
//  IDE: xcode/VS

//  In this program:
//  Ruichun Chen: main(), menu(), compare(), readFile(), dataValid(), leverOrder(), inOrder()
//  Xin Jing: insert(), remove(), findNode(), zipCode pointer, database class, vector, code integration and debug
//  Wenhui Li: constructor, _getPrime(), _isPrime(), _rehash(), insertHashTable(), searchHashTable(), printHashTable(), deleteHashTable(), code integration and debug
//  Wenzhe Xu: goodhash(), badhash(), destrcutor, destroyHashTable(), fomatting
//  De Yi Huang: part of file I/O, structure design

//  The main funciton contains a database class:
//  The class of Database:
//  1,Four member variables: Two BinarySearchTree  tree Zip1, Zip2, One hashTable HashZip and a Stack for undo function.
//  2, Class saveNode and save function: saveNode a function object and used for the output file. Save function use the saveNode function object to implement the file output and clear the stack.
//  3, The constructor of the database class is to load the input data from input file and build the tree and hashTable.
//  4,The destructor is to clear the shared Zip code pointer in the heap and clear the pointer in the stack.
//  5,UndoDelete function: undo the delete in the reverse order of the delete sequence

//  6, deleteZipCode function : delete the tree and hashtable by zipcode and store the delete item in stack
//  7, deleteCityName function: delete the tree and hastable by cityName and store the delete item in stack
#ifdef _WIN32
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "BinarySearchTree.h"
#include "StackADT.h"
#include "Zipcode.h"
#include "HashTable.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

// Constants for the menu choices
const char DELETE_CHOICE = 'D', INSERT_CHOICE = 'F', SEARCH_CHOICE = 'S',
UNDO_CHOICE = 'N', PRIMARY_CHOICE = 'P', SECOND_CHOICE = 'C',
PRIMARY_DELETE = 'M', SECOND_DELETE = 'N', LIST_CHOICE = 'L',
UNSORTED_CHOICE = 'U', PSORTED_CHOICE = 'O', SSORTED_CHOICE = 'R',
SPECIAL_CHOICE = 'Z', HIDDEN_CHOICE = 'H', PRINT_CHOICE = 'T',
WRITE_CHOICE = 'W', STAT_CHOICE = 'I', EXIT_CHOICE = 'E',
MENU_CHOICE = 'A';

// Function prototypes
void menu();
char getValid();
int compareByZip(Zipcode *z1, Zipcode *z2);
int compareByName(Zipcode *z1, Zipcode *z2);
int badHash(Zipcode* zip, int tableSize);
int goodHash(Zipcode* zip, int tableSize);

class Database {
private:
   Stack<Zipcode *> s;

   static void deleteZipPointer(Zipcode * z) {
      delete z;
   }

   class SaveNode {
   private:
      std::ostream& os;
   public:
      SaveNode(std::ostream& os1) :os(os1) {}

      void operator()(Zipcode* z) {
         os << z->getZipCode() << "," << z->getCityName() << "," << z->getCountyName()
            << "," << z->getTotalPopulation() << "    " << z->getAfricanAmerican() << "    "
            << z->getAsian() << "    " << z->getHispanicLatino() << "    " << z->getWhite()
            << endl;
      }
   };
   // clear the stack if save to file or close the program
   void clearStack() {
      Zipcode* z;
      while (s.pop(z)) {
         delete z;
      }
   }
public:
   BinarySearchTree<Zipcode *> zip1;
   BinarySearchTree<Zipcode *> zip2;
   hashTable<Zipcode*> hashZip;

   // save the zip code data to file according to the post order and clear the stack
   void save(const char* filename) {
      std::ofstream os(filename);
      zip1.postOrder(SaveNode(os));
      clearStack();            
   }

   ~Database() {
      //traverse any of the tree to delete the heap allocated Zipcode object
      zip1.postOrder(deleteZipPointer);
      clearStack();
   }

   /*****************************************************************************
 This function reads data about zip code from a given file and inserts them
 into some data structures.
 *****************************************************************************/
   Database(const string &fileName) {
      ifstream inputFile;
      inputFile.open(fileName);
      string zipCode;  // unique key
      string cityName; // secondary key
      string countyName;
      int hispanicLatino;
      int asian;
      int white;
      int africanAmerican;
      int totalPopulation;
      if (!inputFile) {
         throw std::runtime_error("File open error.\n");
      }
      while (inputFile) {
         getline(inputFile, zipCode, ',');
         getline(inputFile, cityName, ',');
         getline(inputFile, countyName, ',');
         inputFile >> totalPopulation >> africanAmerican >> asian >>
            hispanicLatino >> white;
         inputFile.ignore();
         Zipcode *zip = new Zipcode();
         zip->setZipcode(zipCode);
         zip->setcityName(cityName);
         zip->setCountyName(countyName);
         zip->setTotalPopulation(totalPopulation);
         zip->setAfricanAmerican(africanAmerican);
         zip->setAsian(asian);
         zip->setHispanicLatino(hispanicLatino);
         zip->setWhite(white);
         // cout << zip << endl;
         zip1.insert(zip, compareByZip);
         zip2.insert(zip, compareByName);
         hashZip.insertHashTable(zip, compareByZip, goodHash);
      }
      inputFile.close();
   }

   void undoDelete() {
      Zipcode *data;
      while (s.pop(data)) {
         cout << data->getCityName() << endl;
         zip1.insert(data, compareByZip);
         zip2.insert(data, compareByName);
         hashZip.insertHashTable(data, compareByZip, goodHash); ///***
      }
   }
   // delete the tree by zipcode and store the delete item in stack
   void deleteZipCode(const std::string &zip) {
      Zipcode zipObj;
      zipObj.setZipcode(zip);
      std::vector<Zipcode *> data;
      zip1.getEntry(&zipObj, compareByZip, data);
      if (!data.empty()) {
         // data.size() must be 1 as the zipcode is the unique key
         Zipcode *z = data[0];
         s.push(z);
         cout << zip << " deleted!" << endl;
         zip1.remove(z, compareByZip);
         zip2.remove(z, compareByName);
         hashZip.deleteHashTable(z, goodHash, compareByZip); ///***
      }
      else {
         cout << zip << " doesn't exist!" << endl;
      }
      cout << endl;
   }
   // delete the tree by cityName and store the delete item in stack
   void deleteCityName(const std::string &zip) {
      Zipcode zipObj;
      zipObj.setcityName(zip);
      std::vector<Zipcode *> data;
      zip2.getEntry(&zipObj, compareByName, data);
      if (!data.empty()) {
         for (size_t i = 0; i < data.size(); i++) {
            Zipcode *z = data[i];
            s.push(z);
            cout << zip << " deleted!" << endl;
            zip1.remove(z, compareByZip);
            zip2.remove(z, compareByName);
            hashZip.deleteHashTable(z, goodHash,compareByZip ); ///***
         }
          cout << endl;
      }
      else {
         cout << zip << " doesn't exist!" << endl;
      }
   }
};
void printData(Zipcode *&);
void printZip(Zipcode *&);
void printName(Zipcode *&);
void printStatics(Database &d);
void searchHashManager(Zipcode z, Database &d);
string validCode();
string validCity();
Zipcode* getData();
void printTop();
void deleteMenu();
void searchMenu();
void listMenu();

int main() {
#ifdef _WIN32
   _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
   char choice = 'a'; // To hold a menu choice
   char choice1 = 'a';
   char choice2 = 'a';
   string zip, name, deleteData;
   string inputFileName = "/Users/rich_chan/Desktop/team_project/team_project/CIS 22C input.txt";
   Database database(inputFileName);
   menu();
   while (choice != EXIT_CHOICE) {
      switch (choice) {
      case HIDDEN_CHOICE:
      cout << "Group members: Ruichun Chen, Xin Jing, Wenhui Li, Wenzhe Xu, De Yi Huang" << endl << endl;
          break;
      case MENU_CHOICE:
          menu();
          break;
      case INSERT_CHOICE:
         cout << "\nInserting a new element..." << endl;
         Zipcode *data;
         data = getData();
         database.zip1.insert(data, compareByZip);
         database.zip2.insert(data, compareByName);
          database.hashZip.insertHashTable(data, compareByZip, goodHash);
         // please add hash inter function here
         break;
      case DELETE_CHOICE: // traversals
      {
          deleteMenu();
         while (choice1 != EXIT_CHOICE) {
            switch (choice1) {
            case PRIMARY_DELETE:
               cout << "Doing Delete..." << endl;
               zip = validCode();
               database.deleteZipCode(zip);
               break;
            case SECOND_DELETE:
               cout << "Doing Delete..." << endl;
               name = validCity();
               database.deleteCityName(name);
               break;
            case EXIT_CHOICE:
               break;
            default:
               break;
            }
            cout << "Delete Menu: ";
            choice1 = getValid();
         }
         choice1 = 'a';
         break;
      }
      case UNDO_CHOICE:
         cout << "Undoing Delete..." << endl;
         database.undoDelete();
         cout << endl;
         break;
      case SEARCH_CHOICE:
          searchMenu();
         cout << EXIT_CHOICE << "- Exit\n" << endl;
         while (choice1 != EXIT_CHOICE) {
            switch (choice1) {
            case PRIMARY_CHOICE: {
               cout << "\nFinding Data By Zip Code..." << endl;
               zip = validCode();
               Zipcode zipObj;
               zipObj.setZipcode(zip);
               std::vector<Zipcode *> data;
               database.zip1.getEntry(&zipObj, compareByZip, data);
               searchHashManager(zipObj, database);
               if (!data.empty()) {
                  cout << "~~**Data Found in Tree!**~~" << endl;
                  for (size_t i = 0; i < data.size(); i++) {
                     cout << *data[i] << endl;
                  }
               }
               else
                  cout << "~~**Data Not Found in Tree!**~~" << endl;
            }
                break;
            case SECOND_CHOICE: {
               cout << "\nFinding Data By City Name..." << endl;
               name = validCity();
               Zipcode zipObj;
               zipObj.setcityName(name);
               std::vector<Zipcode *> data;
               database.zip2.getEntry(&zipObj, compareByName, data);
               if (!data.empty()) {
                  cout << "Data Found!" << endl;
                  for (size_t i = 0; i < data.size(); i++) {
                     cout << *data[i] << endl;
                  }
               }
               else
                  cout << "Data Not Found!" << endl;
            } break;
            case EXIT_CHOICE:
               break;
            default:
               break;
            }
            cout << "Search Menu: ";
            choice1 = getValid();
         }
         choice1 = 'a';
         break;
      case PRINT_CHOICE: // indented tree
         database.zip1.levelOrder(printData);
         break;
      case LIST_CHOICE: // smallest
         listMenu();
         while (choice2 != EXIT_CHOICE) {
            switch (choice2) {
            case UNSORTED_CHOICE:
               printTop();
               database.hashZip.printHashTable(printData);
               break;
            case PSORTED_CHOICE:
               cout << "Using The Primary Key(zip code):" << endl;
               printTop();
               database.zip1.inOrder(printData);
               cout << endl;
               break;
            case SSORTED_CHOICE:
               cout << "Using The Secondary Key(name):" << endl;
               printTop();
               database.zip2.inOrder(printData);
               cout << endl;
               break;
            case SPECIAL_CHOICE:
               cout << "Using The Indented Tree:" << endl;
               database.zip1.levelOrder(printZip);
               cout << endl << endl;
               break;
            default:
               break;
            }
            cout << "List Menu: ";
            choice2 = getValid();
         }
         choice2 = 'a';
         break;
      case WRITE_CHOICE: // write data
         database.save("/Users/rich_chan/Desktop/team_project/team_project/output.txt");
         break;
      case STAT_CHOICE:
         printStatics(database);
         break;
      case EXIT_CHOICE: // exit destory
         break;
      default:
         break;
      }
      cout << "Main Menu: ";
      choice = getValid();
   }
   cout << "\n\t*** THE END ***\n";
   return 0;
}

/**~*~*
What do you want to do?
What do you want to do?
D - Delete the element from tree
N - Undo the deleted element
S – Search (has a sub-menu)
L - List (has a sub-menu)
W - Write data to a file
I - Statistics
E – Exit
*~**/
void menu() {
   // Display the menu and get the user's choice.
   cout << "\nWhat do you want to do?\n\t"
        << INSERT_CHOICE << " - Inserting a new element\n\t"
        << DELETE_CHOICE << " - Delete the element from tree\n\t"
        << UNDO_CHOICE << " - Undo the deleted element\n\t"
        << SEARCH_CHOICE << " - Search (has a sub-menu)\n\t"
        << LIST_CHOICE << " - List (has a sub-menu)\n\t"
        << WRITE_CHOICE << " - Write data to a file\n\t"
        << STAT_CHOICE << " - Statistics\n\t"
        << MENU_CHOICE << " - Display menu\n\t"
        << EXIT_CHOICE << " - Exit\n";
}

/**~*~*
 This function prompts the user to enter a char within a given range
 If the input is not valid (not a character) it prompts the user to enter a new
 char, until the input is valid, and set the input to uppercase
 *~**/
char getValid() {
   char input;
   do {
      cout << "Enter your choice: ";
      cin >> input;
      cin.ignore();
      if (isalpha(input))
         input = toupper(input);
   } while (input < 'A' || input > 'Z');
   return input;
}

/*****************************************************************************
 Insert manager: check if the input of city is correct
 return city in string
 *****************************************************************************/
string validCity() {
   string city;
   bool cityStatus, isSpace = false;
   do { // check the city name
      cityStatus = true;
      int count = 0, index = 0;
      cout << "Please input the city name: ";
      getline(cin, city);
      for (int i = 0; i < city.length(); i++) {
         if (isdigit(city[i])) { ///***
            cityStatus = false;
         }
         if (isspace(city[i])) {
            index = i;
            count++;
            isSpace = true;
         }
         else {
            count = 1;
         }
      }
      if (isSpace) {
         city[0] = toupper(city[0]);
         for (int i = 1; i < index - count + 1; i++) {
            city[i] = tolower(city[i]);
         }
         city[index + 1] = toupper(city[index + 1]);
         for (int i = index + 2; i < city.length(); i++) {
            city[i] = tolower(city[i]);
         }
         city.erase(index - count + 1, count - 1);
      }
      else {
         city[0] = toupper(city[0]);
         for (int i = 1; i < index - count + 1; i++) {
            city[i] = tolower(city[i]);
         }
      }
   } while (!cityStatus);
   return city;
}
/*****************************************************************************
 Insert manager: check if the input of year is correct
 return zipcode in string
 *****************************************************************************/
string validCode() {
   string zipCode;
   while (true) { // check the year
      cout << "Please input the zip code: ";
      getline(cin, zipCode);
      try {
         int year = std::stoi(zipCode);
         if (year < 90001 || year > 96162) { // the range of zip code in CA
            continue;
         }
      }
      catch (const std::exception &) {
         continue;
      }
      break;
   }
   //cout << zipCode << endl;

   return zipCode;
}

/*****************************************************************************
The header of data, print the zip code of city
call the function name, address as pointer to be called
 *****************************************************************************/
void printData(Zipcode *&zip) {
   cout << left << setw(8) << zip->getZipCode();
   cout << left << setw(15) << zip->getCityName();
   cout << left << setw(15) << zip->getCountyName();
   cout << left << setw(10) << zip->getTotalPopulation();
   cout << left << setw(10) << zip->getAfricanAmerican();
   cout << left << setw(10) << zip->getAsian();
   cout << left << setw(10) << zip->getHispanicLatino();
   cout << left << setw(10) << zip->getWhite() << endl;
}

/*****************************************************************************
The header of data, print the zip code of city
call the function name, address as pointer to be called
 *****************************************************************************/
void printZip(Zipcode *&zip) { cout << zip->getZipCode() << endl; }

/*****************************************************************************
The header of data, print the name of city
call the function name, address as pointer to be called
 *****************************************************************************/
void printName(Zipcode *&zip) { cout << zip->getCityName() << endl; }

/*****************************************************************************
 Function pointer compare : 0 means equal, -1 means left < right, 1 means left
>right The function pointer uses for compare the zip code
*****************************************************************************/
int compareByZip(Zipcode *z1, Zipcode *z2) {
   return z1->getZipCode().compare(z2->getZipCode());
}

/*****************************************************************************
 Function pointer compare : 0 means equal, -1 means left < right, 1 means left
>right The function pointer uses for compare the city name
*****************************************************************************/
int compareByName(Zipcode *z1, Zipcode *z2) {
   return z1->getCityName().compare(z2->getCityName());
}

/***********************************************************
badHash function map the use zipcode as the key to calculate
the address that mapped the obj into the corresponding bucket,
This method may raise higher collision.
***********************************************************/
int badHash(Zipcode* zip, int tableSize)
{
   long int sum = 0;
   string key = zip->getZipCode();
   for (int i = 0; i < key.length(); i++)
   {
      sum += key[i];
   }
   return sum % tableSize;
}

/***********************************************************
goodHash function map the use zipcode as the key to calculate
the address that mapped the obj into the corresponding bucket,
This method may raise lower collision.
***********************************************************/
int goodHash(Zipcode* zip, int tableSize)
{
   long int sum = 0;
   string key = zip->getZipCode();
   for (int i = 0; i < key.length(); i++)
   {
      sum += key[i] * key[i] * key[i] * (i + 1);
   }
   return sum % tableSize;
}
/**************************************************************
 this function print the statics infomation of the hashtable
 which include:bucket count, collison number and load
 factor.
*****************************************************************/
void printStatics(Database &d)
{
   cout << "=============== " << endl;
   cout << "Hash Statstics: " << endl;
   cout << "=============== " << endl;
   cout << "Table Size: " << d.hashZip.getTableSize() << endl;
   cout << "Bucket count: " << d.hashZip.getBucketCount() << endl;
   cout << "Collision count: " << d.hashZip.getCollision() << endl;
   cout << "Load factor: " << setprecision(2) << d.hashZip.getLoadFactor() << endl;
   cout << "===============" << endl << endl;
}
/********************************************************
 this function read data from file into object.
***********************************************************/
Zipcode* getData() {
   string zipCode;  // unique key
   string cityName; // secondary key
   string countyName;
   int hispanicLatino;
   int asian;
   int white;
   int africanAmerican;
   int totalPopulation;
   Zipcode *zip = new Zipcode;
   cout << "zipCode: ";
   getline(cin, zipCode);
   cout << "City Name: ";
   getline(cin, cityName);
   cout << "County Name: ";
   getline(cin, countyName);
   cout << "Hispanic Latino: ";
   cin >> hispanicLatino;
   cout << "Asian: ";
   cin >> asian;
   cout << "White: ";
   cin >> white;
   cout << "African American: ";
   cin >> africanAmerican;
   cout << "Total Population: ";
   cin >> totalPopulation;
   zip->setZipcode(zipCode);
   zip->setcityName(cityName);
   zip->setCountyName(countyName);
   zip->setTotalPopulation(totalPopulation);
   zip->setAfricanAmerican(africanAmerican);
   zip->setAsian(asian);
   zip->setHispanicLatino(hispanicLatino);
   zip->setWhite(white);
   //cout << *zip << endl;
   return zip;
}
/********************************************************
this function print the top header .
***********************************************************/
void printTop() {
   cout << "====================================================================================" << endl;
   cout << "ZipCode    City          County       Total    AAmerican Asian     Hispanic  White   " << endl;
   cout << "====================================================================================" << endl;
}
/**************************************************************
this function  manage the search hashtable. if found
 print the data infomation. else show not foun infomation
 parameter: Zipcode, Database
*****************************************************************/
void searchHashManager(Zipcode z, Database &d)
{
    Zipcode *dataout = NULL;
    if(d.hashZip.searchHashTable(&z, dataout, compareByZip, goodHash))
    {
        cout << "\n~~**Data found in HashTable!**~~" << endl;
        cout << *dataout << endl;
    }
    else
        cout << "~~**Data not found in HashTable!**~~" << endl;
}
/********************************************************
this function show the delete submenu.
***********************************************************/
void deleteMenu()
{
    cout << "\t" << PRIMARY_DELETE << " - Delete element using the primary key(zip code)\t" << endl;
    cout << "\t" << SECOND_DELETE << " - Delete elements using the secondary key(name) (all matches)\t" << endl;
    cout << "\t" << EXIT_CHOICE << "- Exit\n" << endl;
}
/********************************************************
this function show the search submenu.
***********************************************************/
void searchMenu()
{
    cout << PRIMARY_CHOICE
         << " - Find and display one element using the primary key(zip code)\n\t"
         << endl;
    cout << SECOND_CHOICE
         << " - Find and display elements using the secondary key(name) (all "
       "matches)\n\t"
         << endl;
}
/********************************************************
this function show the List submenu.
***********************************************************/
void listMenu()
{
    cout << "\t" << UNSORTED_CHOICE << " - List unsorted data" << endl;
    cout << "\t" << PSORTED_CHOICE << " - List data sorted by the primary key(zip code)" << endl;
    cout << "\t" << SSORTED_CHOICE << " - List data sorted by the secondary key(name)" << endl;
    cout << "\t" << SPECIAL_CHOICE << " - Special print, as an indented list" << endl;
    cout << "\t" << EXIT_CHOICE << " - Exit\n" << endl;
}


/*
 OUTPUT

 What do you want to do?
     F - Inserting a new element
     D - Delete the element from tree
     N - Undo the deleted element
     S - Search (has a sub-menu)
     L - List (has a sub-menu)
     W - Write data to a file
     I - Statistics
     A - Display menu
     E - Exit
 Main Menu: Enter your choice: F

 Inserting a new element...
 zipCode: 93333
 City Name: San Diego
 County Name: Orange
 Hispanic Latino: 100
 Asian: 200
 White: 300
 African American: 400
 Total Population: 2000
 Main Menu: Enter your choice: S
 P - Find and display one element using the primary key(zip code)
     
 C - Find and display elements using the secondary key(name) (all matches)
     
 E- Exit

 Search Menu: Enter your choice: p

 Finding Data By Zip Code...
 Please input the zip code: 95014

 ~~**Data found in HashTable!**~~
 County Name: Santa Clara
 City Name: Cupertino
 zipCode: 95014
 Hispanic Latino: 2407
 Asian: 41627
 White: 16354
 African American: 522
 Total Population: 62682


 ~~**Data Found in Tree!**~~
 County Name: Santa Clara
 City Name: Cupertino
 zipCode: 95014
 Hispanic Latino: 2407
 Asian: 41627
 White: 16354
 African American: 522
 Total Population: 62682


 Search Menu: Enter your choice: c

 Finding Data By City Name...
 Please input the city name: San Jose
 Data Found!
 County Name: Santa Clara
 City Name: San Jose
 zipCode: 95123
 Hispanic Latino: 18843
 Asian: 15032
 White: 26706
 African American: 3002
 Total Population: 66858


 County Name: Santa Clara
 City Name: San Jose
 zipCode: 95111
 Hispanic Latino: 33573
 Asian: 22341
 White: 5856
 African American: 1602
 Total Population: 64891


 Search Menu: Enter your choice: e
 Main Menu: Enter your choice: d
     M - Delete element using the primary key(zip code)
     N - Delete elements using the secondary key(name) (all matches)
     E- Exit

 Delete Menu: Enter your choice: M
 Doing Delete...
 Please input the zip code: 95014
 95014 deleted!

 Delete Menu: Enter your choice: n
 Doing Delete...
 Please input the city name: San Jose
 San Jose deleted!
 San Jose deleted!

 Delete Menu: Enter your choice: e
 Main Menu: Enter your choice: l
     U - List unsorted data
     O - List data sorted by the primary key(zip code)
     R - List data sorted by the secondary key(name)
     Z - Special print, as an indented list
     E - Exit

 List Menu: Enter your choice: u
 ====================================================================================
 ZipCode    City          County       Total    AAmerican Asian     Hispanic  White
 ====================================================================================
 Bucket [3]:
 95687   Vacaville      Solano         67504     8269      4900      15174     34511
 Bucket [5]:
 93333   San Diego      Orange         2000      400       200       100       300
 Bucket [6]:
 92704   Santa Ana      Orange         90525     859       16069     64901     7983
 95064   Santa Cruz     Santa Cruz     9993      247       2834      2819      3506
 Bucket [8]:
 93063   Simi Valley    Ventura        56457     816       5143      14183     34544
 Bucket [11]:
 93117   Goleta         Santa Barbara  57913     1413      7949      17847     28528
 Bucket [15]:
 92020   El Cajon       San Diego      58862     3217      2133      15113     35396
 94305   Stanford       Santa Clara    15343     661       4124      2073      7287
 Bucket [16]:
 95020   Gilroy         Santa Clara    63251     1012      4242      36678     20144
 95212   Stockton       San Joaquin    27512     1508      11900     6873      6016
 Bucket [18]:
 90210   Beverly Hills  Los Angeles    20016     188       1680      804       16546
 Bucket [20]:
 91711   Claremont      Los Angeles    36933     1990      5059      10070     18245
 Bucket [24]:
 95376   Tracy          San Joaquin    52371     2689      4658      24208     18054
 Bucket [25]:
 93033   Oxnard         Ventura        83572     1799      5802      70401     4867
 Bucket [28]:
 95035   Milpitas       Santa Clara    75614     2365      50484     11742     8312
 95616   Davis          Yolo           50068     1438      12393     6874      26389
 Bucket [29]:
 90011   Los Angeles    Los Angeles    108051    9040      619       97454     626
 Bucket [32]:
 92399   Yucaipa        San Bernardino 54027     627       1400      18387     32439
 Bucket [34]:
 92114   San Diego      San Diego      69635     13657     17345     30277     6211
 93003   Ventura        Ventura        51956     831       2317      16417     30526
 Bucket [40]:
 94123   San Francisco  San Francisco  25461     470       2565      1693      19808
 Bucket [46]:
 95630   Folsom         Sacramento     75864     3168      12492     8489      47543
 Bucket [47]:
 94087   Sunnyvale      Santa Clara    57178     769       27823     4162      21929
 ======= ============== ==============  ================

 List Menu: Enter your choice: o
 Using The Primary Key(zip code):
 ====================================================================================
 ZipCode    City          County       Total    AAmerican Asian     Hispanic  White
 ====================================================================================
 90011   Los Angeles    Los Angeles    108051    9040      619       97454     626
 90210   Beverly Hills  Los Angeles    20016     188       1680      804       16546
 91711   Claremont      Los Angeles    36933     1990      5059      10070     18245
 92020   El Cajon       San Diego      58862     3217      2133      15113     35396
 92114   San Diego      San Diego      69635     13657     17345     30277     6211
 92399   Yucaipa        San Bernardino 54027     627       1400      18387     32439
 92704   Santa Ana      Orange         90525     859       16069     64901     7983
 93003   Ventura        Ventura        51956     831       2317      16417     30526
 93033   Oxnard         Ventura        83572     1799      5802      70401     4867
 93063   Simi Valley    Ventura        56457     816       5143      14183     34544
 93117   Goleta         Santa Barbara  57913     1413      7949      17847     28528
 93333   San Diego      Orange         2000      400       200       100       300
 94087   Sunnyvale      Santa Clara    57178     769       27823     4162      21929
 94123   San Francisco  San Francisco  25461     470       2565      1693      19808
 94305   Stanford       Santa Clara    15343     661       4124      2073      7287
 95020   Gilroy         Santa Clara    63251     1012      4242      36678     20144
 95035   Milpitas       Santa Clara    75614     2365      50484     11742     8312
 95064   Santa Cruz     Santa Cruz     9993      247       2834      2819      3506
 95212   Stockton       San Joaquin    27512     1508      11900     6873      6016
 95376   Tracy          San Joaquin    52371     2689      4658      24208     18054
 95616   Davis          Yolo           50068     1438      12393     6874      26389
 95630   Folsom         Sacramento     75864     3168      12492     8489      47543
 95687   Vacaville      Solano         67504     8269      4900      15174     34511

 List Menu: Enter your choice: r
 Using The Secondary Key(name):
 ====================================================================================
 ZipCode    City          County       Total    AAmerican Asian     Hispanic  White
 ====================================================================================
 90210   Beverly Hills  Los Angeles    20016     188       1680      804       16546
 91711   Claremont      Los Angeles    36933     1990      5059      10070     18245
 95616   Davis          Yolo           50068     1438      12393     6874      26389
 92020   El Cajon       San Diego      58862     3217      2133      15113     35396
 95630   Folsom         Sacramento     75864     3168      12492     8489      47543
 95020   Gilroy         Santa Clara    63251     1012      4242      36678     20144
 93117   Goleta         Santa Barbara  57913     1413      7949      17847     28528
 90011   Los Angeles    Los Angeles    108051    9040      619       97454     626
 95035   Milpitas       Santa Clara    75614     2365      50484     11742     8312
 93033   Oxnard         Ventura        83572     1799      5802      70401     4867
 92114   San Diego      San Diego      69635     13657     17345     30277     6211
 93333   San Diego      Orange         2000      400       200       100       300
 94123   San Francisco  San Francisco  25461     470       2565      1693      19808
 92704   Santa Ana      Orange         90525     859       16069     64901     7983
 95064   Santa Cruz     Santa Cruz     9993      247       2834      2819      3506
 93063   Simi Valley    Ventura        56457     816       5143      14183     34544
 94305   Stanford       Santa Clara    15343     661       4124      2073      7287
 95212   Stockton       San Joaquin    27512     1508      11900     6873      6016
 94087   Sunnyvale      Santa Clara    57178     769       27823     4162      21929
 95376   Tracy          San Joaquin    52371     2689      4658      24208     18054
 95687   Vacaville      Solano         67504     8269      4900      15174     34511
 93003   Ventura        Ventura        51956     831       2317      16417     30526
 92399   Yucaipa        San Bernardino 54027     627       1400      18387     32439

 List Menu: Enter your choice: z
 Using The Indented Tree:
 Level 1.95212
     Level 2.95376
         Level 3.95616
             Level 4.95630
                 Level 5.95687
     Level 2.95035
         Level 3.95064
         Level 3.94087
             Level 4.94123
                 Level 5.95020
                     Level 6.94305
             Level 4.90011
                 Level 5.91711
                     Level 6.93033
                         Level 7.93063
                             Level 8.93117
                                 Level 9.93333
                         Level 7.92114
                             Level 8.92704
                                 Level 9.93003
                                 Level 9.92399
                             Level 8.92020
                     Level 6.90210


 List Menu: Enter your choice: e
 Main Menu: Enter your choice: u
 Main Menu: Enter your choice: n
 Undoing Delete...
 San Jose
 San Jose
 Cupertino

 Main Menu: Enter your choice: i
 ===============
 Hash Statstics:
 ===============
 Table Size: 53
 Bucket count: 20
 Collision count: 6
 Load factor: 0.38
 ===============

 Main Menu: Enter your choice: a

 What do you want to do?
     F - Inserting a new element
     D - Delete the element from tree
     N - Undo the deleted element
     S - Search (has a sub-menu)
     L - List (has a sub-menu)
     W - Write data to a file
     I - Statistics
     A - Display menu
     E - Exit
 Main Menu: Enter your choice: w
 Main Menu: Enter your choice: e

     *** THE END ***
 Program ended with exit code: 0
 */
