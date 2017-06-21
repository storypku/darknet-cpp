#include <iostream>
#include "opencv2/opencv.hpp"
#include "OpenXml.h"
#include <boost/bind.hpp>

using namespace std;
using namespace cv;

//#define Write
#define Show

struct annotationIDStruct
{
    string id;
    int number;
    vector<AnnotationStruct> annotations;
};

Rect Resize(Rect bb, float scaleFactor)
{
   int oriWidth = bb.width;
   int oriHeight = bb.height;

   bb.width = bb.width*scaleFactor;
   bb.height = bb.height*scaleFactor;
   if(scaleFactor>1)
   {
       // shift x,y point towards upper left corner
       bb.x = bb.x - (bb.width-oriWidth)/2;
       bb.y = bb.y - (bb.height-oriHeight)/2;
   }
   else
   {
       bb.x = bb.x + (oriWidth-bb.width)/2;
       bb.y = bb.y + (oriHeight-bb.height)/2;
       // shift x,y point towards bottom right corner
   }
   return bb;
}

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        cout << "usage: ./ShowAnnotations <path to the directory the contains 'XML_files.txt'>" << endl;
    }



    string path = argv[1];
    string temp = path;
    string pathXmlList = temp.append("XML_files.txt");

    ifstream fileList;
    fileList.open(pathXmlList);

    if(!fileList.is_open())
    {
        cerr << "unable to open XML file list:" << pathXmlList << endl;
        return -1;
    }

    string line;
    vector<string> XMLfiles;
    if (fileList.is_open())
    {
        while ( getline (fileList,line) )
        {
          string temp = path;
          XMLfiles.push_back(temp.append(line));
        }
        fileList.close();
    }

    // parse the XML files
    vector<AnnotationStruct> annotations;
    for(int i=0;i<XMLfiles.size();i++)
    {
        OpenXml xml(XMLfiles[i]);
        xml.XmlToData(annotations);
    }

    // define ID vector
    vector<annotationIDStruct> annotations_ID;

    // show the annotations:
    cout << "there are " << annotations.size() << " annotation files " << endl;
    for(int i=0;i<annotations.size();i++)
    {
        string temp = annotations[i].path;
        Mat image = imread(temp.c_str());

        if(image.empty())
        {
            cerr << "unable to open image on path = " << temp.c_str() << endl;
            return -1;
        }

        for(int j=0;j<annotations[i].boxes.size();j++)
        {            
            rectangle(image,annotations[i].boxes[j].BB,Scalar(255,0,0),5);
            cout << "\t name = " << annotations[i].boxes[j].name << endl;
            Point P1;
            P1.x = annotations[i].boxes[j].BB.x;
            P1.y = annotations[i].boxes[j].BB.y;
            putText( image, annotations[i].boxes[j].name, P1, 2,20,Scalar(255,255,0),5);

            if(annotations_ID.size()==0)
            {
                // if vector is empty, add first item
                annotationIDStruct entry;
                entry.id = annotations[i].boxes[j].name;
                entry.number = 1;
                entry.annotations.push_back(annotations[i]);
                annotations_ID.push_back(entry);
            }
            else
            {
                // check if this id is already stored in the vector
                std::vector< annotationIDStruct >::iterator it = std::find_if ( annotations_ID.begin (), annotations_ID.end (),
                boost::bind ( &annotationIDStruct::id, _1 ) == annotations[i].boxes[j].name );
                if(it != annotations_ID.end() )
                {
                    cout << "ID " << annotations[i].boxes[j].name << " was found in vecor " << endl;
                    (*it).number++;
                    (*it).annotations.push_back(annotations[i]);
                }
                else
                {
                    annotationIDStruct entry;
                    entry.id = annotations[i].boxes[j].name;
                    entry.number = 1;
                    entry.annotations.push_back(annotations[i]);
                    annotations_ID.push_back(entry);
                }
            }
        }
#ifdef Show
        namedWindow("image",CV_WINDOW_NORMAL);
        imshow("image", image);
        waitKey();
#endif

#ifdef Write	
		char Path[200];
		resize(image, image, Size(), 0.5, 0.5);
        sprintf(Path,"%s%05d%s", "Detections/",i,".jpg");
        imwrite(Path,image);
#endif
    }

    // loop over all the annotations and write them into a single file that was called "annotations.txt"
    ofstream outputfile;
    int counter = 0;

    outputfile.open("annotations.txt");

    for(int i=0;i<annotations_ID.size();i++)
    {
        cout << "ID = " << annotations_ID[i].id;
        char makeDir[200];
        sprintf(makeDir,"%s%s","mkdir ",annotations_ID[i].id.c_str());
        system(makeDir);

        cout << "\t number = " << annotations_ID[i].number << endl;
        counter = counter + annotations_ID[i].number;
        for(int j=0;j<annotations_ID[i].annotations.size();j++)
        {
            outputfile << annotations_ID[i].annotations[j].path << " " << annotations_ID[i].annotations[j].boxes.size();
            for(int k=0;k<annotations_ID[i].annotations[j].boxes.size();k++)
            {
                outputfile << " " << annotations_ID[i].annotations[j].boxes[k].BB.x << " " << annotations_ID[i].annotations[j].boxes[k].BB.y;
                outputfile << " " << annotations_ID[i].annotations[j].boxes[k].BB.width << " " <<annotations_ID[i].annotations[j].boxes[k].BB.height << " ";
            }
            outputfile << endl;
        }
    }
}
