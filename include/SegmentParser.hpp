#ifndef SEGMENT_PARSER_HPP
#define SEGMENT_PARSER_HPP

#include <vector>
#include <iostream>
#include "Segment.hpp"
#include "Event.hpp"


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