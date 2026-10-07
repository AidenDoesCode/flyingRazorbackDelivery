#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <limits>
#include "Address.h"

using namespace std;

// Convert degrees to radians helper function
double toRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

// Helper function to convert a string to lowercase for case-insensitive comparisons
string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c) { return tolower(c); });
    return lowerStr;
}

// Task A: Calculates the Haversine distance in meters between two lat/lon coordinates
double calculate_distance(float lat1, float lon1, float lat2, float lon2) {
    const double R = 6371008.8; // Earth's radius in meters

    double phi1 = toRadians(lat1);
    double phi2 = toRadians(lat2);
    double delta_phi = toRadians(lat2 - lat1);
    double delta_lambda = toRadians(lon2 - lon1);

    double a = sin(delta_phi / 2.0) * sin(delta_phi / 2.0) +
               cos(phi1) * cos(phi2) *
               sin(delta_lambda / 2.0) * sin(delta_lambda / 2.0);

    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));

    return R * c; // Distance in meters
}

//function to read address, helper function
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

    while (inputFile >> lat >> lon >> houseNum) {
        getline(inputFile >> ws, streetName); // Read the rest of the line as street name
        addresses.emplace_back(lat, lon, houseNum, streetName);
    }

    inputFile.close();
    return addresses;
}

// Recursive Binary Search implementation
int binarySearchAddressRecursive(const vector<Address>& addresses, int houseNum, const string& streetName, int low, int high) {
    if (low > high) {
        return -1; // Not found
    }

    int mid = low + (high - low) / 2;
    string targetStreet = toLower(streetName);
    string currentStreet = toLower(addresses[mid].getStreetName());

    if (currentStreet == targetStreet) {
        if (addresses[mid].getHouseNumber() == houseNum) {
            return mid; // Exact match found
        } else if (addresses[mid].getHouseNumber() < houseNum) {
            return binarySearchAddressRecursive(addresses, houseNum, streetName, mid + 1, high);
        } else {
            return binarySearchAddressRecursive(addresses, houseNum, streetName, low, mid - 1);
        }
    } else if (currentStreet < targetStreet) {
        return binarySearchAddressRecursive(addresses, houseNum, streetName, mid + 1, high);
    } else {
        return binarySearchAddressRecursive(addresses, houseNum, streetName, low, mid - 1);
    }
}

// Linear Search to find the address closest to a given lat/lon cord.
int findClosestAddressLinear(const vector<Address>& addresses, float targetLat, float targetLon) {
    if (addresses.empty()) return -1;

    int closestIndex = 0;
    double minDistance = numeric_limits<double>::max();

    for (size_t i = 0; i < addresses.size(); ++i) {
        double dist = calculate_distance(targetLat, targetLon, 
                                         addresses[i].getLatitude(), 
                                         addresses[i].getLongitude());
        if (dist < minDistance) {
            minDistance = dist;
            closestIndex = i;
        }
    }

    return closestIndex;
}

//Find the central city address for the pizza kitchen
Address findCentralLocation(const vector<Address>& addresses) {
    double latSum = 0.0;
    double lonSum = 0.0;

    for (const auto& addr : addresses) {
        latSum += addr.getLatitude();
        lonSum += addr.getLongitude();
    }

    float avgLat = static_cast<float>(latSum / addresses.size());
    float avgLon = static_cast<float>(lonSum / addresses.size());

    int closestIdx = findClosestAddressLinear(addresses, avgLat, avgLon);
    return addresses[closestIdx];
}

// Query distance to a specific customer
void calculateDistanceToCustomer(const vector<Address>& addresses, const Address& kitchenAddress) {
    int houseNum;
    string streetName;

    cout << "\n--- Customer Distance Query ---" << endl;
    cout << "Enter customer house number: ";
    while (!(cin >> houseNum)) {
        cout << "Invalid input. Please enter a valid house number: ";
        //clear console and buffer
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter customer street name: ";
    getline(cin, streetName);

    int idx = binarySearchAddressRecursive(addresses, houseNum, streetName, 0, addresses.size() - 1);

    if (idx != -1) {
        double distance = calculate_distance(kitchenAddress.getLatitude(), kitchenAddress.getLongitude(),
                                             addresses[idx].getLatitude(), addresses[idx].getLongitude());
        cout << "\n[Match Found]" << endl;
        addresses[idx].print();
        cout << "Distance from Flying Razorback Pizza Kitchen: " << distance << " meters" << endl;
    } else {
        cout << "\nError: Address '" << houseNum << " " << streetName << "' was not found in database." << endl;
    }
}

// Count addresses within drone delivery range
void evaluateDroneCoverage(const vector<Address>& addresses, const Address& kitchenAddress) {
    double droneRange;
    cout << "\n--- Drone Coverage Analysis ---" << endl;
    cout << "Enter drone operational range (in meters): ";
    while (!(cin >> droneRange) || droneRange < 0) {
        cout << "Invalid range. Please enter a positive number: ";
        //clear console and buffer
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int reachableCustomers = 0;
    for (const auto& addr : addresses) {
        double dist = calculate_distance(kitchenAddress.getLatitude(), kitchenAddress.getLongitude(),
                                         addr.getLatitude(), addr.getLongitude());
        if (dist <= droneRange) {
            reachableCustomers++;
        }
    }

    double percentage = (static_cast<double>(reachableCustomers) / addresses.size()) * 100.0;
    cout << "\nCoverage Results:" << endl;
    cout << "- Total customers reachable: " << reachableCustomers << " / " << addresses.size() << endl;
    cout << "- City coverage percentage: " << percentage << "%" << endl;
}

int main() {
    string filename = "street_name_1000.txt"; //for the rec binary search

    vector<Address> addresses = read_address(filename);

    if (addresses.empty()) {
        cerr << "Failed to read address data. Exiting program." << endl;
        return 1;
    }

    Address kitchenLocation = findCentralLocation(addresses);
    int choice = 0;

    do {
        cout << "\n==========================================" << endl;
        cout << "   Flying Razorback Pizza Delivery System " << endl;
        cout << "==========================================" << endl;
        cout << "1. Display Kitchen Location (Central Address)" << endl;
        cout << "2. Calculate Distance to Customer Address" << endl;
        cout << "3. Analyze Drone Coverage Range" << endl;
        cout << "4. Exit" << endl;
        cout << "Select an option (1-4): ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number between 1 and 4." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                cout << "\n--- Kitchen Location ---" << endl;
                kitchenLocation.print();
                break;
            case 2:
                calculateDistanceToCustomer(addresses, kitchenLocation);
                break;
            case 3:
                evaluateDroneCoverage(addresses, kitchenLocation);
                break;
            case 4:
                cout << "\nExiting application. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid option! Please select a valid menu choice (1-4)." << endl;
                break;
        }

    } while (choice != 4);

    return 0;
}