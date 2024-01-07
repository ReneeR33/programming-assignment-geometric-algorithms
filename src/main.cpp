#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include "RBTree.hpp"
#include "Event.hpp"
#include "SegmentParser.hpp"


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

    uint intersections = 0;
    RBTree tree;

    for (auto& event : events) {
        switch (event.type)
        {
        case Event::Type::VERTICAL_TOP:
            tree.insert(event.point.x);
            break;
        
        case Event::Type::VERTICAL_BOTTOM:
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