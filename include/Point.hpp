#ifndef POINT_HPP
#define POINT_HPP


struct Point {
    Point(int x, int y) : x(x), y(y) {}
    Point() : x(0), y(0) {}

    int x;
    int y;
};


#endif //POINT_HPP