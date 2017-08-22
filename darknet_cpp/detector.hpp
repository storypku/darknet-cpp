/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Detector API
 */

#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <string>
#include <opencv2/opencv.hpp>

#include "darknet.h"
#include "image.hpp"

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
     *  image:          image dimensions must match the network input size
     *  thres:          minimum probability score a detection needs to be considered a detection
     *  hier_thres:     ?
     *  objectCount:    ouput parameter -> number of detected objects
     *  returns true on success
     */
    bool detect(const Image & image,
                float thresh,
                float hier_thresh);

    /*
     *  Retrieve detections calulated with the detect method
     *  detections:     list of retrieved detections
     *  returns true on success
     */
    bool get_detections(std::vector<Detection>& detections);

    /*
     *  Detector network input dimensions
     */
    int get_width();
    int get_height();

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

#endif /* DETECTOR_HPP */
