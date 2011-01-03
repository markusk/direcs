/* 
 * File:   QKinectCaptureThread.cpp
 * Author: matt
 * 
 * Created on 23 November 2010, 19:50
 */

#include <qt4/QtCore/qmutex.h>

#include "QKinectCaptureThread.h"

QKinectCaptureThread::QKinectCaptureThread(freenect_context *passed_f_context, freenect_device *passed_f_device) {
    rgb_frontbuffer = new freenect_pixel[FREENECT_RGB_SIZE];
    depth_frontbuffer = new freenect_depth[FREENECT_DEPTH_SIZE];
    /*set the backbuffers to point at something incase data is requested before
     * the backbuffers have been filled*/
    rgb_backbuffer = rgb_frontbuffer;
    depth_backbuffer = depth_frontbuffer;
    f_device = passed_f_device;
    f_context = passed_f_context;

    /*pass a pointer to this object as the user data in the freenect_device
     * struct so calls can be made to this instance*/
    freenect_set_user(f_device, this); //however thought this method up was one sexy programmer

    freenect_set_tilt_degs(f_device, 0);
    freenect_set_led(f_device, LED_RED);
    freenect_set_depth_callback(f_device, QKinectCaptureThread::depthCallback);
    freenect_set_rgb_callback(f_device, QKinectCaptureThread::RGBCallback);
    freenect_set_rgb_format(f_device, FREENECT_FORMAT_RGB);
    freenect_set_depth_format(f_device, FREENECT_FORMAT_11_BIT);

    freenect_start_depth(f_device);
    freenect_start_rgb(f_device);
}

QKinectCaptureThread::~QKinectCaptureThread() {
}

void QKinectCaptureThread::run() {
    death_lock.lock();
    die = false;
    freenect_start_depth(f_device);
    freenect_start_rgb(f_device);
    while (!die) {
        death_lock.unlock();
        //Do whatever needs to be done here
        bool set_die = false;
        if (freenect_process_events(f_context) < 0)set_die = true;
        
        //Finish doin what needs to be done
        death_lock.lock();
        if (set_die == true)die = set_die;
    }
    death_lock.unlock(); //Maybe not be needed, but better safe than sorry
    freenect_stop_depth(f_device);
    freenect_stop_rgb(f_device);
    freenect_close_device(f_device);
    return;
}

void QKinectCaptureThread::stopThread() {
    death_lock.lock();
    die = true;
    qDebug("Killing thread");
    death_lock.unlock();
}

void QKinectCaptureThread::depthCallback(freenect_device *dev, freenect_depth *v_depth, uint32_t timestamp) {
    //Static callback trampoline for interfacing with c code
    ((QKinectCaptureThread*) freenect_get_user(dev))->newDepth(v_depth, timestamp);
}

void QKinectCaptureThread::RGBCallback(freenect_device *dev, freenect_pixel *rgb, uint32_t timestamp) {
    //Static callback trampoline for interfacing with c code
    ((QKinectCaptureThread*) freenect_get_user(dev))->newRGB(rgb, timestamp);
}

void QKinectCaptureThread::newDepth(freenect_depth *v_depth, uint32_t timestamp) {
    frame_lock.lock();
    depth_backbuffer = v_depth;
    frame_lock.unlock();
}

void QKinectCaptureThread::newRGB(freenect_pixel *rgb, uint32_t timestamp) {
    frame_lock.lock();
    rgb_backbuffer = rgb;
    frame_lock.unlock();
}

freenect_pixel* QKinectCaptureThread::getRawRGB() {
    frame_lock.lock();
    //rgb_backbuffer = rgb_frontbuffer;
    memcpy(rgb_frontbuffer, rgb_backbuffer, FREENECT_RGB_SIZE);
    frame_lock.unlock();
    return rgb_frontbuffer;
}

freenect_depth* QKinectCaptureThread::getRawDepth() {
    frame_lock.lock();
    memcpy(depth_frontbuffer, depth_backbuffer, FREENECT_DEPTH_SIZE);
    frame_lock.unlock();
    return depth_frontbuffer;
}