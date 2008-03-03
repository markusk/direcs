#ifndef GLCONTEXT_H_
#define GLCONTEXT_H_

#include <Qt/qgl.h>
#include <QString>
#include <QtGui/QMouseEvent>

/** An individual OPENGL context class */

class QtGLContext : public QGLWidget
{
	Q_OBJECT

public:
	/// Constructor
	QtGLContext(QWidget* parent = 0, const QGLWidget* sharedWidget = 0, Qt::WFlags f = 0 );
	QtGLContext(const QGLFormat &format, QWidget* parent = 0, const QGLWidget* sharedWidget = 0, Qt::WFlags f = 0 );
	~QtGLContext();

	// set image parameters
	bool setImage(
		unsigned char* imgP, 
		const int width, 
		const int height, 
		const int pixeldepth,
		const bool flipped = false, // vertical flip!
		const bool mirrored = false); // horicontal flip!

	// redefines the clipping rectangle for the texture drawing
	void setZoomRect(float ulX, float ulY, float lrX, float lrY);

	void resetBox() { m_exit.setX(0); m_exit.setY(0); m_entry.setX(0); m_entry.setY(0);}

signals:
	void boxCreated(int x, int y, int x2, int y2);
	void saveImage();

protected:

	// standard qt methods
	virtual void paintGL();
	virtual void resizeGL(int w, int h);
	virtual void initializeGL();

	/*
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	*/
	// internal draw method
	void drawTexture(float ul, float ur, float ll, float lr);
	// helper method
	unsigned int NextLargerPowerOfTwo(unsigned int n);


private:
	
	// pointer to the data of the displayed image
	unsigned char* m_imgP;
	// image parameters
	unsigned int m_width, m_height, m_pixeldepth;
	// if set to true image will be flipped vertically
    bool         m_flipped;
	// if set to true image will be flipped horizontally
    bool         m_mirrored;
	// width of the texture buffer
	unsigned int m_texWidth;
	// height of the texture buffer
	unsigned int m_texHeight;
	// texture identifier
	unsigned int m_texNameGL;
	// clipping rectangle for texture
	float m_texUpperLeftX, m_texUpperLeftY, m_texLowerRightX, m_texLowerRightY;
	// box parameters (for mouse dragging)
	QPoint m_entry, m_exit;	
	bool m_mousePressed;

};

#endif
