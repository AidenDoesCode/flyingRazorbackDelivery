#include "Address.h"
#include <iostream>

using namespace std;

// Default constructor
Address::Address() 
    : latitude(0.0f), longitude(0.0f), houseNumber(0), streetName("") {}

// Parameterized constructor
Address::Address(float lat, float lon, int houseNum, const string& street)
    : latitude(lat), longitude(lon), houseNumber(houseNum), streetName(street) {}

// Destructor
Address::~Address() {}

// Getters
float Address::getLatitude() const { return latitude; }
float Address::getLongitude() const { return longitude; }
int Address::getHouseNumber() const { return houseNumber; }
string Address::getStreetName() const { return streetName; }

// Setters
void Address::setLatitude(float lat) { latitude = lat; }
void Address::setLongitude(float lon) { longitude = lon; }
void Address::setHouseNumber(int houseNum) { houseNumber = houseNum; }
void Address::setStreetName(const string& street) { streetName = street; }

// Print method
void Address::print() const {
    cout << latitude << " " << longitude << " " 
         << houseNumber << " " << streetName << endl;
}