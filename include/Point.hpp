#ifndef POINT_HPP
#define POINT_HPP


typedef unsigned int uint;


struct Point {
    Point(uint x, uint y) : x(x), y(y) {}
    Point() : x(0), y(0) {}

    uint x;
    uint y;
};


#endif //POINT_HPP