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
#include <thread>
#include <mutex>
#include <boost/filesystem.hpp>

using namespace cv;

#define GST_CAPTURE_STRING      "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)I420, " \
                                "framerate=(fraction)30/1 ! nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"
#define GST_OUTPUT_STRING       "appsrc ! videoconvert ! video/x-raw, format=(string)BGRx ! nvvidconv ! video/x-raw(memory:NVMM), format=(string)I420 ! " \
                                "omxh264enc ! video/x-h264, stream-format=byte-stream ! rtph264pay ! udpsink host=10.66.24.128 port=8554 sync=false async=false "

static Darknet::Detector g_detector;
static Darknet::Image g_dnimage_detection;
static bool g_detector_busy;

static bool detect_in_image(void)
{
    if (!g_detector.detect(g_dnimage_detection, 0.24, 0.5)) {
        std::cerr << "Failed to run detector" << std::endl;
        return false;
    }

    g_detector_busy = false;
    return true;
}

static void print_stats(std::vector<Darknet::Detection> detections)
{
    static auto prevTime = std::chrono::system_clock::now();

    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> period = (now - prevTime);
    prevTime = now;
    std::cout << "FPS: " << 1 / period.count();

    std::cout << "Labels: ";

    for (auto detection : detections) {
        std::cout << detection.label << ", ";
    }

    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    cv::VideoCapture cap;
    cv::VideoWriter writer;
    cv::Mat cvimage, cvimage_detection;
    Darknet::Image dnimage_detection;
    std::vector<Darknet::Detection> latest_detections;
    std::thread detector_thread;

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_data_file> <input_cfg_file> <input_weights_file>" << std::endl;
        return -1;
    }

    std::string input_data_file(argv[1]);
    std::string input_cfg_file(argv[2]);
    std::string input_weights_file(argv[3]);

    if (!cap.open(GST_CAPTURE_STRING)) {
        std::cerr << "Could not open video input stream" << std::endl;
        return -1;
    }

    if (!cap.read(cvimage)) {
        std::cerr << "Failed to capture initial camera image" << std::endl;
        return -1;
    }

    //TODO put '30' and other constants in defines
    if (!writer.open(GST_OUTPUT_STRING, 0, 30, cvimage.size())) {
        std::cerr << "Could not open video output stream" << std::endl;
        return -1;
    }

    if (!g_detector.setup(input_data_file, input_cfg_file, input_weights_file, 0.4, 2)) {
        std::cerr << "Setup failed" << std::endl;
        return -1;
    }

    cv::Size detector_input_size(g_detector.get_width(), g_detector.get_height());

    //TODO: limit to only the person class
    while(1) {

        if (!cap.read(cvimage)) {
            std::cerr << "Video capture read failed/EoF" << std::endl;
            return false;
        }

        // resize to match detector input dimensions
        cv::resize(cvimage, cvimage_detection, detector_input_size);
        // convert opencv image to darknet image
        dnimage_detection.set(cvimage_detection);

        // if detector thread is finished, start it again
        if (!g_detector_busy) {
            if (detector_thread.joinable())
                detector_thread.join();

            g_detector.get_detections(latest_detections);
            g_dnimage_detection = dnimage_detection;
            g_detector_busy = true;
            detector_thread = std::thread(detect_in_image);
        }

        // overlay detections
        Darknet::image_overlay(latest_detections, cvimage);
        print_stats(latest_detections);

        // restream
        writer.write(cvimage);

    }

    return 0;
}
