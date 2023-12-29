#include "RBTree.hpp"
#include <stdexcept>
#include <string>
// for debugging
//#include <iostream>
//#define DEBUG


RBTree::Node RBTree::NILNode = {
    .key = 0,
    .color = RBTree::Color::BLACK,
    .parent = &NILNode,
    .left = &NILNode,
    .right = &NILNode
};

const RBTree::NodePtr RBTree::NIL = &NILNode;


RBTree::RBTree() {
    this->root = NIL;
}

RBTree::~RBTree() {
    delete_tree(this->root);
}

void RBTree::insert(int key) {
    NodePtr x = this->root;
    NodePtr y = NIL;
    
    while (x != NIL) {
        y = x;
        if (key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    NodePtr z = new Node();
    z->key = key;
    z->color = Color::RED;
    z->parent = y;
    z->left = NIL;
    z->right = NIL;

    if (y == NIL) {
        this->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    insert_fixup(z);
}

void RBTree::remove(int key) {
    if (this->root == NIL) {
        throw std::runtime_error("function RBTree::remove could not delete element, tree was empty");
    }

    //TODO: handle duplicates(?)
    NodePtr z = this->root;
    while (z != NIL && z->key != key) {
        if (key < z->key) {
            z = z->left;
        } else {
            z = z->right;
        }
    }

    if (z == NIL) {
        throw std::runtime_error("function RBTree::remove could not delete element, element " + std::to_string(key) + " not found in tree");
    }

    NodePtr x, y;
    y = z;
    Color y_original_color = y->color;

    if (z->left == NIL) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == NIL) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y != z->right) {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        } else {
            // in case x is T.nil
            x->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_original_color == Color::BLACK) {
        remove_fixup(x);
    }

    NIL->parent = NIL;
    delete z;
}

int RBTree::elements_in_range(int lb, int rb) {
    int result = 0;

    NodePtr split = this->root;
    NodePtr p = NIL;

    while (split != NIL && !(lb < split->key && rb >= split->key)) {
        p = split;

        if (split->key >= lb && split->key <= rb) {
            result++;
        }

        if (lb < split->key) {
            split = split->left;
        } else {
            split = split->right;
        }
    }

    if (split == NIL && p == NIL) {
        return 0;
    } else if (split == NIL && p != NIL) {
        #ifdef DEBUG
        std::cout << "didn't find a split value... last element before nill in search path: " << p->key << std::endl;
        #endif
        return result;
    }

    #ifdef DEBUG
    std::cout << "split: " << split->key << std::endl;
    #endif

    // also count the split node
    result++;

    NodePtr x = split->left;
    while (x != NIL)
    {
        if (lb < x->key) {
            result++;
            result += elements_in_tree(x->right);
            x = x->left;
        } else {
            if (lb == x->key) {
                result++;
            }
            x = x->right;
        }
    }
    x = split->right;
    while (x != NIL)
    {
        if (rb >= x->key) {
            result++;
            result += elements_in_tree(x->left);
            x = x->right;
        } else {
            x = x->left;
        }
    }

    return result;
}

void RBTree::left_rotate(NodePtr x) {
    if (x == NIL) {
        throw std::runtime_error("input x for function RBTree::left_rotate cannot be NIL");
    } else if (x == nullptr) {
        throw std::runtime_error("input x for function RBTree::left_rotate cannot be nullptr");
    }

    NodePtr y = x->right;
    if (y == NIL) {
        throw std::runtime_error("right child y of input x in function RBTree::left_rotate cannot be NIL");
    } else if (y == nullptr) {
        throw std::runtime_error("right child y of input x in function RBTree::left_rotate cannot be nullptr");
    }

    x->right = y->left;
    if (y->left != NIL) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == NIL) {
        this->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void RBTree::right_rotate(NodePtr y) {
    if (y == NIL) {
        throw std::runtime_error("input y for function RBTree::right_rotate cannot be NIL");
    } else if (y == nullptr) {
        throw std::runtime_error("input y for function RBTree::right_rotate cannot be nullptr");
    }

    NodePtr x = y->left;
    if (x == NIL) {
        throw std::runtime_error("left child x of input y in function RBTree::right_rotate cannot be NIL");
    } else if (x == nullptr) {
        throw std::runtime_error("left child x of input y in function RBTree::right_rotate cannot be nullptr");
    }

    y->left = x->right;
    if (x->right != NIL) {
        x->right->parent = x;
    }

    x->parent = y->parent;
    if (y->parent == NIL) {
        this->root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
}

void RBTree::transplant(NodePtr u, NodePtr v) {
    if (u == NIL) {
        return;
    }

    if (u->parent == NIL) {
        this->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    v->parent = u->parent;
}

void RBTree::insert_fixup(NodePtr z) {
    while (z->parent->color == Color::RED) {
        if (z->parent == z->parent->parent->left) {
            NodePtr y = z->parent->parent->right;

            if (y->color == Color::RED) {
                z->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(z);
                }
                z->parent->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                right_rotate(z->parent->parent);
            }
        } else {
            NodePtr y = z->parent->parent->left;

            if (y->color == Color::RED) {
                z->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(z);
                }
                z->parent->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                left_rotate(z->parent->parent);
            }
        }
    }

    this->root->color = Color::BLACK;
}

void RBTree::remove_fixup(NodePtr x) {
    NodePtr w;
    while (x != this->root && x->color == Color::BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == Color::RED) {
                w->color = Color::BLACK;
                x->parent->color = Color::RED;
                left_rotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                w->color = Color::RED;
                x = x->parent;
            } else {
                if (w->right->color == Color::BLACK) {
                    w->left->color = Color::BLACK;
                    w->color = Color::RED;
                    right_rotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = Color::BLACK;
                w->right->color = Color::BLACK;
                left_rotate(x->parent);
                x = this->root;
            }
        } else {
            w = x->parent->left;
            if (w->color == Color::RED) {
                w->color = Color::BLACK;
                x->parent->color = Color::RED;
                right_rotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
                w->color = Color::RED;
                x = x->parent;
            } else {
                if (w->left->color == Color::BLACK) {
                    w->right->color = Color::BLACK;
                    w->color = Color::RED;
                    left_rotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = Color::BLACK;
                w->left->color = Color::BLACK;
                right_rotate(x->parent);
                x = this->root;
            }
        }
    }
    x->color = Color::BLACK;
}

int RBTree::elements_in_tree(NodePtr root) {
    if (root == NIL) {
        return 0;
    }

    return 1 + elements_in_tree(root->left) + elements_in_tree(root->right);
}

RBTree::NodePtr RBTree::minimum(NodePtr root) {
    NodePtr result = root;
    while (result->left != NIL) {
        result = result->left;
    }

    return result;
}

void RBTree::delete_tree(NodePtr root) {
    if (root == NIL) {
        return;
    }

    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
}