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