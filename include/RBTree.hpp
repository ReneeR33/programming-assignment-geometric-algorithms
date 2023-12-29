#ifndef RB_TREE
#define RB_TREE


class RBTree {
public:
    struct Node;
    typedef Node* NodePtr;

    enum Color {
        RED,
        BLACK
    };

    struct Node {
        int key;
        Color color;
        NodePtr parent;
        NodePtr left;
        NodePtr right;
    };

    static const NodePtr NIL;

    RBTree();
    ~RBTree();

    NodePtr get_root() const { return this->root; }

    void insert(int key);
    void remove(int key);

    int elements_in_range(int lb, int rb);

private:
    static Node NILNode;

    NodePtr root;

    void left_rotate(NodePtr x);
    void right_rotate(NodePtr y);
    void transplant(NodePtr u, NodePtr v);

    void insert_fixup(NodePtr z);
    void remove_fixup(NodePtr x);

    int elements_in_tree(NodePtr root);
    NodePtr minimum(NodePtr root);

    void delete_tree(NodePtr root);
};


#endif //RB_TREE