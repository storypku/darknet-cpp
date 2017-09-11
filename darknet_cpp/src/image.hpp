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

    void operator=(const Image & image);

    int width;
    int height;
    int channels;
    float* data;    /* used for reading by detector */
    cv::Mat _data;  /* owns the image data */
};

}
#endif /* IMAGE_HPP */
