#include "OpenXml.h"

OpenXml::OpenXml(string xmlFile)
{

    // We'll parse the example.xml
    QFile* file = new QFile(QString::fromStdString(xmlFile));
    // If we can't open it, let's show an error message.
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        //cout << "Enable to open file." << endl;
        return;
    }
    xml = new QXmlStreamReader(file);    
}

void OpenXml::XmlToData(vector<AnnotationStruct> &Annotations) {

    AnnotationStruct AnnotationEntry;

    while( !xml->atEnd() && !xml->hasError()) {
        QXmlStreamReader::TokenType token = xml->readNext();
        // If token is just StartDocument, we'll go to next.
        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }
        if (token == QXmlStreamReader::StartElement) {
            if (xml->name() == "annotation") {
                xml->readNext();
                // start of XML was found
            }

            if (xml->name() == "path") {
                xml->readNext();
                string path = xml->text().toString().toStdString();
                AnnotationEntry.path = path;
                }

            if (xml->name() == "object")
            {
                annotationBB entry = this->readObject(AnnotationEntry);
                AnnotationEntry.boxes.push_back(entry);
            }
        }
    }
    // store only when "path" value is not empty!
    if(!AnnotationEntry.path.empty())
        Annotations.push_back(AnnotationEntry);
}

annotationBB OpenXml::readObject(AnnotationStruct &AnnotationEntry) {
    annotationBB BoundingBoxItem;
    int xmin = 0, ymin = 0, xmax = 0, ymax = 0;

    while (!(xml->tokenType() == QXmlStreamReader::EndElement && xml->name() == "object"))  {
        xml->readNext();
        if (xml->name() == "name")
        {
            xml->readNext();
            string name = xml->text().toString().toStdString();
            xml->readNext();
            BoundingBoxItem.name = name;
        }

        if(xml->name() == "bndbox")
        {
            while (!(xml->tokenType() == QXmlStreamReader::EndElement && xml->name() == "bndbox"))  {
                xml->readNext();
                if (xml->name() == "xmin")
                {
                    xml->readNext();
                    xmin = xml->text().toString().toInt();
                    xml->readNext();
                }
                if (xml->name() == "ymin")
                {
                    xml->readNext();
                    ymin = xml->text().toString().toInt();
                    xml->readNext();
                }
                if (xml->name() == "xmax")
                {
                    xml->readNext();
                    xmax = xml->text().toString().toInt();
                    xml->readNext();
                }
                if (xml->name() == "ymax")
                {
                    xml->readNext();
                    ymax = xml->text().toString().toInt();
                    xml->readNext();
                }
                BoundingBoxItem.BB.x = xmin;
                BoundingBoxItem.BB.y = ymin;
                BoundingBoxItem.BB.width = xmax-xmin;
                BoundingBoxItem.BB.height = ymax-ymin;
            }
        }
    }
    return BoundingBoxItem;
}
