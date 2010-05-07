/*************************************************************************
 *   Copyright (C) 2010 by Markus Knapp                                  *
 *   www.direcs.de                                                       *
 *                                                                       *
 *   This file is part of direcs.                                        *
 *                                                                       *
 *   direcs is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   direcs is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with direcs. If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                       *
 *************************************************************************/

#ifndef GLCONTEXT_H_
#define GLCONTEXT_H_

#include <Qt/qgl.h>
#include <QString>
#include <QtGui/QMouseEvent>

/**
\brief An individual OPENGL context class for camera stuff using OpenCV.

An individual OPENGL context class for camera stuff using OpenCV.
*/
class QtGLContext : public QGLWidget
{
	Q_OBJECT

	
public:
	//! Constructor
	QtGLContext(QWidget* parent = 0, const QGLWidget* sharedWidget = 0, Qt::WFlags f = 0 );
	QtGLContext(const QGLFormat &format, QWidget* parent = 0, const QGLWidget* sharedWidget = 0, Qt::WFlags f = 0 );
	~QtGLContext();

	//! set image parameters 'flipped' is a vertical flip!
	//bool setImage(unsigned char* imgP, const int width, const int height, const int pixeldepth, const bool flipped = false);
	bool setImage(unsigned char* imgP, const bool flipped = false);
	
	//! set the image data (for not doing this at every setImage call!)
	void setImageData(const int width, const int height, const int pixeldepth);

	//! redefines the clipping rectangle for the texture drawing
	void setZoomRect(float ulX, float ulY, float lrX, float lrY);

	//void resetBox() { m_exit.setX(0); m_exit.setY(0); m_entry.setX(0); m_entry.setY(0);}

	/**
	Enables or disables the mirror mode for the camera. When activated, the camera live image is mirrored horicontally.
	@param state has to be Qt::Checked to enable the detection. All other states disable.
	*/ 
	void enableMirrorMode(int state);

	
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
	/**
	draws the texture sub image on the screen
	@param ul is the upper left coordinate
	@param ur is the upper right coordinate
	@param ll is the lower left coordinate
	@param lr is the lower right coordinate
	*/
	void drawTexture(float ul, float ur, float ll, float lr);
	//! helper method
	unsigned int NextLargerPowerOfTwo(unsigned int n);


private:
	//! pointer to the data of the displayed image
	unsigned char* m_imgP;
	//! image parameters
	unsigned int m_width, m_height, m_pixeldepth;
	//! if set to true image will be flipped vertically
    bool         m_flipped;
	//! if set to true image will be flipped horizontally
    bool         m_mirrored;
	//! width of the texture buffer
	unsigned int m_texWidth;
	//! height of the texture buffer
	unsigned int m_texHeight;
	//! texture identifier
	unsigned int m_texNameGL;
	//! clipping rectangle for texture
	float m_texUpperLeftX, m_texUpperLeftY, m_texLowerRightX, m_texLowerRightY;
	// box parameters (for mouse dragging)
	QPoint m_entry, m_exit;	
	bool m_mousePressed;
	// "the image" (texture)
	unsigned char* textureGL;

};

#endif
