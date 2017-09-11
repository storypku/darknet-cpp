/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Image converter to convert various opencv image formats to a darknet image
 */
#ifndef CONVERT_CV_HPP
#define CONVERT_CV_HPP

#include "image.hpp"
#include <opencv2/opencv.hpp>

namespace Darknet
{

class ConvertCv
{
public:
    /*
     *  Setup converter to work withfimages of in_width/in_height size
     *  in_width :      width of input images to convert
     *  in_height :     height of input images to convert
     *  out_width :     width of darknet image to produce
     *  out_height :    height of darknet image to produce
     */
    virtual void setup(int in_width, int in_height, int out_width, int out_height) = 0;

    /*
     *  Convert and resize (if needed) an opencv image to a darknet image
     *  in :    opencv input image
     *  out :   darknet output image
     *  returns true on success, false on failure
     */
    virtual bool convert(const cv::Mat& in, Image& out) = 0;
};

}
#endif /* CONVERT_CV_HPP */
