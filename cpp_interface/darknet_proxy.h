/*
 *  Author: EAVISE
 *  Description: C++ API for darknet framework.
 *  It currently only supports interence detection
 */
#ifndef DARKNET_PROXY_H
#define DARKNET_PROXY_H

#include <string>
#include <opencv2/opencv.hpp>

#include "darknet.h"

#ifdef _DEBUG
#define DPRINTF printf
#define EPRINTF printf
#else
#define DPRINTF
#define EPRINTF printf
#endif

namespace Darknet
{

struct Detection
{
    float x;            // box relative center x position
    float y;            // box relative center y position
    float width;        // box relative width
    float height;       // box relative height
    float probability;  // label probability
    int label_index;    // label index that matches the labels list (starts with 0)
    std::string label;  // label name
};

class Detector
{
public:
    Detector();
    ~Detector();

    /*
     *  Setup network for detection
     *  returns true on success
     */
    bool setup(std::string data_cfg_file,
                std::string net_cfg_file,
                std::string weight_cfg_file,
                float nms, int max_classes);

    /*
     *  Run the network on a given input image
     *  thres:          minimum probability score a detection needs to be considered a detection
     *  hier_thres:     ?
     *  objectCount:    ouput parameter -> number of detected objects
     *  returns true on success
     */
    bool detect(const cv::Mat & inputMat,
                float thresh,
                float hier_thresh,
                int & objectCount);

    /*
     *  Retrieve detections calulated with the detect method
     *  detections:     list of retrieved detections
     *  returns true on success
     */
    bool get_detections(std::vector<Detection>& detections);

    /*
     *  Draw box overlay of detections on an image
     *  NOTE: image size must match the given image to the detect method in order to get correct boxing
     */
    bool get_overlay(cv::Mat& image);

private:

    box     *m_boxes;
    char    **m_classNames;
    float   **m_probs;
    bool    m_bSetup;
    network m_net;
    layer   m_l;
    float   m_nms;
    int     m_maxClasses;
    int     m_threshold;
    std::vector<Detection> m_detections;

};

} /* namespace Darknet */

#endif /* DARKNET_PROXY_H */
