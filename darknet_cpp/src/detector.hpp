/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Detector API
 */

#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <string>
#include "darknet.h"
#include "image.hpp"
#include "detection.hpp"

namespace Darknet
{

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
                float nms);

    /*
     *  Cleanup
     *  NOTE: this is also called from the destructor
     */
    void release();

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
     *  Get detector network input dimensions when setup has already constructed
     *  the network
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
    int     m_threshold;
    std::vector<Detection> m_detections;

};

} /* namespace Darknet */

#endif /* DETECTOR_HPP */
