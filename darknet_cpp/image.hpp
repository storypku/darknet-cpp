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

    /*
     *  Constructors
     */
    Image();
    Image(cv::Mat& image);

    /*
     *  Set image content
     *  Conversion does not resize the image, make sure the input image
     *  has already the same size and channels as the network input layer.
     */
    bool set(cv::Mat& image);

    /*
     *  readonly fields
     */
    float* data;
    int width;
    int height;
    int channels;

private:
    cv::Mat m_floatMat;
};

}
#endif /* IMAGE_HPP */
