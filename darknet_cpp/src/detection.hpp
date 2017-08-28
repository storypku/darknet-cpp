/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Detection structure
 */

#ifndef DETECTION_HPP
#define DETECTION_HPP

#include <string>

namespace Darknet
{
    struct Detection
    {
        float x;            // box relative center x position
        float y;            // box relative center y position
        float width;        // box relative width
        float height;       // box relative height
        float probability;  // label probability
        int label_index;    // label index that matches the labels list (starts with 0)
        std::string label;  // label name
    };
}

#endif /* DETECTION_HPP */
