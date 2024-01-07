#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>


#ifdef _WIN32
typedef unsigned int uint;
#endif


#ifndef POINT_HPP
#define POINT_HPP


struct Point {
    Point(uint x, uint y) : x(x), y(y) {}
    Point() : x(0), y(0) {}

    uint x;
    uint y;
};


#endif //POINT_HPP


#ifndef SEGMENT_HPP
#define SEGMENT_HPP


struct Segment {
    Segment(Point start, Point end) : start(start), end(end) {}
    Segment() {}

    bool is_horizontal() { return start.y == end.y; }
    bool is_vertical() { return start.x == end.x; }

    Point start;
    Point end;
};


#endif //SEGMENT_HPP


#ifndef EVENT_HPP
#define EVENT_HPP


struct Event {
    enum Type {
        HORIZONTAL,
        VERTICAL_TOP,
        VERTICAL_BOTTOM
    };

    Type type;
    Point point;
    Segment* segment;
};


#endif //EVENT_HPP


#ifndef SEGMENT_PARSER_HPP
#define SEGMENT_PARSER_HPP

#include <vector>
#include <iostream>


class SegmentParser
{
public:
    void parse(
        std::istream &input_stream, 
        std::vector<Segment> &hs_out, 
        std::vector<Segment> &vs_out, 
        std::vector<Event> &e_out
    );
};


#endif //SEGMENT_PARSER_HPP


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


int main() {
    SegmentParser parser;

    std::vector<Segment> horizontal_segments;
    std::vector<Segment> vertical_segments;
    std::vector<Event> events;

    parser.parse(std::cin, horizontal_segments, vertical_segments, events);
    
    std::sort(events.begin(), events.end(), [](const Event& l, const Event& r) {
        return (l.point.y > r.point.y) || 
               (l.point.y == r.point.y && 
                    ((l.type == Event::Type::VERTICAL_TOP && r.type == Event::Type::HORIZONTAL) ||
                     (l.type == Event::Type::HORIZONTAL && r.type == Event::Type::VERTICAL_BOTTOM) ||
                     (l.type == Event::Type::VERTICAL_TOP && r.type == Event::Type::VERTICAL_BOTTOM)));
    });

    #ifdef DEBUG
    for (auto& event : events) {
        switch (event.type)
        {
        case Event::Type::VERTICAL_TOP:
            std::cout << "Added vertical start event: (" << event.point.x << ", " << event.point.y << ")" << std::endl;
            break;
        
        case Event::Type::VERTICAL_BOTTOM:
            std::cout << "Added vertical end event: (" << event.point.x << ", " << event.point.y << ")" << std::endl;
            break;

        case Event::Type::HORIZONTAL:
            std::cout << "Added horizontal event: (" << event.point.x << ", " << event.point.y << ")" << std::endl;
            break;
        
        default:
            break;
        }
    }
    #endif

    uint intersections = 0;
    RBTree tree;

    for (auto& event : events) {
        switch (event.type)
        {
        case Event::Type::VERTICAL_TOP:
            //std::cout << "Adding " << event.point.x << '\n';
            /*if (event.point.x == 0) {
                std::cout << "Adding " << event.point.x << '\n';
            }*/
            tree.insert(event.point.x);
            break;
        
        case Event::Type::VERTICAL_BOTTOM:
            //std::cout << "Removing " << event.point.x << '\n';
            tree.remove(event.point.x);
            break;

        case Event::Type::HORIZONTAL:
            intersections += tree.elements_in_range(event.segment->start.x, event.segment->end.x);
            break;
        
        default:
            break;
        }
    }

    std::cout << intersections << std::endl;
}


