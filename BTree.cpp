#include "BTree.h"
#include <iostream>
#include <stack>

void BTree::insert(int key) {
    Node* current = root;
    Node* parent = nullptr;
    if (is_node_full(root, level)) {
        Node* new_root = new Node(level);
        new_root->children[0] = root;
        new_root->is_leaf = false;
        root = new_root;
        current = new_root;
    }
    while (!current->is_leaf) {
        int i = current->num_keys - 1;
        while (i >= 0 && key < current->keys[i]) {
            current->keys[i + 1] = current->keys[i];
            i--;
        }
        parent = current;
        current = current->children[i + 1];
        if (is_node_full(current, level)) {
            Node *child = current;
            Node* new_child = new Node(level);

            parent->keys[i] = child->keys[level / 2];
            parent->num_keys++;

            new_child->num_keys = level / 2;
            for (int j = 0; j < level / 2; j++) {
                new_child->keys[j] = child->keys[j + level / 2 + 1];
            }
            child->num_keys = level / 2;

            while (i >= 0 && key < parent->keys[i]) {
                i--;
            }
            current = parent->children[i + 1] = new_child;
        }
    }
    int i = current->num_keys - 1;
    while (i >= 0 && key < current->keys[i]) {
        current->keys[i + 1] = current->keys[i];
        i--;
    }
    current->keys[i + 1] = key;
    current->num_keys++;
}

bool BTree::is_node_full(BTree::Node *node, int level) const{
    if (node->num_keys == level - 1) {
        return true;
    }
    return false;
}

BTree::BTree(int level) : level(level){
    root = new Node(level);
}

void BTree::delete_tree(Node* node) {
    if (!root) {
        return;
    }
    std::stack<Node*> stack_node;
    stack_node.push(root);
    while (!stack_node.empty()) {
        Node* current  = stack_node.top();
        stack_node.pop();
        for (int i = 0; current->num_keys; i++) {
            if (current->children[i]) {
                stack_node.push(current->children[i]);
            }
        }
        delete[] current->keys;
        delete[] current->children;
        delete[] current;
    }
    root = nullptr;
}

