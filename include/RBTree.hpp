#ifndef RB_TREE
#define RB_TREE


typedef unsigned int uint;


class RBTree {
public:
    struct Node;
    typedef Node* NodePtr;

    enum Color {
        RED,
        BLACK
    };

    struct Node {
        uint key;
        size_t size;
        Color color;
        NodePtr parent;
        NodePtr left;
        NodePtr right;
    };

    static const NodePtr NIL;

    RBTree();
    ~RBTree();

    NodePtr get_root() const { return this->root; }

    void insert(uint key);
    void remove(uint key);

    size_t elements_in_range(uint lb, uint rb) const;

private:
    static Node NILNode;

    NodePtr root;

    void left_rotate(NodePtr x);
    void right_rotate(NodePtr y);
    void transplant(NodePtr u, NodePtr v);

    void insert_fixup(NodePtr z);
    void remove_fixup(NodePtr x);

    size_t elements_in_tree(NodePtr root) const;

    void delete_tree(NodePtr root);
};


#endif //RB_TREE