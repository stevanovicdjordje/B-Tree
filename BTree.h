#ifndef ASP2DZ2_BTREE_H
#define ASP2DZ2_BTREE_H

#include <iostream>
#include <vector>
#include <variant>
#include <stack>

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
        Node* parent;
        std::vector<Key> keys;
        std::vector<Node *> children;
        bool is_leaf;
        int num_keys;

        explicit Node(int level) : is_leaf(true), num_keys(0), parent(nullptr) {
            keys.reserve(level);
            children.reserve(level);
        }
    };

public:
    struct SearchResult {
        Node* node;
        std::stack<Node*> parent_stack;
        int steps;
        bool isFound;
        SearchResult() : node(), isFound (), steps() {}
    };
    Node *root;
    int level;

    BTree() {};
    BTree(int level);
    static void vector_empty(Node *node, int level);
    static bool is_node_full(Node *node, int level);
    void insert(long long int key, AccountData &account_data);
    void insert_in_non_full(Node *node, AccountData &account_data, long long int key) const;
    static int find_insert_index(Node* node, long long int key);
    void split(Node *current, Node *parent, int index);
    static int binary_search(std::vector<Node::Key> keys, long long int key, int size);
    BTree::SearchResult search_key(long long int key) const;
    void print_tree(Node *node) const;
    void delete_tree(Node* node);
    void delete_key(long long int key) const;
};

#endif //ASP2DZ2_BTREE_H
