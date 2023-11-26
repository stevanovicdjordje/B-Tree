#ifndef ASP2DZ2_BTREE_H
#define ASP2DZ2_BTREE_H

#include <iostream>
#include <vector>
#include <variant>

class BTree {
public:
    struct AccountData {
        std::vector<std::variant<long long int, float, int>> data;

        AccountData() : data() {}
    };

private:
    struct Node {
        struct Key {
            long long int key;
            std::vector<AccountData> account_data;
        };

        std::vector<Key> keys;
        std::vector<Node *> children;
        bool is_leaf;
        int num_keys;

        explicit Node(int level) : is_leaf(true), num_keys(0) {
            keys.resize(level - 1);
            children.resize(level, nullptr);
        }
    };

public:
    Node *root;
    int level;

    BTree() {};

    BTree(int level);

    void root_split(Node *new_node, Node *node, long long int key);

    static void insert_element_in_new_node(Node *node, Node *new_node, int index);

    static void remove_from_node(Node *node, int index);

    static void vector_empty(Node *node, int level);

    static int get_number_of_keys(Node *node);

    static bool is_node_full(Node *node, int level);

    void insert(long long int key, AccountData &account_data);

    void insert_in_non_full(Node *node, AccountData &account_data, long long int key) const;

    bool split(Node *current, Node *parent, long long int key, AccountData &account_data);

    void delete_tree(Node *node);

    static int binary_search(std::vector<Node::Key> keys, long long int key, int size);

    bool search_key(long long int key, AccountData &account_data) const;

    void print_tree(Node *node) const;

    ~BTree() { delete_tree(root); }
};

#endif //ASP2DZ2_BTREE_H
