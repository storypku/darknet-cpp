#ifndef FRAME_GRABBER_H
#define FRAME_GRABBER_H

#include "darknet.h"

#ifndef __cplusplus

struct FrameGrabber
{
    pthread_t thread;
    pthread_mutex_t mutex;
    int running;
    CvCapture *cap;
    IplImage *img;
    IplImage *img_copy;
};

/* assumes cap is already opened */
int frame_grabber_open(struct FrameGrabber *self, CvCapture *cap);

IplImage *frame_grabber_grab(struct FrameGrabber *self);

void frame_grabber_close(struct FrameGrabber *self);

#endif

#endif /* FRAME_GRABBER_H */
