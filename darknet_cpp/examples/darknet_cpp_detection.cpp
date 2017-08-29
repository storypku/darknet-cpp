/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Darknet C++ detection demo
 */

#include "darknet.hpp"

#include "opencv2/highgui/highgui.hpp"
#include <string>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define DETECTION_THRESHOLD         0.24
#define DETECTION_HIER_THRESHOLD    0.5
#define NMS_THRESHOLD               0.4
#define TARGET_FPS                  30

int main(int argc, char *argv[])
{
    cv::VideoCapture cap;
    cv::Mat cvimage, cvimage_detection;
    Darknet::Image dnimage_detection;
    Darknet::Detector detector;
    std::vector<Darknet::Detection> detections;

    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input_data_file> <input_cfg_file> <input_weights_file>" << std::endl;
        return -1;
    }

    std::string input_data_file(argv[1]);
    std::string input_cfg_file(argv[2]);
    std::string input_weights_file(argv[3]);

    if (!cap.open(0)) {
        std::cerr << "Could not open video input stream" << std::endl;
        return -1;
    }

    if (!detector.setup(input_data_file, input_cfg_file, input_weights_file, NMS_THRESHOLD)) {
        std::cerr << "Setup failed" << std::endl;
        return -1;
    }

    cv::Size detector_input_size(detector.get_width(), detector.get_height());

    while(1) {

        if (!cap.read(cvimage)) {
            std::cerr << "Video capture read failed/EoF" << std::endl;
            return false;
        }

        // resize to match detector input dimensions
        cv::resize(cvimage, cvimage_detection, detector_input_size);

        // convert opencv image to darknet image
        dnimage_detection.set(cvimage_detection);

        // run detector
        if (!detector.detect(dnimage_detection, DETECTION_THRESHOLD, DETECTION_HIER_THRESHOLD)) {
            std::cerr << "Failed to run detector" << std::endl;
            return false;
        }

        detector.get_detections(detections);

        // draw bounding boxes
        Darknet::image_overlay(detections, cvimage);

        cv::imshow("Overlay", cvimage);
        cv::waitKey(1);

    }

    return 0;
}