// Testing file provided for EECS 281 Lab 7

#include "hashtable.h"
#include <string>
#include <iostream>

int main() {

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
    std::cout << midterm.erase("sam") << " ";
    std::cout << midterm["sam"] << "\n";
    // ADD MORE TESTS OF YOUR OWN
    
    std::cout << midterm["mas"] << "\n";
    midterm["mas"] = 10;
    std::cout << midterm["sm"] << " ";
    midterm["sm"] = 1;
    std::cout << midterm["fesse"] << " ";
    midterm["fesse"] = 2;
    std::cout << midterm["ung"] << " ";
    midterm["ung"] = 3;
    std::cout << midterm["jis"] << " \n";
    midterm["jis"] = 4;
    std:: cout << midterm.insert("jis", 5);
    

    return 0;
}
