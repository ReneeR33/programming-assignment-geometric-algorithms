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
    auto segments = parser.parse(std::cin);

    std::vector<Event> events;

    for (auto& segment : segments) {
        Event::Type type;

        if (segment.is_vertical()) {
            type = Event::Type::VERTICAL_BOTTOM;
            events.push_back({
                .type = type,
                .point = segment.start,
                .segment = &segment
            });
            type = Event::Type::VERTICAL_TOP;
        } else {
            type = Event::Type::HORIZONTAL;
        }

        events.push_back({
            .type = type,
            .point = segment.end,
            .segment = &segment
        });
    }

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

    int intersections = 0;
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