// Be sure to link with -lfreenect_sync
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <libfreenect/libfreenect_sync.h>

freenect_context *f_ctx;
freenect_device *f_dev;

int main(int argc, char **argv)
{   
    IplImage *image = cvCreateImageHeader(cvSize(640,480), 8, 3);
    char *data;
    while (cvWaitKey(10) < 0) {
      unsigned int timestamp;
      freenect_sync_get_video((void**)&data, &timestamp, 0, FREENECT_VIDEO_RGB);
      cvSetData(image, data, 640*3);
      cvCvtColor(image, image, CV_RGB2BGR);
      cvShowImage("RGB", image);
    }
    free(data);
}
