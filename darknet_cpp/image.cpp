/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Image in darknet format, can be constructed using several other given formats
 */

#include "image.hpp"
#include "logging.hpp"

using namespace Darknet;

Image::Image() : 
   data(nullptr),
   width(0),
   height(0),
   channels(0)
{
}

Image::Image(cv::Mat& image)
{
    (void) set(image);
}

bool Image::set(cv::Mat& image)
{
    cv::Mat image_rgb;
    std::vector<cv::Mat> floatMatChannels(3);

    if (image.empty()) {
        EPRINTF("Input image is empty\n");
        return false;
    }

    //Assume 8-bit BGR
    cv::cvtColor(image, image_rgb, CV_BGR2RGB);

    // Convert the bytes to float
    image_rgb.convertTo(m_floatMat, CV_32FC3, 1/255.0);

    // Get the image to suit darknet
    cv::split(m_floatMat, floatMatChannels);
    cv::vconcat(floatMatChannels, m_floatMat);

    data = reinterpret_cast<float *>(m_floatMat.data);
    width = image.cols;
    height = image.rows;
    channels = image.channels();

    return true;
}

void Image::operator=(const Image & image) {
    m_floatMat = image.m_floatMat.clone();
    data = reinterpret_cast<float *>(m_floatMat.data);
    width = image.width;
    height = image.height;
    channels = image.channels;
}
