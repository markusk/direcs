#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>

#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>

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
