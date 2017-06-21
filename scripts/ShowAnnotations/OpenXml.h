#ifndef OPENXML_H
#define OPENXML_H

#include <iostream>
#include <string>     // std::string, std::to_string
#include <stdio.h>

#include <QXmlStreamReader>
#include <QFile>

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

//----------------------------------------------------------------//
// Structures
//----------------------------------------------------------------//
struct annotationBB
{
    Rect BB;
    string name;
};

struct AnnotationStruct
{
    string path;
    int nrOfAnnotations;
    vector<annotationBB> boxes;
};

using namespace std;
using namespace cv;

class OpenXml
{
    public:

        OpenXml(string xmlFile);
        void XmlToData(vector<AnnotationStruct> &Annotations);

    private:
        QXmlStreamReader* xml;
        annotationBB readObject(AnnotationStruct &AnnotationEntry);
};

#endif // OPENXML_H
