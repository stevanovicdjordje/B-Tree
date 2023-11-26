#include <iostream>
#include "BTree.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

void menu() {
    std::cout << "****  MENU  ****" << std::endl;
    std::cout << "1. Make a tree" << std::endl;
    std::cout << "2. Print tree" << std::endl;
    std::cout << "3. Print customer information" << std::endl;
    std::cout << "4. Insert new record" << std::endl;
    std::cout << "5. Delete record" << std::endl;
    std::cout << "6. Delete all customer records" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

std::vector<std::string> process_line(const std::string &line_to_process, char delimiter) {
    std::istringstream line(line_to_process);
    std::string token;
    std::vector<std::string> lines;

    while (std::getline(line, token, delimiter)) {
        lines.push_back(token);
    }
    lines.erase(lines.begin() + 1);
    lines.erase(lines.begin() + 2);
    return lines;
}


std::basic_ifstream<char> open_file(const char *file_name) {
    std::ifstream data_file;
    data_file.open(file_name);
    return data_file;
}


BTree make_tree(int level) {
    char delimiter = '|';
    std::vector<std::string> data;
    std::string line_to_process;
    std::ifstream data_file;
    BTree Customers(level);
    data_file = open_file("C://Users//jovan//CLionProjects//asp2dz2//CustomerAccount20.txt");
    if (!data_file.is_open()) {
        std::cout << "File is not open!" << std::endl;
        exit(0);
    } else {
        while (std::getline(data_file, line_to_process)) {
            data = process_line(line_to_process, delimiter);
            long long int key = std::stoll(data[1]);
            BTree::AccountData account_data;
            account_data.data.emplace_back(std::stoll(data[0]));
            account_data.data.emplace_back(std::stoi(data[2]));
            account_data.data.emplace_back(std::stof(data[3]));
            Customers.insert(key, account_data);
        }
    }
    return Customers;
}

int main() {
    BTree Customers;
    int choice, level;
    while (true) {
        menu();
        std::cin >> choice;
        switch (choice) {
            case 1:
                std::cout << "Making tree..." << std::endl;
                std::cin >> level;
                Customers = make_tree(level);
                break;
            case 2:
                std::cout << "Printing tree..." << std::endl;
                Customers.print_tree(Customers.root);
                break;
            case 3:
                std::cout << "Printing customer information..." << std::endl;
                ///
                break;
            case 4:
                std::cout << "Inserting new record..." << std::endl;
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
                Customers.delete_tree(Customers.root);
                exit(0);
            default:
                std::cout << "Wrong choice, try again" << std::endl;
                menu();
                std::cin >> choice;
                break;
        }
    }
}
