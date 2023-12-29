#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include "Point.hpp"


struct Segment {
    Segment(Point start, Point end) : start(start), end(end) {}
    Segment() {}

    bool is_horizontal() { return start.y == end.y; }
    bool is_vertical() { return start.x == end.x; }

    Point start;
    Point end;
};


#endif //SEGMENT_HPP