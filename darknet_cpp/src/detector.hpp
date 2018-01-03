/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Detector API
 */

#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <string>
#include <memory>
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
     *  data_cfg_file:      data configuration file
     *  net_cfg_file:       network configuration file that describes the network architecture
     *  weight_cfg_file:    weights file that contains the trained network weights
     *  nms:                non maxima suppression threshold (number between 0 and 1)
     *  thresh:             detection threshold. Detection probabilities lower than this threshold
     *                      will not be considered detections (number between 0 and 1)
     *  hier_thres:         Hierarchical threshold ??? (number between 0 and 1)
     *  output_width:       Detection coordinates will be scaled according to this width. See the
     *                      'get_detections' method for more info
     *  output_height:      Detection coordinates will be scaled according to this height. See the
     *                      'get_detections' method for more info
     *  NOTE: if output_width and output_height are 0, they will be set internally to the network input
     *  width and height respectively
     *
     *  returns true on success
     */
    bool setup(std::string data_cfg_file,
                std::string net_cfg_file,
                std::string weight_cfg_file,
                float nms,
                float thresh,
                float hier_thresh,
                int output_width = 0,
                int output_height = 0);

    /*
     *  Set output size to scale detection coordinates. Usefull if output_size it not yet known when calling setup
     *  output_width:       Detection coordinates will be scaled according to this width. See the
     *                      'get_detections' method for more info
     *  output_height:      Detection coordinates will be scaled according to this height. See the
     *                      'get_detections' method for more info
     */
    void set_output_size(int output_width, int output_height);

    /*
     *  Run the network on a given input image
     *  image:  input image. The image dimensions (width, height and number of channels) must match the
     *          network input size, if not, the function will return false
     *  returns true on success
     */
    bool detect(const Image & image);

    /*
     *  Retrieve detections calulated with the detect method
     *  detections:     list of retrieved detections
     *  returns true on success
     *
     *  The detection values x, y, width, height have dimensions according to the given 'output_width'
     *  and 'output_height' (see setup or set_output_size methods).
     *      x:      0 = left edge of image, output_width = right edge of image
     *      y:      0 = top edge of image, output_height = bottom edge of image
     *      width:  0 = 0, output_width = width of image
     *      height: 0 = 0, output_height = height of image
     *
     *  Note that the values can go beyond the interval [0, 1] so clipping is needed
     *  for visualization
     */
    bool get_detections(std::vector<Detection>& detections);

    /*
     *  Get detector network input dimensions when setup has already constructed
     *  the network
     */
    int get_width();
    int get_height();
    int get_channels();

private:

    /* Pimpl idiom: hide original implementation from this api */
    class   impl;
    std::unique_ptr<impl> pimpl;
};

} /* namespace Darknet */

#endif /* DETECTOR_HPP */
