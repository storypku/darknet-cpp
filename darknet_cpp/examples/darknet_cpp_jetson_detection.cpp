/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Jetson detection and restreaming demo
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

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define DETECTION_THRESHOLD         0.24
#define DETECTION_HIER_THRESHOLD    0.5
#define NMS_THRESHOLD               0.4
#define TARGET_FPS                  30

#define GST_CAPTURE_STRING          "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)I420, " \
                                    "framerate=(fraction)" STR(TARGET_FPS) "/1 ! nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"
#define GST_OUTPUT_STRING_START     "appsrc ! videoconvert ! video/x-raw, format=(string)BGRx ! nvvidconv ! video/x-raw(memory:NVMM), format=(string)I420 ! " \
                                    "omxh264enc ! video/x-h264, stream-format=byte-stream ! rtph264pay ! udpsink host="
#define GST_OUTPUT_STRING_END       " sync=false async=false "

static Darknet::Detector g_detector;
static Darknet::Image g_dnimage_detection;
static bool g_detector_busy;

static bool detect_in_image(void)
{
    if (!g_detector.detect(g_dnimage_detection, DETECTION_THRESHOLD, DETECTION_HIER_THRESHOLD)) {
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
    std::cout << " Labels: ";

    for (auto detection : detections) {
        std::cout << detection.label << ", " << detection.probability << "; ";
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
    std::vector<Darknet::Detection> latest_filtered_detections;
    std::thread detector_thread;
    std::vector<std::string> detection_filter( {"person"} );

    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <input_data_file> <input_cfg_file> <input_weights_file> <ip_addr_destination>" << std::endl;
        return -1;
    }

    std::string input_data_file(argv[1]);
    std::string input_cfg_file(argv[2]);
    std::string input_weights_file(argv[3]);
    std::string ip_addr_dest(argv[4]);

    if (!cap.open(GST_CAPTURE_STRING)) {
        std::cerr << "Could not open video input stream" << std::endl;
        return -1;
    }

    if (!cap.read(cvimage)) {
        std::cerr << "Failed to capture initial camera image" << std::endl;
        return -1;
    }

    if (!writer.open(GST_OUTPUT_STRING_START + ip_addr_dest + GST_OUTPUT_STRING_END, 0, TARGET_FPS, cvimage.size())) {
        std::cerr << "Could not open video output stream" << std::endl;
        return -1;
    }

    if (!g_detector.setup(input_data_file, input_cfg_file, input_weights_file, NMS_THRESHOLD)) {
        std::cerr << "Setup failed" << std::endl;
        return -1;
    }

    cv::Size detector_input_size(g_detector.get_width(), g_detector.get_height());

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

            // filter detections
            Darknet::filter_detections(latest_detections, latest_filtered_detections, detection_filter);
        }

        // overlay detections
        Darknet::image_overlay(latest_filtered_detections, cvimage);
        print_stats(latest_filtered_detections);

        // restream
        writer.write(cvimage);

    }

    return 0;
}
