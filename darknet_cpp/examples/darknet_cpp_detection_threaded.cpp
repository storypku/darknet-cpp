/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Darknet C++ detection demo with detections
 *  running in seperate thread
 */

#include "darknet.hpp"

#include "opencv2/highgui/highgui.hpp"
#include <thread>
#include <string>
#include <chrono>

#define DETECTION_THRESHOLD         0.24
#define DETECTION_HIER_THRESHOLD    0.5
#define NMS_THRESHOLD               0.4

static Darknet::Detector g_detector;
static Darknet::Image g_dnimage;
static bool g_new_detections = false;

static bool detect_in_image(void)
{
    if (!g_detector.detect(g_dnimage)) {
        std::cerr << "Failed to run detector" << std::endl;
        return false;
    }

    g_new_detections = true;
    return true;
}

int main(int argc, char *argv[])
{
    cv::VideoCapture cap;
    cv::Mat cvimage, cvimage_prev;
    Darknet::Image dnimage;
    Darknet::ConvertCvBgr8 converter;
    std::thread detector_thread;
    std::vector<Darknet::Detection> detections;

    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input_data_file> <input_cfg_file> <input_weights_file> [<videofile>]" << std::endl;
        return -1;
    }

    std::string input_data_file(argv[1]);
    std::string input_cfg_file(argv[2]);
    std::string input_weights_file(argv[3]);

    if (argc == 5) {
        std::string videofile(argv[4]);
        if (!cap.open(videofile)) {
            std::cerr << "Could not open video file" << std::endl;
            return -1;
        }
    } else if (!cap.open(0)) {
        std::cerr << "Could not open video input stream" << std::endl;
        return -1;
    }

    int image_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int image_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    if (!g_detector.setup(input_data_file,
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

    converter.setup(image_width, image_height, g_detector.get_width(), g_detector.get_height());
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

        if (detector_thread.joinable()) {
            detector_thread.join();
        }

        g_dnimage = dnimage;

        if (g_new_detections) {
            g_new_detections = false;

            // get detections
            g_detector.get_detections(detections);

            // start new detection thread
            detector_thread = std::thread(detect_in_image);

            // draw bounding boxes
            Darknet::image_overlay(detections, cvimage_prev);

            auto now = std::chrono::system_clock::now();
            std::chrono::duration<double> period = (now - prevTime);
            prevTime = now;
            std::cout << "FPS: " << 1 / period.count() << std::endl;

            cv::imshow("Overlay", cvimage_prev);
            cv::waitKey(1);

        } else {
            detector_thread = std::thread(detect_in_image);
        }

        cvimage_prev = cvimage.clone();
    }

    return 0;
}
