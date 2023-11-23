#include <iostream>
#include "BTree.h"
#include <fstream>

void menu() {
    std::cout << "****  MENU  ****" << std::endl;
    std::cout << "1. Make a tree" << std::endl;
    std::cout << "2. Print tree" << std::endl;
    std::cout << "3. Print customer information" << std::endl;
    std::cout << "4. Enter new record" << std::endl;
    std::cout << "5. Delete record" << std::endl;
    std::cout << "6. Delete all customer records" << std::endl;
    std::cout << "0. Exit" << std::endl;
}



int main() {
    int choice;
    while (true) {
        menu();
        std::cin >> choice;
        switch (choice) {
            case 1:
                std::cout << "Making tree..." << std::endl;
                ///;
                break;
            case 2:
                std::cout << "Printing tree..." << std::endl;
                ///;
                break;
            case 3:
                std::cout << "Printing customer information..." << std::endl;
                ///
                break;
            case 4:
                std::cout << "Entering new record..." << std::endl;
                ///;
                break;
            case 5:
                std::cout << "Deleting record..." << std::endl;
                ///;
                break;
            case 6:
                std::cout << "Deleting all customer records..." << std::endl;
                ///;
                break;
            case 0:
                std::cout << "Exiting..." << std::endl;
                exit(0);
            default:
                std::cout << "Wrong choice, try again" << std::endl;
                menu();
                std::cin >> choice;
                break;
        }
    }
    return 0;
}
