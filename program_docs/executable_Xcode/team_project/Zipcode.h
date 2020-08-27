//
//  Group 3- Project
//  c++
//
//  Programmer: Ruichun Chen, Xin Jing, Wenhui Li, Wenzhe Xu, De Yi Huang
//  IDE: xcode/VS

//  In this program:
//  1. the zipCode class contains zipCode as primary key, cityName as secondary key, county name, total polulation, the polulation of Hispanic Latino, Asian, White, African American
//  2. the overloading > and < function, use the compare function to compare two values
//  3. the overloading the << funciton is used to print out the data
#ifndef ZIPCODE_H_INCLUDED
#define ZIPCODE_H_INCLUDED
#include <iostream>
#include <string>
using std::string;
using std::endl;
using std::ostream;
using std::istream;

class Zipcode;
ostream &operator << (ostream &, const Zipcode &);

//overloading stream operation function prototype
class Zipcode
{
private:
    string zipCode; // unique key
    string cityName; // secondary key
    string countyName;
    int hispanicLatino;
    int asian;
    int white;
    int africanAmerican;
    int totalPopulation;
public:
    //constructors
    Zipcode() {zipCode = ""; cityName = ""; countyName = ""; hispanicLatino = -1; asian = -1; white = -1; africanAmerican = -1; totalPopulation = -1;}
    Zipcode(string zip, string city, string county,  int hl, int as, int wh, int aa, int total)
    {zipCode = zip; cityName = city; countyName = county; hispanicLatino = hl; asian = as; white = wh; africanAmerican = aa; totalPopulation = total;}

    //setters
    void setZipcode(string zip) { zipCode = zip; }
    void setcityName(string name) { cityName = name; }
    void setCountyName(string name) {countyName = name;}
    void setHispanicLatino(int his) { hispanicLatino = his; }
    void setAsian(int as) { asian = as; }
    void setWhite(int wh) { white = wh; }
    void setAfricanAmerican(int afa) { africanAmerican = afa; }
    void setTotalPopulation(int p) { totalPopulation = p; } /// depends on the data given, if total population is given directly, just assign it. else, calculate it and assign.

    //getters
    string getZipCode() const {return zipCode;}
    string getCityName() const {return cityName;}
    string getCountyName() const {return countyName;}
    int getHispanicLatino() const { return hispanicLatino; }
    int getAsian() const { return asian; }
    int getWhite() const { return white; }
    int getAfricanAmerican() const { return africanAmerican; }
    int getTotalPopulation() const { return totalPopulation; }

    // other functions
    bool operator < (const Zipcode &right) { return (zipCode.compare(right.zipCode) < 0); }
    bool operator > (const Zipcode &right) { return (zipCode.compare(right.zipCode) > 0); }
    bool operator == (const Zipcode &right) { return (zipCode.compare(right.zipCode) == 0);}
    friend std::ostream &operator << ( ostream & strm, const Zipcode & obj)
    {
        strm << "County Name: " << obj.countyName << endl;
        strm << "City Name: " << obj.cityName << endl;
        strm << "zipCode: " << obj.zipCode << endl;
        strm << "Hispanic Latino: " <<  obj.hispanicLatino << endl;
        strm << "Asian: " << obj.asian << endl;
        strm << "White: " << obj.white << endl;
        strm << "African American: " << obj.africanAmerican << endl;
        strm << "Total Population: " << obj.totalPopulation << endl << endl;
        return strm;
    }
};

/*ostream &operator << (ostream &strm, const Zipcode &obj)
{
    strm << "County Name: " << obj.countyName << endl;
    strm << "City Name: " << obj.cityName << endl;
    strm << "zipCode: " << obj.zipCode << endl;
    strm << "Hispanic Latino: " <<  obj.hispanicLatino << endl;
    strm << "Asian: " << obj.asian << endl;
    strm << "White: " << obj.white << endl;
    strm << "African American: " << obj.africanAmerican << endl;
    strm << "Total Population: " << obj.totalPopulation << endl;
    return strm;
}*/
#endif // ZIPCODE_H_INCLUDED
