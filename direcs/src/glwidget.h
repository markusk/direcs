#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "QKinect.h"


class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(QWidget *_parent);

	void sendImage(cv::Mat *img);

	/// Convert from Qt::QImage to cv::Mat
	cv::Mat qimage2mat(const QImage& qimage);

	/// Convert from cv::Mat to Qt::QImage
	QImage mat2qimage(const cv::Mat& mat);

	/// do OpenCV stuff
	void processOpenCV();


protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);


private:
	/// @brief called when the timer is triggered
	void timerEvent(QTimerEvent *_event);


	QImage qframe;

	cv::Mat mImage; // OpenCV test

	/// @brief the image data from the Kinect
	std::vector<uint8_t> m_rgb;
};

#endif // GLWIDGET_H
