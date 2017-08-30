/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Detector API
 */

#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <string>
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

    /* Pimpl idiom: hide original implementation from this api */
    class   impl;
    std::unique_ptr<impl> pimpl;
};

} /* namespace Darknet */

#endif /* DETECTOR_HPP */
