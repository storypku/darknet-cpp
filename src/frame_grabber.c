#include "frame_grabber.h"
#include <unistd.h>

static void *frame_grabber_thread(void *v_self)
{
    struct FrameGrabber *self = v_self;

    while (self->running) {
        pthread_mutex_lock(&self->mutex);
        self->img = cvQueryFrame(self->cap);
        pthread_mutex_unlock(&self->mutex);
        usleep(10000);
    }

    return NULL;
}

int frame_grabber_open(struct FrameGrabber *self, CvCapture *cap)
{
    IplImage *image;
    self->img = NULL;
    self->cap = cap;
    pthread_mutex_unlock(&self->mutex);

    /* check if cap is opened */

    self->running = 1;
    if (pthread_create(&self->thread, NULL, frame_grabber_thread, self)) {
        fprintf(stderr, "Frame grabber thread creation failed\n");
        return -1;
    }

    /* wait until thread is running by grabbing frames */
    while (1) {
        image = frame_grabber_grab(self);
        if (image)
            break;
        usleep(1000);
    }
    cvReleaseImage(&image);

    return 0;
}

IplImage *frame_grabber_grab(struct FrameGrabber *self)
{
    IplImage *image;

    if (!self->running || !self->img)
        return NULL;

    pthread_mutex_lock(&self->mutex);
    /* deepcopy image */
    image = cvCloneImage(self->img);
    pthread_mutex_unlock(&self->mutex);
    return image;
}

void frame_grabber_close(struct FrameGrabber *self)
{
    self->running = 0;
    pthread_join(self->thread, NULL);
    cvReleaseImage(&self->img);
}
