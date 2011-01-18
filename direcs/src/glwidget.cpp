#include "glwidget.h"


GLWidget::GLWidget(QWidget *_parent)   : QGLWidget(_parent)
{
//	setMinimumSize(640,480);
	// re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
	this->resize(_parent->size());

	// - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// contour finding stuff
//	g_image    = NULL;
	g_gray    = NULL;
	g_thresh  = 100;
	g_storage  = NULL;
	// - - - - - - - - - - - - - - - - - - - - - - - - - - -
}


void GLWidget::initializeGL()
{
	glClearColor(0.0f,0.0f,0.0f,1.0f);
}


void GLWidget::paintGL()
{
	glClear (GL_COLOR_BUFFER_BIT);
	glClearColor (0.0,0.0,0.0,1.0);

	if (!qframe.isNull())
	{
		qframe = qframe.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		// you can use glDrawPixels directly
		// glDrawPixels(qframe.width(),qframe.height(), GL_RGBA, GL_UNSIGNED_BYTE, qframe.bits());
		// or do 2D texture mapping
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0,qframe.width(),qframe.height(),0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe.width(), qframe.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe.bits() );
		glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex2f(0,qframe.height());
		glTexCoord2f(0,1); glVertex2f(0,0);
		glTexCoord2f(1,1); glVertex2f(qframe.width(),0);
		glTexCoord2f(1,0); glVertex2f(qframe.width(),qframe.height());
		glEnd();
		glDisable(GL_TEXTURE_2D);
		// .... end

		// some example of alpha blending
		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glColor4f(0.0f,1.0f,1.0f, 0.9f);
		glFlush();
	}
	// qDebug() << "Drawing...";
}


void GLWidget::resizeGL(int w, int h)
{
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	glMatrixMode (GL_MODELVIEW);
	// qDebug() << "Resizing...";
}


void GLWidget::sendImage(cv::Mat* img)
{
/*
	// - - - - - - - - - - - - - - - - - - - - - - - - - - -
	if ( g_storage==NULL )
	{
//		g_gray = cvCreateImage( cvSize(FREENECT_FRAME_W, FREENECT_FRAME_H), 8, 1 );
		g_gray = cvCreateImage( cvSize(640, 480), 8, 1 );
		g_storage = cvCreateMemStorage(0);
	} else
	{
		cvClearMemStorage( g_storage );
	}

	CvSeq* contours = 0;
	cvCvtColor( img, g_gray, CV_BGR2GRAY );
	cvThreshold( g_gray, g_gray, g_thresh, 255, CV_THRESH_BINARY );
	cvFindContours( g_gray, g_storage, &contours );
	cvZero( g_gray );

	if( contours )
	{
		cvDrawContours(g_gray, contours, cvScalarAll(255), cvScalarAll(255), 100);
	}

	//cvShowImage( "Contours", g_gray );
	// - - - - - - - - - - - - - - - - - - - - - - - - - - -

	qframe = QImage((const unsigned char*)(g_gray->imageData), g_gray->width, g_gray->height, g_gray->widthStep, QImage::Format_RGB888).rgbSwapped();
*/
	qframe = QImage((const unsigned char*)(img->data), img->cols, img->rows, img->step, QImage::Format_RGB888).rgbSwapped();
	qframe = QGLWidget::convertToGLFormat(qframe);
	this->updateGL();

}
