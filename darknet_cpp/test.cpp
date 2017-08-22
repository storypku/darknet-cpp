/*
 *  Author: EAVISE
 *  Description: Demo for cpp API
 */

#include "darknet.hpp"

#include "opencv2/core/core.hpp"
#include <opencv2/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <chrono>
#include <boost/filesystem.hpp>

using namespace cv;

#define MAX_OBJECTS_PER_FRAME   (100)
#define TARGET_SHOW_FPS         (10)
#define GST_CAPTURE_STRING      "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)I420, " \
                                "framerate=(fraction)30/1 ! nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"

int main(int argc, char *argv[])
{
    bool ret = false;
    cv::Mat cvimage, cvimage_detection;
    cv::VideoCapture cap;
    Darknet::Image image_detection;
    Darknet::Detector detector;
    std::vector<Darknet::Detection> detections;

    if (argc < 3) {
        EPRINTF("Usage: %s <input_data_file> <input_cfg_file> <input_weights_file>\n", argv[0]);
        return -1;
    }

    std::string input_data_file(argv[1]);
    std::string input_cfg_file(argv[2]);
    std::string input_weights_file(argv[3]);

    if (!detector.setup(input_data_file, input_cfg_file, input_weights_file, 0.4, 2)) {
        EPRINTF("Setup failed\n");
        return -1;
    }

    if (!cap.open(GST_CAPTURE_STRING)) {
        EPRINTF("Could not open input stream\n");
        return -1;
    }

    auto prevTime = std::chrono::system_clock::now();
    cv::Size detector_input_size(detector.get_width(), detector.get_height());

    //TODO: limit to only the person class
    //TODO: run detection in seperate thread from frame acquisition
    //TODO: add restreaming at 30fps
    while(1) {

        if (!cap.read(cvimage)) {
            EPRINTF("Video capture read failed/EoF\n");
            return -1;
        }

        // resize to match detector input dimensions
        cv::resize(cvimage, cvimage_detection, detector_input_size);

        // convert opencv image to darknet image
        image_detection.set(cvimage_detection);

        if (!detector.detect(image_detection, 0.24, 0.5)) {
            EPRINTF("Failed to run detector\n");
            return -1;
        }

        detector.get_detections(detections);
        Darknet::image_overlay(detections, cvimage);

        auto now = std::chrono::system_clock::now();
        std::chrono::duration<double> period = (now - prevTime);
        prevTime = now;
        printf("==> FPS: %f\n", 1 / period.count());

        resize(cvimage, cvimage, cv::Size(320, 180));
        imshow("Overlay", cvimage);
        waitKey(1);
    }

    return 0;
}
