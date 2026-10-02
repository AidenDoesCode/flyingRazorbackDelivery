#ifndef ADDRESS_H
#define ADDRESS_H

#include <iostream>
#include <string>

using namespace std;

class Address {
private:
    float latitude;
    float longitude;
    int houseNumber;
    string streetName;

public:
    // Default constructor
    Address();

    // Parameterized constructor
    Address(float lat, float lon, int houseNum, const string& street);

    // Destructor
    ~Address();

    // Getters
    float getLatitude() const;
    float getLongitude() const;
    int getHouseNumber() const;
    string getStreetName() const;

    // Setters
    void setLatitude(float lat);
    void setLongitude(float lon);
    void setHouseNumber(int houseNum);
    void setStreetName(const string& street);

    // Print method
    void print() const;
};

#endif