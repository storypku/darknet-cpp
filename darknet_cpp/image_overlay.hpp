/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Render detection bounding boxes on opencv images
 */

#ifndef IMAGE_OVERLAY_HPP
#define IMAGE_OVERLAY_HPP

#include <opencv2/opencv.hpp>
#include "detector.hpp"

namespace Darknet
{
    /*
     *  detections: list of detections
     *  image:      image to use to overlay the detections
     *  NOTE:       assumes the aspect ratio of 'image' is the same as the aspect ratio of the detector.
     *              If not, the bounding boxes will not be displayed correctly.
     */
    void image_overlay(std::vector<Detection> detections, cv::Mat& image);
}
#endif /* IMAGE_OVERLAY_HPP */
