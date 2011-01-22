#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// using namespace cv;

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


protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);


private:
	QImage qframe;
};

#endif // GLWIDGET_H
