import sys
import os
import glob
import xml.etree.ElementTree as ET
from shutil import copyfile

def convert(size, box):
    dw = 1./size[0]
    dh = 1./size[1]
    x = (box[0] + box[1])/2.0
    y = (box[2] + box[3])/2.0
    w = box[1] - box[0]
    h = box[3] - box[2]
    x = x*dw
    w = w*dw
    y = y*dh
    h = h*dh

    if x == 0.0:
        x = 0.00001

    if y == 0.0:
        y = 0.00001

    return (x,y,w,h)

def xml2txt(path_in, path_out):
    data = []

    out_file = open(path_out, 'w')
    tree = ET.parse(path_in)
    root = tree.getroot()

    size = root.find('size')
    w = int(size.find('width').text)
    h = int(size.find('height').text)

    for obj in root.iter('object'):
        difficult = obj.find('difficult').text
        cls_id = 0
        xmlbox = obj.find('bndbox')
        b = (float(xmlbox.find('xmin').text), float(xmlbox.find('xmax').text), float(xmlbox.find('ymin').text),
             float(xmlbox.find('ymax').text))
        bb = convert((w, h), b)



        print ("Writing: " + path_out )
        out_file.write(str(cls_id) + " " + " ".join([str(a) for a in bb]) + '\n')



def main(argv):

    if len(argv) != 5:
        print ("provide 4 arguments:\n\t[PATH_OF_IMG]\n\t[PATH_OF_VOC]\n\t[OUTPUT_PATH_OF_IMG]\n\t[OUTPUT_PATH_OF_YOLO]")
    else:
        path_img_read = argv[1]
        path_voc_read = argv[2]

        path_img_write = argv[3]
        path_voc_write = argv[4]

        file_list = names = [os.path.splitext(os.path.basename(x))[0] for x in glob.glob(path_voc_read + "*.xml") ]

        i = 0
        for file in file_list:
            # check if img and xml exist
            if os.path.isfile(path_img_read + file + ".jpg") and os.path.isfile(path_voc_read + file + ".xml"):

                xml2txt(path_voc_read + file + ".xml", path_voc_write + "IMG_" + str(i) + ".txt")
                copyfile(path_img_read + file + ".jpg", path_img_write + "IMG_" + str(i) + ".jpg")

                i+=1
            else:
                print ("Error processing [NO_SYNC_IMG_VOC]: " + path_voc_read + file + ".xml")

main(sys.argv)