void SegmentParser::parse(
        std::istream &input_stream, 
        std::vector<Segment> &hs_out, 
        std::vector<Segment> &vs_out, 
        std::vector<Event> &e_out
    ) {
    enum State {
        R_HORIZONTAL,
        R_VERTICAL,
        NONE
    };

    hs_out.clear();
    vs_out.clear();
    e_out.clear();

    std::array<size_t, 2> space_indices;
    std::array<std::string, 3> line_substrings;
    uint lines_remaining;

    State state = NONE;
    bool read_horizontal = false;
    bool read_vertical = false;
    std::string line;

    while((!read_horizontal || !read_vertical)) {
        std::getline(input_stream, line);

        if (line.empty()) {
            throw std::runtime_error("InputParser: Invalid input");
        }

        space_indices[0] = line.find(' ');
        space_indices[1] = line.find(' ', space_indices[0] + 1);

        line_substrings[0] = line.substr(0, space_indices[0]);
        line_substrings[1] = line.substr(space_indices[0] + 1, space_indices[1] - space_indices[0] - 1);
        line_substrings[2] = line.substr(space_indices[1] + 1, line.size() - space_indices[1] - 1);

        if (line[0] == 'h') {
            // stoi? uint?
            lines_remaining = std::stoul(line_substrings[2]);
            hs_out.resize(lines_remaining);
            state = R_HORIZONTAL;
        } else if (line[0] == 'v') {
            lines_remaining = std::stoul(line_substrings[2]);
            vs_out.resize(lines_remaining);
            state = R_VERTICAL;
        } else {
            int index = lines_remaining - 1;

            switch (state) {
            case R_HORIZONTAL:
                {
                    uint x_start = std::stoul(line_substrings[0]);
                    uint x_end = std::stoul(line_substrings[1]);
                    uint y_pos = std::stoul(line_substrings[2]);

                    hs_out[index] = Segment(
                        Point(x_start, y_pos),
                        Point(x_end, y_pos)
                    );

                    e_out.push_back({
                        .type = Event::Type::HORIZONTAL,
                        .point = hs_out[index].end,
                        .segment = &hs_out[index]
                    });

                    lines_remaining--;
                    if (lines_remaining <= 0) {
                        read_horizontal = true;
                    }
                }
                break;

            case R_VERTICAL:
                {
                    uint x_pos = std::stoul(line_substrings[0]);
                    uint y_start = std::stoul(line_substrings[1]);
                    uint y_end = std::stoul(line_substrings[2]);

                    vs_out[index] = Segment(
                        Point(x_pos, y_start),
                        Point(x_pos, y_end)
                    );

                    e_out.push_back({
                        .type = Event::Type::VERTICAL_BOTTOM,
                        .point = vs_out[index].start,
                        .segment = &vs_out[index]
                    });
                    e_out.push_back({
                        .type = Event::Type::VERTICAL_TOP,
                        .point = vs_out[index].end,
                        .segment = &vs_out[index]
                    });

                    lines_remaining--;
                    if (lines_remaining <= 0) {
                        read_vertical = true;
                    }
                }
                break;
            
            default:
                throw std::runtime_error("InputParser: Invalid input");
                break;
            }
        }
    }
}


RBTree::Node RBTree::NILNode = {
    .key = 0,
    .size = 0,
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

void RBTree::insert(uint key) {
    NodePtr x = this->root;
    NodePtr y = NIL;
    
    while (x != NIL) {
        x->size++;
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
    z->size = 1;

    if (y == NIL) {
        this->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    insert_fixup(z);
}

void RBTree::remove(uint key) {
    if (this->root == NIL) {
        throw std::runtime_error("function RBTree::remove could not delete element, tree was empty");
    }

    //TODO: handle duplicates(?)
    NodePtr z = this->root;
    while (z != NIL && z->key != key) {
        z->size--;
        if (key < z->key) {
            z = z->left;
        } else {
            z = z->right;
        }
    }

    if (z == NIL) {
        throw std::runtime_error("function RBTree::remove could not delete element, element " + std::to_string(key) + " not found in tree");
    }

    z->size--;

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
        //y = minimum(z->right);
        NodePtr y = z->right;
        while (y->left != NIL) {
            y->size--;
            y = y->left;
        }

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
        y->size = z->size;
    }

    if (y_original_color == Color::BLACK) {
        remove_fixup(x);
    }

    NIL->parent = NIL;
    delete z;
}

size_t RBTree::elements_in_range(uint lb, uint rb) const {
    size_t result = 0;

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

    x->size = 1 + x->left->size + x->right->size;
    y->size = 1 + y->left->size + y->right->size;
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
        x->right->parent = y;
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

    y->size = 1 + y->left->size + y->right->size;
    x->size = 1 + x->left->size + x->right->size;
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

size_t RBTree::elements_in_tree(NodePtr root) const {
    /*if (root == NIL) {
        return 0;
    }

    return 1 + elements_in_tree(root->left) + elements_in_tree(root->right);
    */
   return root->size;
}

void RBTree::delete_tree(NodePtr root) {
    if (root == NIL) {
        return;
    }

    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
}