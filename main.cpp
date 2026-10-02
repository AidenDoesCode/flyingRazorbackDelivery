#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Address.h"

using namespace std;

// Task 2: read_address function implementation
vector<Address> read_address(const string& filename) {
    vector<Address> addresses;
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return addresses;
    }

    float lat;
    float lon;
    int houseNum;
    string streetName;

    // Parse each line formatted as: [Latitude] [Longitude] [House Number] [Street Name...]
    while (inputFile >> lat >> lon >> houseNum) {
        getline(inputFile >> ws, streetName); // Read the rest of the line as street name
        addresses.emplace_back(lat, lon, houseNum, streetName);
    }

    inputFile.close();
    return addresses;
}

int main() {
    string filename = "locations_1000.txt"; // Or "street_name_1000.txt"

    vector<Address> addresses = read_address(filename);

    if (!addresses.empty()) {
        cout << "First Address:" << endl;
        addresses.front().print();

        cout << "\nLast Address:" << endl;
        addresses.back().print();
    } else {
        cout << "No addresses read or file empty." << endl;
    }

    return 0;
}