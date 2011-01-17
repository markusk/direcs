#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.hpp>

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

	IplImage*    g_image;
	IplImage*    g_gray;
	int        g_thresh;
	CvMemStorage*  g_storage;
};

#endif // GLWIDGET_H
