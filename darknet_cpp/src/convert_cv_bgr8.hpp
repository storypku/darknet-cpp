/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Image converter to convert an opencv BGR 8-bit image to a darknet image
 */
#ifndef CONVERT_CV_BGR8_HPP
#define CONVERT_CV_BGR8_HPP

#include "convert_cv.hpp"

namespace Darknet
{

class ConvertCvBgr8 : ConvertCv
{
public:
    /* see ConvertCv for documentation */
    void setup(int in_width, int in_height, int out_width, int out_height);
    bool convert(const cv::Mat& in, Image& out);

private:
    bool m_resize_needed;
    int m_in_width;
    int m_in_height;
    int m_out_width;
    int m_out_height;
    cv::Size m_new_size;
    cv::Mat m_image_resize;
    cv::Mat m_image_resize_roi;
    cv::Mat m_image_rgb;
    cv::Mat m_image_rgb_float;
};

}
#endif /* CONVERT_CV_BGR8_HPP */
