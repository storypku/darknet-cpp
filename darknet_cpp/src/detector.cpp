/*
 *  Author: EAVISE
 *  Description: Detector API implementation
 */

#include "detector.hpp"
#include "logging.hpp"
#include "darknet.h"                /* original darknet !!! */
#include <boost/filesystem.hpp>

using namespace Darknet;

/*
 *  Implementation class, see header for method descriptions
 */

extern "C" {
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>
}

static bool filesystem_exist(const std::string& path) {
    struct stat buf = { 0 };
    return stat(path.c_str(), &buf) == 0;
}

class Detector::impl
{
public:
    impl();
    ~impl();
    bool setup(std::string data_cfg_file,
                std::string net_cfg_file,
                std::string weight_cfg_file,
                float nms,
                float thresh,
                float hier_thresh,
                int output_width,
                int output_height);
    void set_output_size(int output_width, int output_height);
    void release();
    bool detect(const Image & image);
    bool get_detections(std::vector<Detection>& detections);
    int get_width();
    int get_height();
    int get_channels();

private:
    std::vector<std::string> m_class_names;
    bool    m_bSetup;
    network *m_net;
    detection *m_dets;
    int     m_nboxes;
    int     m_classes;
    float   m_nms;
    float   m_threshold;
    float   m_hier_threshold;
    int     m_output_width;
    int     m_output_height;
};

/*
 *  Implementations
 */

Detector::impl::impl() :
        m_class_names(),
        m_bSetup(false),
        m_net(nullptr),
        m_dets(nullptr),
        m_nboxes(0),
        m_classes(0),
        m_nms(0),
        m_threshold(0),
        m_hier_threshold(0),
        m_output_width(0),
        m_output_height(0) {}

Detector::impl::~impl()
{
    release();
}

void Detector::impl::release()
{
    // TODO - Massive cleanup here

    m_bSetup = false;

}

bool Detector::impl::setup(std::string label_names_file,
                std::string net_cfg_file,
                std::string weight_cfg_file,
                float nms,
                float thresh,
                float hier_thresh,
                int output_width,
                int output_height)
{
    m_nms = nms;
    m_threshold = thresh;
    m_hier_threshold = hier_thresh;
    m_output_width = output_width;
    m_output_height = output_height;

    if (!filesystem_exist(label_names_file)) {
        DPRINTF("Label names file %s not found\n", label_names_file.c_str());
        return false;
    }

    if (!filesystem_exist(net_cfg_file)) {
    //if (!boost::filesystem::exists(net_cfg_file)) {
        EPRINTF("Network cfg file %s not found\n", net_cfg_file.c_str());
        return false;
    }

    if (!filesystem_exist(weight_cfg_file)) {
    // if (!boost::filesystem::exists(weight_cfg_file)) {

        EPRINTF("Weights file %s not found\n", weight_cfg_file.c_str());
        return false;
    }

    std::ifstream label_names_stream(label_names_file);
    std::string name;

    while (std::getline(label_names_stream, name))
        m_class_names.push_back(name);

    m_net = load_network(net_cfg_file.c_str(), weight_cfg_file.c_str(), 0);
    if (!m_net) {
        EPRINTF("Failed to load network %s, %s\n", net_cfg_file.c_str(), weight_cfg_file.c_str());
        return false;
    }

    DPRINTF("Setup: net->n = %d\n", m_net->n);
    set_batch_network(m_net, 1);

    layer l = m_net->layers[m_net->n-1];
    m_classes = l.classes;
    DPRINTF("Setup: layers = %d, %d, %d, classes = \n", l.w, l.h, l.n, m_classes);

    if (m_output_width == 0 && m_output_height == 0) {
        DPRINTF("No detections output widht/height provided, using network dimensions\n");
        m_output_width = m_net->w;
        m_output_height = m_net->h;
    }

    DPRINTF("Image expected w,h = [%d][%d]!\n", m_net->w, m_net->h);
    DPRINTF("Detection coordinates will be given within the following range w,h = [%d][%d]!\n", m_output_width, m_output_height);

    DPRINTF("Setup: Done\n");
    m_bSetup = true;
    return true;
}

void Detector::impl::set_output_size(int output_width, int output_height)
{
    m_output_width = output_width;
    m_output_height = output_height;
}

bool Detector::impl::detect(const Image & image)
{
    if (!m_bSetup) {
        EPRINTF("Not Setup!\n");
        return false;
    }

    if (!image.data) {
        EPRINTF("Image is empty\n");
        return false;
    }

    if (m_net->w != image.width || m_net->h != image.height || m_net->c != image.channels) {
        EPRINTF("Given image dimensions do not match the network size: "
                "image dimensions: w = %d, h = %d, c = %d, network dimensions: w = %d, h = %d, c = %d\n",
                image.width, image.height, image.channels, m_net->w, m_net->h, m_net->c);
        return false;
    }

    // Predict
    (void) network_predict(m_net, image.data);

    if (m_dets)
        free_detections(m_dets, m_nboxes);

    m_dets = get_network_boxes(m_net, m_output_width, m_output_height, m_threshold, m_hier_threshold, 0, 0, &m_nboxes);

    if (m_nms > 0)
        do_nms(m_dets, m_nboxes, m_classes, m_nms);

    return true;
}

bool Detector::impl::get_detections(std::vector<Detection>& detections)
{
    int i;

    if (!m_bSetup)
        return false;

    // Extract detections in correct format
    detections.clear();
    for (i = 0; i < m_nboxes; ++i) {
        float prob;
        size_t class_index = max_index(m_dets[i].prob, m_classes);

        if (class_index >= m_class_names.size()) {
            EPRINTF("Class index exceeds class names list, probably the model does not match the names list\n");
            return false;
        }

        prob = m_dets[i].prob[class_index];

        if (prob > m_threshold) {
            Detection detection;
            detection.x = m_dets[i].bbox.x;
            detection.y = m_dets[i].bbox.y;
            detection.width = m_dets[i].bbox.w;
            detection.height = m_dets[i].bbox.h;
            detection.probability = prob;
            detection.label_index = class_index;
            detection.label = m_class_names[class_index];
            detections.push_back(detection);
        }
    }

    return true;
}

int Detector::impl::get_width()
{
    if (!m_bSetup)
        return 0;

    return m_net->w;
}

int Detector::impl::get_height()
{
    if (!m_bSetup)
        return 0;

    return m_net->h;
}

int Detector::impl::get_channels()
{
    if (!m_bSetup)
        return 0;

    return m_net->c;
}

/*
 *  Wrappers
 */

Detector::Detector() :
    pimpl{ new Detector::impl() }
{
}

/* needed for use of unique_ptr and incomplete type definitions */
Detector::~Detector() = default;

bool Detector::setup(std::string data_cfg_file,
                std::string net_cfg_file,
                std::string weight_cfg_file,
                float nms,
                float thresh,
                float hier_thresh,
                int output_width,
                int output_height)
{
    return pimpl->setup(data_cfg_file, net_cfg_file, weight_cfg_file, nms,
                            thresh, hier_thresh, output_width, output_height);
}

void Detector::set_output_size(int output_width, int output_height)
{
    return pimpl->set_output_size(output_width, output_height);
}

bool Detector::detect(const Image & image)
{
    return pimpl->detect(image);
}

bool Detector::get_detections(std::vector<Detection>& detections)
{
    return pimpl->get_detections(detections);
}

int Detector::get_width()
{
    return pimpl->get_width();
}

int Detector::get_height()
{
    return pimpl->get_height();
}

int Detector::get_channels()
{
    return pimpl->get_channels();
}
