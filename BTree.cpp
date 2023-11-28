#include "BTree.h"
#include <stack>
#include <queue>
#include <cmath>

static int steps;

bool BTree::is_node_full(BTree::Node *node, int level) {
    if (node->num_keys == level) {
        return true;
    }
    return false;
}

BTree::BTree(int level) : level(level) {
    root = new Node(level);
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
            if (!current->is_leaf) {
                for (int i = 0; i < current->num_keys + 1; i++) {
                    node_queue.push(current->children[i]);
                }
            }
        }
    }
}

void BTree::insert(long long int key, BTree::AccountData &account_data) {
    SearchResult search_result = search_key(key);
    if (search_result.isFound) {
        for (int i = 0; i < search_result.node->num_keys; i++) {
            if (key == search_result.node->keys[i].key) {
                search_result.node->keys[i].account_data.push_back(account_data);
                return;
            }
        }
    }

    while (!search_result.isFound) {
        Node *current = search_result.node;
        Node *parent = current->parent;
        insert_in_non_full(current, account_data, key);
        if (is_node_full(current, level)) {
            if (parent) {
                int index = find_insert_index(parent, key);
                split(current, parent, index);
                break;
            }else {
                int index = 0;
                split(current, parent, index);
                break;
            }
        } else {
            break;
        }
    }
}
int BTree::find_insert_index(Node *node, long long int key) {
    if (node == nullptr) {
        return 0;  // If the current node is nullptr, return 0 (insert at the beginning)
    }
    int index = 0;
    while (index < node->num_keys && key > node->keys[index].key) {
        index++;
    }
    return index;
}


BTree::SearchResult BTree::search_key(long long int key) const {
    SearchResult search_result;
    std::stack<Node *> tree_stack;
    tree_stack.push(root);

    while (!tree_stack.empty()) {
        Node *current = tree_stack.top();
        tree_stack.pop();

        if (current->num_keys != 0) {
            int index = binary_search(current->keys, key, current->num_keys);

            if (index != -1) {
                search_result.node = current;
                search_result.isFound = true;
                search_result.steps = steps;
                steps = 0;
                return search_result;
            }

            if (!current->is_leaf) {
                int last_index = current->num_keys - 1;
                int first_index = 0;
                if (key > current->keys[last_index].key) {
                    search_result.parent_stack.push(current);
                    tree_stack.push(current->children[last_index + 1]);
                    continue;
                } else if (key < current->keys[first_index].key) {
                    search_result.parent_stack.push(current);
                    tree_stack.push(current->children[first_index]);
                    continue;
                } else {
                    int low = 0;
                    int high = current->num_keys - 1;

                    while (low <= high) {
                        int mid = (low + high) / 2;

                        if (key > current->keys[mid].key && key < current->keys[mid + 1].key) {
                            search_result.parent_stack.push(current);
                            tree_stack.push(current->children[mid + 1]);
                            break;
                        } else if (key < current->keys[mid].key) {
                            high = mid - 1;
                        } else {
                            low = mid + 1;
                        }
                    }
                }

            }
            search_result.node = current;
            search_result.isFound = false;
            steps = 0;
            return search_result;
        }
        search_result.node = root;
        search_result.isFound = false;
        steps = 0;
        return search_result;
    }
    return search_result;
}


int BTree::binary_search(std::vector<Node::Key> keys, long long int key, int size) {
    int low = 0;
    int high = size - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        steps++;
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

void BTree::split(Node *current, Node *parent, int index) {
    Node *left_child = current;
    Node *right_child = new Node(level);

    right_child->keys.insert(right_child->keys.begin(), left_child->keys.begin() + (level / 2 + 1), left_child->keys.end());
    left_child->keys.erase(left_child->keys.begin() + (level / 2 + 1), left_child->keys.end());

    if (!left_child->is_leaf) {
        right_child->children.insert(right_child->children.begin(), left_child->children.begin() + (level / 2 + 1),
                                     left_child->children.end());
        left_child->children.erase(left_child->children.begin() + (level / 2 + 1), left_child->children.end());

        for (Node *child : right_child->children) {
            if (child != nullptr) {
                child->parent = right_child;
            }
        }
    }

    if (parent != nullptr) {
        parent->keys.insert(parent->keys.begin() + index, left_child->keys.back());
        left_child->keys.pop_back();
    } else {
        Node *newRoot = new Node(level);
        newRoot->keys.push_back(left_child->keys.back());
        left_child->keys.pop_back();

        newRoot->num_keys = newRoot->keys.size();
        newRoot->children.push_back(left_child);
        newRoot->children.push_back(right_child);
        newRoot->is_leaf = false;
        right_child->parent = newRoot;
        left_child->parent = newRoot;

        root = newRoot;
        left_child->num_keys = left_child->keys.size();
        right_child->num_keys = right_child->keys.size();
        if (!left_child->is_leaf) {
            for (Node *child : newRoot->children) {
                if (child != nullptr) {
                    child->parent = newRoot;
                }
            }
        }

        if (is_node_full(newRoot, level)) {
            int root_index = find_insert_index(newRoot, newRoot->keys[0].key);
            split(newRoot, nullptr, root_index);
        }

        return;
    }

    left_child->keys.pop_back();

    right_child->parent = parent;
    left_child->parent = parent;

    parent->children.insert(parent->children.begin() + index + 1, right_child);

    left_child->num_keys = left_child->keys.size();
    right_child->num_keys = right_child->keys.size();

    parent->num_keys = parent->keys.size();

    if (is_node_full(parent, level)) {
        int parent_index = find_insert_index(parent->parent, parent->keys[0].key);
        split(parent, parent->parent, parent_index);
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
        node->keys.resize(level);
    } else if (node->keys.size() != level) {
        node->keys.resize(node->keys.size() + 1);
    } else {
        return;
    }
}

void BTree::delete_tree(BTree::Node *node) {
    if (node == nullptr) {
        return;
    }
    std::stack<Node*> node_stack;
    node_stack.push(node);

    while (!node_stack.empty()) {
        Node* current = node_stack.top();
        node_stack.pop();

        for (int i = 0; i < current->num_keys; i++) {
            if (!current->is_leaf) {
                node_stack.push(current->children[i]);
            }
        }
        delete current;
    }
    root = nullptr;
}


void BTree::delete_key(long long int key) const {
    int minimum = ceil((level / 2) - 1);
    SearchResult search_result = search_key(key);
    if (search_result.isFound) {
        int i = 0;
        while (key != search_result.node->keys[i].key) {
            i++;
        }
        if (search_result.node->num_keys - 1 != minimum) {
            search_result.node->keys.erase(search_result.node->keys.begin() + i);
            search_result.node->num_keys--;
        }else{
            ///
        }
    }else{
        std::cout << "Customer do not have any accounts! " << std::endl;
        return;
    }
}

