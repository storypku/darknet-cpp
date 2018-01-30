/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Image in darknet format, can be constructed using several other given formats
 */

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <opencv2/opencv.hpp>

namespace Darknet
{

class Image
{
public:
    Image();

    void resize(int w, int h, int c);
    void operator=(const Image & image);

    int width;
    int height;
    int channels;
    float *data;

private:
    std::vector<float> m_data;
};

}
#endif /* IMAGE_HPP */
