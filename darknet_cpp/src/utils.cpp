/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Render detection bounding boxes on opencv images
 */

#include "utils.hpp"

using namespace Darknet;

void Darknet::image_overlay(const std::vector<Detection> detections, cv::Mat& image)
{
    const int font_face = cv::FONT_HERSHEY_SIMPLEX;
    const double font_scale = 0.5;
    const int box_thickness = 1;
    const int text_thickness = 1;

    const std::vector<cv::Scalar> colors(  {cv::Scalar(255, 255, 102),
                                            cv::Scalar(102, 255, 224),
                                            cv::Scalar(239, 102, 255),
                                            cv::Scalar(102, 239, 255),
                                            cv::Scalar(255, 102, 178),
                                            cv::Scalar(193, 102, 255),
                                            cv::Scalar(255, 102, 224),
                                            cv::Scalar(102, 193, 255),
                                            cv::Scalar(255, 102, 132),
                                            cv::Scalar(117, 255, 102),
                                            cv::Scalar(255, 163, 102),
                                            cv::Scalar(102, 255, 178),
                                            cv::Scalar(209, 255, 102),
                                            cv::Scalar(163, 255, 102),
                                            cv::Scalar(255, 209, 102),
                                            cv::Scalar(102, 147, 255),
                                            cv::Scalar(147, 102, 255),
                                            cv::Scalar(102, 255, 132),
                                            cv::Scalar(255, 117, 102),
                                            cv::Scalar(102, 102, 255)} );
    int number_of_colors = colors.size();

    for (auto detection : detections) {
        cv::Point left_top(     std::max(0, static_cast<int>(detection.x - (detection.width / 2))),
                                std::max(0, static_cast<int>(detection.y - (detection.height / 2))));
        cv::Point right_bottom( std::min(static_cast<int>(detection.x + (detection.width / 2)), image.cols - 1),
                                std::min(static_cast<int>(detection.y + (detection.height / 2)), image.rows - 1));


        cv::Scalar color(colors[detection.label_index % number_of_colors]);
        std::string text(std::to_string(static_cast<int>(detection.probability * 100)) + "% " + detection.label);

        int baseline;
        cv::Size text_size = cv::getTextSize(text, font_face, font_scale, text_thickness, &baseline);

        /* left bottom origin */
        cv::Point text_orig(    std::min(image.cols - text_size.width - 1, left_top.x),
                                std::max(text_size.height, left_top.y - baseline));


        /* draw bounding box */
        cv::rectangle(image, left_top, right_bottom, color, box_thickness);

        /* draw text and text background */
        cv::Rect background(text_orig.x, text_orig.y - text_size.height, text_size.width, text_size.height + baseline);
        cv::rectangle(image, background, color, cv::FILLED);
        cv::putText(image, text, text_orig, font_face, font_scale, cv::Scalar(0, 0, 0), text_thickness, cv::LINE_AA);
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
