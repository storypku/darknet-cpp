/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Utilities for post processing detections
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <opencv2/opencv.hpp>
#include "detection.hpp"

namespace Darknet
{
    /*
     *  Render detection bouding boxes on a given input image
     *  detections: list of detections
     *  image:      image to use to overlay the detections
     *  NOTE:       assumes the aspect ratio of 'image' is the same as the aspect ratio of the detector.
     *              If not, the bounding boxes will not be displayed correctly.
     */
    void image_overlay(const std::vector<Detection> detections, cv::Mat& image);

    void filter_detections(const std::vector<Detection> input, std::vector<Detection>& output, std::vector<std::string> include);
}
#endif /* UTILS_HPP */
