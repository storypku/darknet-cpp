/*
 *  Author: EAVISE
 *  Description: C++ API for darknet framework.
 *  It currently only supports inference detection
 */

#include "detector.hpp"
#include "logging.hpp"
#include <boost/filesystem.hpp>

using namespace Darknet;

Detector::Detector() :
    m_boxes(0),
    m_probs(0),
    m_classNames(0),
    m_l({}),
    m_net({}),
    m_nms(0),
    m_maxClasses(0),
    m_threshold(0),
    m_bSetup(false)
{
}

Detector::~Detector()
{
    // TODO - Massive cleanup here

    if(m_boxes)
        free(m_boxes);
    if(m_probs)
        free_ptrs((void **)m_probs, m_l.w*m_l.h*m_l.n);
    if(m_classNames)
    {
        //todo
    }
}

bool Detector::setup(std::string data_cfg_file,
                std::string net_cfg_file,
                std::string weight_cfg_file,
                float nms, int max_classes)
{
    char nameField[] = "names";
    char defaultName[] = "data/names.list";

    if (!boost::filesystem::exists(data_cfg_file)) {
        DPRINTF("Data config file %s not found\n", data_cfg_file.c_str());
        return false;
    }

    if (!boost::filesystem::exists(net_cfg_file)) {
        EPRINTF("Network cfg file %s not found\n", net_cfg_file.c_str());
        return false;
    }

    if (!boost::filesystem::exists(weight_cfg_file)) {
        EPRINTF("Weights file %s not found\n", weight_cfg_file.c_str());
        return false;
    }

    list *options = read_data_cfg(data_cfg_file.c_str());

    char *nameListFile = option_find_str(options, nameField, defaultName);
    if (!nameListFile) {
        DPRINTF("No valid nameList file specified in options file [%s]!\n", data_cfg_file.c_str());
        return false;
    }

    m_classNames = get_labels(nameListFile);
    if(!m_classNames)
    {
        DPRINTF("No valid class names specified in nameList file [%s]!\n", nameListFile);
        return false;
    }

    int j;
    bool ret = false;

    m_bSetup = false;
    m_nms = nms;
    m_maxClasses = max_classes;

    m_net = parse_network_cfg(net_cfg_file.c_str());
    DPRINTF("Setup: net.n = %d\n", m_net.n);
    DPRINTF("net.layers[0].batch = %d\n", m_net.layers[0].batch);

    load_weights(&m_net, weight_cfg_file.c_str());

    set_batch_network(&m_net, 1);
    m_l = m_net.layers[m_net.n-1];
    DPRINTF("Setup: layers = %d, %d, %d\n", m_l.w, m_l.h, m_l.n);

    // Class limiter
    if(m_l.classes > m_maxClasses) {
        EPRINTF("Warning: Read classes from cfg (%d) > maxClasses (%d)\n", m_l.classes, m_maxClasses);
        //return false;
    }

    DPRINTF("Image expected w,h = [%d][%d]!\n", m_net.w, m_net.h);

    m_boxes = (box *)calloc(m_l.w * m_l.h * m_l.n, sizeof(box));
    m_probs = (float **)calloc(m_l.w * m_l.h * m_l.n, sizeof(float *));

    if(!m_boxes || !m_probs) {
        EPRINTF("Error allocating boxes/probs, %p/%p !\n", m_boxes, m_probs);
        goto clean_exit;
    }

    for(j = 0; j < m_l.w*m_l.h*m_l.n; ++j) {
        m_probs[j] = (float*)calloc(m_l.classes + 1, sizeof(float));
        if(!m_probs[j]) {
            EPRINTF("Error allocating probs[%d]!\n", j);
            goto clean_exit;
        }
    }

    ret = true;
    m_bSetup = ret;
    DPRINTF("Setup: Done\n");
    return ret;

clean_exit:
    if(m_boxes)
        free(m_boxes);

    if(m_probs)
        free_ptrs((void **)m_probs, m_l.w*m_l.h*m_l.n);

    m_boxes = NULL;
    m_probs = NULL;

    return ret;
}

bool Detector::detect(
            const Image & image,
            float thresh,
            float hier_thresh)
{
    int i;

    m_threshold = thresh;

    if (!m_bSetup) {
        EPRINTF("Not Setup!\n");
        return false;
    }

    if (m_net.w != image.width || m_net.h != image.height || m_net.c != image.channels) {
        EPRINTF("Given image dimensions do not match the network size: "
                "image dimensions: w = %d, h = %d, c = %d, network dimensions: w = %d, h = %d, c = %d\n",
                image.width, image.height, image.channels, m_net.w, m_net.h, m_net.c);
        return false;
    }

    // Predict
    (void) network_predict(m_net, image.data);
    get_region_boxes(m_l, m_net.w, m_net.h, m_net.w, m_net.h, thresh, m_probs, m_boxes, 0, 0, hier_thresh, 1);

    // Apply non maxima suppression
    DPRINTF("m_l.softmax_tree = %p, nms = %f\n", m_l.softmax_tree, m_nms);
    do_nms_sort(m_boxes, m_probs, m_l.w * m_l.h * m_l.n, m_l.classes, m_nms);

    // Extract detections in correct format
    m_detections.clear();
    for (i = 0; i < (m_l.w * m_l.h * m_l.n); ++i) {
        int classIndex = max_index(m_probs[i], m_l.classes);
        float prob = m_probs[i][classIndex];

        if (prob > m_threshold) {
            Detection detection;
            detection.x = m_boxes[i].x;
            detection.y = m_boxes[i].y;
            detection.width = m_boxes[i].w;
            detection.height = m_boxes[i].h;
            detection.probability = prob;
            detection.label_index = classIndex;
            detection.label = m_classNames[classIndex];
            m_detections.push_back(detection);
        }
    }

    return true;
}

bool Detector::get_detections(std::vector<Detection>& detections)
{
    if (!m_bSetup)
        return false;

    detections = m_detections;

    return true;
}

int Detector::get_width()
{
    if (!m_bSetup)
        return 0;

    return m_net.w;
}

int Detector::get_height()
{
    if (!m_bSetup)
        return 0;

    return m_net.h;
}
