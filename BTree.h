#ifndef ASP2DZ2_BTREE_H
#define ASP2DZ2_BTREE_H

class BTree {
    struct Node {
        int *keys;
        Node** children;
        bool is_leaf;
        int num_keys;

        Node (int level) : is_leaf(true), num_keys(0) {
            keys = new int[level - 1];
            children = new Node*[level];
            for (int i = 0; i < level; ++i) {
                children[i] = nullptr;
            }
        }
    };
    Node* root;
    int level;
public:
    BTree(int level);
    bool is_node_full(Node* node, int level) const;
    void print_tree() const;
    void delete_key();
    void insert(int key);
    void delete_tree(Node* node);
    ~BTree() { delete_tree(root);}

};

#endif //ASP2DZ2_BTREE_H
