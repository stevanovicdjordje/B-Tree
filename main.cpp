#include <iostream>
#include "BTree.h"
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

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
    data_file = open_file("C://Users//jovan//CLionProjects//asp2dz2//CustomerAccount250.txt");
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
    data_file.close();
    return Customers;
}


std::vector<std::string> process_customer_line(const std::string &line_to_process, char delimiter) {
    std::istringstream line(line_to_process);
    std::string token;
    std::vector<std::string> lines;

    while (std::getline(line, token, delimiter)) {
        lines.push_back(token);
    }
    lines.erase(lines.begin() + 4);
    return lines;
}

void search_for_customer_and_print(BTree Customer, const std::vector<std::string> &data) {
    const char *out_file = "output.txt";
    std::ofstream output_file(out_file);

    long long int key = std::stoll(data[0]);
    BTree::SearchResult search_result = Customer.search_key(key);
    if (search_result.isFound) {
        std::cout << "Total steps: " << search_result.steps << std::endl;
        int i = 0;
        while (key != search_result.node->keys[i].key) {
            i++;
        }
        int count = 0;
        std::vector<BTree::AccountData> account_data;
        account_data = search_result.node->keys[i].account_data;
        while (count != account_data.size()) {
            output_file << key << "|" << data[1] << "|" << data[2] << "|" << data[3] << "|"
                        << std::get<long long int>(account_data[count].data[0]) << "|" << key << "|"
                        << std::get<int>(account_data[count].data[1]) << "|"
                        << std::fixed << std::setprecision(2) << std::get<float>(account_data[count].data[2])
                        << std::endl;
            count++;
        }
    } else {
        output_file << key << "|" << data[1] << "|" << data[2] << "|" << data[3] << "|" << "null" << "|" << "null"
                    << "|"
                    << "null" << "|" << "null" << std::endl;
    }
    output_file.close();
}

void customer_search(BTree Customer) {
    char delimiter = '|';
    std::vector<std::string> data;
    std::string line_to_process;
    std::ifstream data_file;
    data_file = open_file("C://Users//jovan//CLionProjects//asp2dz2//Customer.txt");
    if (!data_file.is_open()) {
        std::cout << "File is not open!" << std::endl;
        exit(0);
    } else {
        while (std::getline(data_file, line_to_process)) {
            data = process_customer_line(line_to_process, delimiter);
            search_for_customer_and_print(Customer, data);
        }
    }
    data_file.close();
}


void insert_another_account(BTree Customers, const std::string& new_acc) {
    BTree::SearchResult search_result;
    char delimiter = '|';
    std::vector<std::string> new_account;
    new_account = process_line(new_acc, delimiter);
    long long int key = std::stoll(new_account[1]);
    BTree::AccountData account_data;
    account_data.data.emplace_back(std::stoll(new_account[0]));
    account_data.data.emplace_back(std::stoi(new_account[2]));
    account_data.data.emplace_back(std::stof(new_account[3]));
    search_result = Customers.search_key(key);
    int i = 0;
    while (key != search_result.node->keys[i].key) {
        i++;
    }
    int count = 0;
    while (count != search_result.node->keys[i].account_data.size()) {
        if (search_result.node->keys[i].account_data[count].data[0] == account_data.data[0]) {
            std::cout << "Error: Account exists! " << std::endl;
            return;
        }
        count++;
    }
    search_result.node->keys[i].account_data.push_back(account_data);
}


int main() {
    std::string str_one = "43000000003|4300000004|4300000001|Joshua Fowle Emergency Expenses|2|7625683.59";
    std::string str_two = "43000000123|4300000001|4300000001|Joshua Fowle Savings Account|4|4545.05";
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
                customer_search(Customers);
                break;
            case 4:
                std::cout << "Inserting new record..." << std::endl;
                insert_another_account(Customers, str_one);
                insert_another_account(Customers, str_two);
                break;
            case 5:
                std::cout << "Deleting record..." << std::endl;
                break;
            case 6:
                std::cout << "Deleting all customer records..." << std::endl;
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