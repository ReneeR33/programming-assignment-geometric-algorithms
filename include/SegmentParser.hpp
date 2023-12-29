#ifndef SEGMENT_PARSER_HPP
#define SEGMENT_PARSER_HPP

#include <vector>
#include <iostream>
#include "Segment.hpp"


class SegmentParser
{
public:
    std::vector<Segment> parse(std::istream &input_stream);
};


#endif //SEGMENT_PARSER_HPP