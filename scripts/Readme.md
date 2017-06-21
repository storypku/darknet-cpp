

The following scripts for creating training annotations are available:

* First, we start by creating the annotations. This tutorial is written to work with the annotationtool that is available in https://gitlab.com/EAVISE/KIFARE. 
* This annotationtool creates an XML file for each image that was annotated. In our next steps, we convert these XML files to the correct training format for both the EAVISE ACF framework as well as for the YOLO framework
  1. Visualize the annotations using the `ShowAnnotations` script, this scripts will graphically display the annotations and it will convert the annotation XML files into a single `annotations.txt` file.
  1. The Matlab script `Annotations2Dollar.m` file, creates training files in the `Dollar` format. This type of annotation can for example be used for training an ACF model with the EAVISE ACF framework.
  1. The Matlab script `Dollar2Yolo.m` file on his turn, transforms the Dollar annotations into an annotation format that is interpretable by the YOLO framework for training.

!!It is important to mention that the above mentioned scripts are the results of some quick and dirty programming. For example, the Annotations2Dollar Matlab file will crash if more than three annotattions are present in one file. Thus, before using them, please have a look at the code and modify it to meet your needs.!!