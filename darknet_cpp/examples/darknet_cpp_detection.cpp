/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Darknet C++ detection demo
 */

#include "darknet.hpp"

#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <chrono>

#define DETECTION_THRESHOLD         0.24
#define DETECTION_HIER_THRESHOLD    0.5
#define NMS_THRESHOLD               0.4

int main(int argc, char *argv[])
{
    cv::VideoCapture cap;
    cv::Mat cvimage;
    Darknet::Image dnimage;
    Darknet::ConvertCvBgr8 converter;
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

    int image_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int image_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    if (!detector.setup(input_data_file,
                        input_cfg_file,
                        input_weights_file,
                        NMS_THRESHOLD,
                        DETECTION_THRESHOLD,
                        DETECTION_HIER_THRESHOLD,
                        image_width,
                        image_height)) {
        std::cerr << "Setup failed" << std::endl;
        return -1;
    }

    converter.setup(image_width, image_height, detector.get_width(), detector.get_height());

    auto prevTime = std::chrono::system_clock::now();

    while(1) {

        if (!cap.read(cvimage)) {
            std::cerr << "Video capture read failed/EoF" << std::endl;
            return -1;
        }

        // convert and resize opencv image to darknet image
        if (!converter.convert(cvimage, dnimage)) {
            std::cerr << "Failed to convert opencv image to darknet image" << std::endl;
            return -1;
        }

        // run detector
        if (!detector.detect(dnimage)) {
            std::cerr << "Failed to run detector" << std::endl;
            return -1;
        }

        detector.get_detections(detections);

        // draw bounding boxes
        Darknet::image_overlay(detections, cvimage);

        auto now = std::chrono::system_clock::now();
        std::chrono::duration<double> period = (now - prevTime);
        prevTime = now;
        std::cout << "FPS: " << 1 / period.count() << std::endl;

        cv::imshow("Overlay", cvimage);
        cv::waitKey(1);

    }

    return 0;
}
