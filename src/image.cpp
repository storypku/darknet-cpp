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
    m_data(0)
{
    data = &m_data[0];
}

void Image::resize(int w, int h, int c)
{
    if (m_data.empty() || w != width || h != height || c != channels) {
        m_data.resize(w * h * c);
        data = &m_data[0];
        width = w;
        height = h;
        channels = c;
    }
}

void Image::operator=(const Image & image)
{
    m_data = image.m_data;
    data = &m_data[0];
    width = image.width;
    height = image.height;
    channels = image.channels;
}
