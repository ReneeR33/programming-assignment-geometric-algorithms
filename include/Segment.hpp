#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include "Point.hpp"


struct Segment {
    Segment(Point start, Point end) : start(start), end(end) {}
    Segment() {}

    Point start;
    Point end;
};


#endif //SEGMENT_HPP