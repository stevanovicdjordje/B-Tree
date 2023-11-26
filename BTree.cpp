#include "BTree.h"
#include <stack>
#include <queue>


bool BTree::is_node_full(BTree::Node *node, int level) {
    if (node->num_keys == level - 1) {
        return true;
    }
    return false;
}

BTree::BTree(int level) : level(level) {
    root = new Node(level);
}

void BTree::delete_tree(Node *node) {
    if (!root) {
        return;
    }
    std::stack<Node *> stack_node;
    stack_node.push(root);
    while (!stack_node.empty()) {
        Node *current = stack_node.top();
        stack_node.pop();
        for (int i = 0; get_number_of_keys(current); i++) {
            if (current->children[i]) {
                stack_node.push(current->children[i]);
            }
        }
        delete[] current;
    }
    root = nullptr;
}

void BTree::insert(long long int key, BTree::AccountData &account_data) {
    bool found = search_key(key, account_data);

    int index = 0;
    std::stack<Node *> tree_stack;
    std::stack<Node *> parents_stack;
    tree_stack.push(root);
    Node *parent = nullptr;

    while (!tree_stack.empty() && !found) {
        Node *current = tree_stack.top();
        tree_stack.pop();

        // LEAF FOUNDED AND NOT FULL //
        if (!is_node_full(current, level) && current->is_leaf) {
            insert_in_non_full(current, account_data, key);
            return;
        }

        // LEAF NOT FOUNDED //
        while (!current->is_leaf) {
            for (int j = 0; j <= current->num_keys - 1; j++) {
                if (key < current->keys[j].key) {
                    index = j;
                } else {
                    index++;
                }
            }
            parent = current;
            tree_stack.push(current->children[index]);
            current = current->children[index];
        }

        // LEAF FOUNDED BUT FULL//
        if (is_node_full(current, level)) {
            if (!parents_stack.empty()) {
                parent = parents_stack.top();
                parents_stack.pop();
            }
            if (split(current, parent, key, account_data)) {
                while (!parents_stack.empty()) {
                    current = parent;
                    parent = parents_stack.top();
                    parents_stack.pop();
                    if (!split(current, parent, key, account_data)) {
                        break;
                    }
                }
            }
        }
    }
}

void BTree::print_tree(BTree::Node *node) const {
    int flag = 1, printed_level = 0;
    std::queue<Node *> node_queue;
    if (!root) {
        return;
    }
    node_queue.push(root);
    while (!node_queue.empty()) {
        Node *current = node_queue.front();
        node_queue.pop();
        for (int i = 0; i < current->num_keys; i++) {
            if (flag) {
                std::cout << "                          ";
                flag = 0;
            }
            if (i == 0) {
                std::cout << current->keys[i].key;
            } else {
                std::cout << "|" << current->keys[i].key;
            }
        }
        if (!node_queue.empty()) {
            std::cout << "    ";
        } else {
            std::cout << "" << std::endl;
            if (printed_level == 0) {
                std::cout << "                ";
                printed_level++;
            } else if (printed_level == 1) {
                std::cout << "           ";
                printed_level++;
            } else {
                std::cout << "     ";
                printed_level++;
            }
            for (int i = 0; i < level; i++) {
                node_queue.push(current->children[i]);
            }
        }
    }
}

bool BTree::search_key(long long int key, AccountData &account_data) const {
    std::stack<Node *> tree_stack;
    tree_stack.push(root);

    while (!tree_stack.empty()) {
        Node *current = tree_stack.top();
        tree_stack.pop();

        if (current->num_keys != 0) {
            int index = binary_search(current->keys, key, current->num_keys);

            if (index != -1) {
                current->keys[index].account_data.push_back(account_data);
                return true;
            }

            if (!current->is_leaf) {
                int last_index = current->num_keys - 1;
                int first_index = 0;
                if (key > current->keys[last_index].key) {
                    int i = level - 1;
                    while (current->children[i] == nullptr) {
                        i--;
                    }
                    tree_stack.push(current->children[i]);
                } else if (key < current->keys[first_index].key) {
                    tree_stack.push(current->children[0]);
                } else {
                    int i = 1;
                    while (current->children[i] == nullptr) {
                        i++;
                    }
                    tree_stack.push(current->children[i]);
                }
            }
        }
    }
    return false;
}


int BTree::binary_search(std::vector<Node::Key> keys, long long int key, int size) {
    int low = 0;
    int high = size - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (keys[mid].key == key) {
            return mid;
        } else if (keys[mid].key < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}


int BTree::get_number_of_keys(BTree::Node *node) {
    return node->keys.size();
}


bool BTree::split(BTree::Node *current, BTree::Node *parent, long long int key, AccountData &account_data) {
    Node *new_node = new Node(level);
    int median_index = level / 2;
    long long int median_element = current->keys[median_index].key;
    long long int previous_median = current->keys[median_index - 1].key;

    if (key > median_element) {
        // MEDIAN GOES TO PARENT
        insert_element_in_new_node(current, new_node, median_index);
        remove_from_node(current, median_index);
        insert_in_non_full(current, account_data, key);

        if (parent == nullptr) {
            root_split(new_node, current, median_element);
            return false;
        }
        return is_node_full(parent, level);
    } else if (previous_median < key && key < median_element) {
        // KEY GOES TO PARENT
        insert_element_in_new_node(current, new_node, median_index);
        remove_from_node(current, median_index);

        if (parent == nullptr) {
            root_split(new_node, current, key);
            return false;
        }
        return is_node_full(parent, level);
    } else {
        // PREVIOUS MEDIAN GOES TO PARENT
        insert_element_in_new_node(current, new_node, median_index - 1);
        remove_from_node(current, median_index);
        insert_in_non_full(new_node, account_data, key);

        if (parent == nullptr) {
            root_split(new_node, current, previous_median);
            return false;
        }
        return is_node_full(parent, level);
    }
}


void BTree::insert_in_non_full(BTree::Node *node, BTree::AccountData &account_data, long long int key) const {
    int i = node->num_keys - 1;
    while (i >= 0 && key < node->keys[i].key) {
        node->keys[i + 1].key = node->keys[i].key;
        i--;
    }
    vector_empty(node, level);
    node->keys[i + 1].key = key;
    node->num_keys++;
    node->keys[i + 1].account_data.push_back(account_data);
}

void BTree::vector_empty(Node *node, int level) {
    if (node->keys.empty()) {
        node->keys.resize(level - 1);
    } else if (node->keys.size() != level - 1) {
        node->keys.resize(node->keys.size() + 1);
    } else {
        return;
    }
}

void BTree::root_split(BTree::Node *new_node, BTree::Node *current, long long int key) {
    Node *new_root = new Node(level);
    new_root->keys[0].key = key;
    new_root->is_leaf = false;
    new_root->children[0] = new_node;
    new_root->children[1] = current;
    new_root->num_keys++;
    root = new_root;
}

void BTree::insert_element_in_new_node(BTree::Node *node, BTree::Node *new_node, int index) {
    for (int i = 0; i < index; i++) {
        new_node->keys[i] = node->keys[i];
        new_node->num_keys++;
    }
}

void BTree::remove_from_node(BTree::Node *node, int index) {
    auto start = node->keys.begin();
    auto end = node->keys.begin() + index + 1;
    node->num_keys--;
    int remove_num_elements = index;
    while (remove_num_elements > 0) {
        node->num_keys--;
        remove_num_elements--;
    }
    node->keys.erase(start, end);
}
