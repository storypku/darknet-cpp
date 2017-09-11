/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Render detection bounding boxes on opencv images
 */

#include "utils.hpp"

using namespace Darknet;

void Darknet::image_overlay(const std::vector<Detection> detections, cv::Mat& image)
{
    const int text_origin_offset_x = 10;
    const int text_origin_offset_y = -10;
    // TODO: provide more colors
    const std::vector<cv::Scalar> colors(  {cv::Scalar(255,0,255),
                                            cv::Scalar(255,0,0),
                                            cv::Scalar(255,255,0),
                                            cv::Scalar(0,255,0),
                                            cv::Scalar(0,255,255),
                                            cv::Scalar(0,0,255)} );
    int number_of_colors = colors.size();

    for (auto detection : detections) {
        cv::Point left_top(     detection.x - (detection.width / 2),
                                detection.y - (detection.height / 2));
        cv::Point right_bottom( detection.x + (detection.width / 2),
                                detection.y + (detection.height / 2));
        cv::Point text_orig(    left_top.x + text_origin_offset_x,
                                right_bottom.y + text_origin_offset_y);

        if (left_top.x < 0) left_top.x = 0;
        if (left_top.y < 0) left_top.y = 0;
        if (right_bottom.x > image.cols) right_bottom.x = image.cols - 1;
        if (right_bottom.y > image.rows) right_bottom.y = image.rows - 1;
        if (text_orig.x < text_origin_offset_x) text_orig.x = text_origin_offset_x;
        if (text_orig.y > image.rows + text_origin_offset_y) text_orig.y = image.rows + text_origin_offset_y;

        int thickness = image.rows * 0.012;
        cv::Scalar color(colors[detection.label_index % number_of_colors]);
        std::string text(std::to_string(static_cast<int>((detection.probability * 100))) + "% " + detection.label);

        cv::rectangle(image, left_top, right_bottom, color, thickness);
        cv::putText(image, text, text_orig, cv::FONT_HERSHEY_SIMPLEX, 1, color, thickness >> 1);
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
