// Testing file provided for EECS 281 Lab 7

#include "hashtable.h"
#include <string>
#include <iostream>
#include <functional>

int main() {
    std::hash<std::string> hasher;
    std::cout << hasher("jishaa")%12;
    
    // VERY BASIC EXAMPLE (will likely not compile with default/empty hashtable.h file)
    HashTable<std::string, int> midterm;
    std::cout << midterm.insert("sam", 50) << " ";
    std::cout << midterm.insert("fee", 100) << " ";
    std::cout << midterm.insert("milo", 95) << " ";
    std::cout << midterm.insert("gabe", 88) << " \n";
    std::cout << midterm["sam"] << " ";
    std::cout << midterm["fee"] << " ";
    std::cout << midterm["milo"] << " ";
    std::cout << midterm["gabe"] << " \n";
    std::cout << midterm["jishaa"] << " \n";
    std::cout << midterm.erase("sam") << " ";
    std::cout << midterm.erase("fee") << " ";
    std::cout << midterm.erase("milo") << " ";
    std::cout << midterm.erase("gabe") << " ";
    std::cout << midterm["sam"] << "\n";
    std::cout << midterm["jishaa"] << " \n";
    // ADD MORE TESTS OF YOUR OWN
    
    std::cout << midterm["jeno"] << "\n";
    midterm["jeno"] = 10;
    std::cout << midterm["jisung"] << " ";
    midterm["jisung"] = 1;
    std::cout << midterm["mark"] << " ";
    midterm["mark"] = 2;
    std::cout << midterm["haechan"] << " ";
    midterm["haechan"] = 3;
    std::cout << midterm["haechan"] << " ";
    std::cout << midterm.insert("haechan", 100) << " ";
    std::cout << midterm["renjun"] << " \n";
    midterm["renjun"] = 4;
    std:: cout << midterm.insert("jisung park", 5);

    return 0;
}
