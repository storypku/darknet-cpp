/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Render detection bounding boxes on opencv images
 */

#include "utils.hpp"

using namespace Darknet;

void Darknet::image_overlay(const std::vector<Detection> detections, cv::Mat& image)
{
    const std::vector<cv::Scalar> colors(  {cv::Scalar(255,0,255),
                                            cv::Scalar(255,0,0),
                                            cv::Scalar(255,255,0),
                                            cv::Scalar(0,255,0),
                                            cv::Scalar(0,255,255),
                                            cv::Scalar(0,0,255)} );

    for (auto detection : detections) {
        cv::Point left_top(     image.cols * (detection.x - (detection.width / 2)),
                                image.rows * (detection.y - (detection.height / 2)));
        cv::Point right_bottom( image.cols * (detection.x + (detection.width / 2)),
                                image.rows * (detection.y + (detection.height / 2)));

        if (left_top.x < 0) left_top.x = 0;
        if (left_top.y < 0) left_top.y = 0;
        if (right_bottom.x > image.cols) right_bottom.x = image.cols - 1;
        if (right_bottom.y > image.rows) right_bottom.y = image.rows - 1;

        //TODO: add text labels
        cv::rectangle(image, left_top, right_bottom, colors[detection.label_index % 6], image.rows * 0.012);
    }
}

void Darknet::filter_detections(const std::vector<Detection> input, std::vector<Detection>& output, std::vector<std::string> include)
{
    output.clear();

    for (auto detection : input) {
        for (auto label : include) {
            if (label == detection.label) {
                output.push_back(detection);
            }
        }
    }
}
