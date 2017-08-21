/*************************************************************************
 * arapaho                                                               *
 *                                                                       *
 * C++ API for Yolo v2                                                   *
 *                                                                       *
 * This test wrapper reads an image or video file and displays           *
 * detected regions in it.                                               *
 *                                                                       *
 * https://github.com/prabindh/darknet                                   *
 *                                                                       *
 * Forked from, https://github.com/pjreddie/darknet                      *
 *                                                                       *
 * Refer below file for build instructions                               *
 *                                                                       *
 * arapaho_readme.txt                                                    *
 *                                                                       *
 *************************************************************************/

#include "darknet_proxy.h"
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
    cv::Mat image;
    cv::VideoCapture cap;
    Darknet::Detector detector;

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

    while(1) {

        if (!cap.read(image)) {
            EPRINTF("Video capture read failed/EoF\n");
            return -1;
        }

        DPRINTF("Image data = %p, w = %d, h = %d\n", image.data, image.cols, image.rows);

        auto now = std::chrono::system_clock::now();
        std::chrono::duration<double> period = (now - prevTime);
        prevTime = now;

        int numObjects = 0;

        detector.detect(image, 0.24, 0.5, numObjects);
        printf("==> #OBJS: %d FPS: %f\n", numObjects, 1 / period.count());
        detector.get_overlay(image);

        resize(image, image, cv::Size(320, 180));
        imshow("Overlay", image);
        waitKey(1);
    }

    return 0;
}
