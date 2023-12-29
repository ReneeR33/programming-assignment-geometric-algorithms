#include "SegmentParser.hpp"

#include <array>


std::vector<Segment> SegmentParser::parse(std::istream &input_stream) {
    enum State {
        R_HORIZONTAL,
        R_VERTICAL,
        NONE
    };

    std::vector<Segment> result;

    std::array<size_t, 2> space_indices;
    std::array<std::string, 3> line_substrings;
    uint lines_remaining;

    State state = NONE;
    bool read_horizontal, read_vertical = false;
    std::string line;

    while((!read_horizontal || !read_vertical)) {
        std::getline(input_stream, line);

        if (line.empty()) {
            throw std::runtime_error("InputParser: Invalid input");
        }

        space_indices[0] = line.find(' ');
        space_indices[1] = line.find(' ', space_indices[0] + 1);

        line_substrings[0] = line.substr(0, space_indices[0]);
        line_substrings[1] = line.substr(space_indices[0] + 1, space_indices[1] - space_indices[0] - 1);
        line_substrings[2] = line.substr(space_indices[1] + 1, line.size() - space_indices[1] - 1);

        if (line[0] == 'h') {
            lines_remaining = std::stoi(line_substrings[2]);
            state = R_HORIZONTAL;
        } else if (line[0] == 'v') {
            lines_remaining = std::stoi(line_substrings[2]);
            state = R_VERTICAL;
        } else {
            switch (state) {
            case R_HORIZONTAL:
                {
                    int x_start = std::stoi(line_substrings[0]);
                    int x_end = std::stoi(line_substrings[1]);
                    int y_pos = std::stoi(line_substrings[2]);

                    result.push_back(Segment(
                        Point(x_start, y_pos),
                        Point(x_end, y_pos)
                    ));

                    lines_remaining--;
                    if (lines_remaining <= 0) {
                        read_horizontal = true;
                    }
                }
                break;

            case R_VERTICAL:
                {
                    int x_pos = std::stoi(line_substrings[0]);
                    int y_start = std::stoi(line_substrings[1]);
                    int y_end = std::stoi(line_substrings[2]);

                    result.push_back(Segment(
                        Point(x_pos, y_start),
                        Point(x_pos, y_end)
                    ));

                    lines_remaining--;
                    if (lines_remaining <= 0) {
                        read_vertical = true;
                    }
                }
                break;
            
            default:
                throw std::runtime_error("InputParser: Invalid input");
                break;
            }
        }
    }

    return result;
}