#ifndef EVENT_HPP
#define EVENT_HPP

#include "Segment.hpp"


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