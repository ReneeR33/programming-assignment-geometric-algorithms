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
        //does not work!
        uint count;
        uint elements_in_subtree;
        Color color;
        NodePtr parent;
        NodePtr left;
        NodePtr right;
    };

    static const NodePtr NIL;

    RBTree();
    ~RBTree();

    void insert(uint key);
    void remove(uint key);

    uint elements_in_range(uint lb, uint rb) const;

private:
    static Node NILNode;

    NodePtr root;

    void left_rotate(NodePtr x);
    void right_rotate(NodePtr y);
    void transplant(NodePtr u, NodePtr v);

    void insert_fixup(NodePtr z);
    void remove_fixup(NodePtr x);

    uint elements_in_tree(NodePtr root) const;

    void delete_tree(NodePtr root);
};


#endif //RB_TREE