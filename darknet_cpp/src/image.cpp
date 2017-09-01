/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Image in darknet format, can be constructed using several other given formats
 */

#include "image.hpp"
#include "logging.hpp"

using namespace Darknet;

Image::Image() :
    width(0),
    height(0),
    channels(0),
    data(nullptr)
{
}

void Image::operator=(const Image & image)
{
    _data = image._data.clone();
    data = reinterpret_cast<float *>(_data.data);
    width = image.width;
    height = image.height;
    channels = image.channels;
}
