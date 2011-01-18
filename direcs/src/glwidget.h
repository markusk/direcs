#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>

#ifdef Q_OS_MAC // FIXME: Using OpenCV 2.1 under Mac OS X only.
	#include <opencv/cv.h>
	#include <opencv/highgui.h>
#include <opencv/cxcore.hpp>
#endif

#ifdef Q_OS_LINUX // FIXME: Using OpenCV 2.2 under Linux only.
	#include <opencv2/core/core_c.h>
	#include <opencv2/highgui/highgui_c.h>
#endif
//using namespace cv;

class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(QWidget *_parent);
	void sendImage(cv::Mat *img);


protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);


private:
	QImage qframe;
};

#endif // GLWIDGET_H
