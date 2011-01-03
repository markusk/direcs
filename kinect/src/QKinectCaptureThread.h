/* 
 * File:   QKinectCaptureThread.h
 * Author: matt
 *
 * Created on 23 November 2010, 19:50
 */

#ifndef _QKINECTCAPTURETHREAD_H
#define	_QKINECTCAPTURETHREAD_H
#include <QThread>
#include <QMutex>
#include <QImage>
#include <QRgb>
#include <QWaitCondition>
#include <libfreenect.h>
#include <stdint.h>
using namespace std;

class QKinectCaptureThread : public QThread {
    Q_OBJECT
public:
    QKinectCaptureThread(freenect_context *passed_f_context, freenect_device *passed_f_device);
    virtual ~QKinectCaptureThread();
    virtual void run();
    void stopThread();
    freenect_pixel* getRawRGB();
    freenect_depth* getRawDepth();
private:
    static void depthCallback(freenect_device *dev, freenect_depth *v_depth, uint32_t timestamp);
    static void RGBCallback(freenect_device *dev, freenect_pixel *rgb, uint32_t timestamp);
    void newDepth(freenect_depth *v_depth, uint32_t timestamp);
    void newRGB(freenect_pixel *rgb, uint32_t timestamp);
    volatile bool die; //flag used to tell the thread to harakiri
    QMutex death_lock; //Kinda like the title of a B-Movie version of a major film
    QMutex frame_lock;
    int frame_count;
    freenect_device *f_device;
    freenect_context *f_context;
    freenect_pixel *rgb_backbuffer;
    freenect_pixel *rgb_frontbuffer;
    freenect_depth *depth_backbuffer;
    freenect_depth *depth_frontbuffer;
    QWaitCondition images_ready;
};

#endif	/* _QKINECTCAPTURETHREAD_H */

