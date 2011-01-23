#include "glwidget.h"


GLWidget::GLWidget(QWidget *_parent)   : QGLWidget(_parent)
{
	//	setMinimumSize(640,480);
	// re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
	this->resize(_parent->size());

	// start the timer to re-draw as quick as possible
	startTimer(500); // each 500 ms

	// create space for our image data from the Kinect
	m_rgb.resize(640*480*3);
}


void GLWidget::initializeGL()
{
	glClearColor(0.0f,0.0f,0.0f,1.0f);
}


void GLWidget::paintGL()
{
	//
	// get picture data from kinect class
	//
	QKinect *kinect=QKinect::instance();
/*
	if(m_mode ==0)
	{
*/
		kinect->getRGB(m_rgb);
/*
	}
	else if(m_mode == 1)
	{
		kinect->getDepth(m_rgb);
	}
*/

	cv::Mat depthMat(cv::Size(640,480), CV_16UC1);
	cv::Mat rgbMat(cv::Size(640,480), CV_8UC3, cv::Scalar(0));

	//copy to ocv_buf..
	memcpy(rgbMat.data, &m_rgb[0], 640*480*3);

	// convert to qimate
	qframe = mat2qimage(rgbMat);

	// qframe = QImage((const unsigned char*)(&m_rgb[0]), 640, 480, QImage::Format_RGB888);


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
	qframe = QImage((const unsigned char*)(img->data), img->cols, img->rows, img->step, QImage::Format_RGB888).rgbSwapped();
	qframe = QGLWidget::convertToGLFormat(qframe);
	this->updateGL();

}


cv::Mat GLWidget::qimage2mat(const QImage& qimage)
{
	cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.bits(), qimage.bytesPerLine());
	cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
	int from_to[] = { 0,0,  1,1,  2,2 };
	cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );

	return mat2;
}


QImage GLWidget::mat2qimage(const cv::Mat& mat)
{
	cv::Mat rgb;
	cv::cvtColor(mat, rgb, CV_BGR2RGB);

	return QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, QImage::Format_RGB888);
}


void GLWidget::processOpenCV()
{
	QImage qimage = QImage((const unsigned char*)(&m_rgb[0]), 640, 480, QImage::Format_RGB888);

	// - - - - - - - -
	// - - - - - - - -
	// - - - - - - - -


	// grab image from the kinect frame in the GUI
//	QImage qimage = ui.frameDepth->grabFrameBuffer();

	// convert QImage to OpeneCV's cv::Mat
	//
	// http://permalink.gmane.org/gmane.comp.lib.opencv/37800
	//
	//Mat qimage2mat(const QImage& qimage) {

	cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.bits(), qimage.bytesPerLine());
	cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
	int from_to[] = { 0,0,  1,1,  2,2 };
	cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );

	// result is now in 'mat2'

	mImage.release(); // reset picture

	// depth image from Kinect now! (mat2)
	cv::cvtColor( mat2, mImage, CV_BGR2RGB);

//	QImage tmp( (uchar*)mImage.data, mImage.cols, mImage.rows, mImage.step, QImage::Format_RGB888 );
//	ui.lblOpenCV->setPixmap( QPixmap::fromImage( tmp ) );


	if (mImage.data)
	{
		cv::Mat gray;
		cv::cvtColor( mImage, gray, CV_RGB2GRAY );
		cv::Canny( gray, gray, 10, 30, 3 );

		// convert b/w Mat to QImage
//		QImage tmp( (uchar*)gray.data, gray.cols, gray.rows, gray.step, QImage::Format_Indexed8  );

		// send image to GUI
//		ui.lblOpenCV->setPixmap( QPixmap::fromImage( tmp ) );
	}
}


void GLWidget::timerEvent(QTimerEvent *_event)
{
	// re-draw GL
	updateGL();
}
