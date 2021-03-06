/*************************************************************************
 *   Copyright (C) Markus Knapp                                          *
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

#include "QtGLContext.h"
#include <QMessageBox>

QtGLContext::QtGLContext(QWidget* parent /*= 0*/, const QGLWidget* sharedWidget /*= 0*/, Qt::WFlags f /*= 0*/) : QGLWidget(parent, sharedWidget, f), m_imgP(0), m_mousePressed(false)
{
    // first init
	m_mirrored = false;
	m_width = 0;
	m_height = 0;
	m_pixeldepth = 0;
}


QtGLContext::~QtGLContext()
{
	//delete[] textureGL;
	glDeleteTextures(1, &m_texNameGL);
}


void QtGLContext::initializeGL()
{
	// INIT
	// Let OpenGL clear to black
	glColor3f(0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the shading model
	glShadeModel(GL_FLAT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGenTextures(1, &m_texNameGL);
}


void QtGLContext::paintGL()
 {
	// PAINT
	glColor3f(1.0,1.0,1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (!m_imgP)
		return;
	
	glDisable(GL_DEPTH_TEST);
	drawTexture(m_texUpperLeftX, m_texUpperLeftY, m_texLowerRightX, m_texLowerRightY);
	glEnable(GL_DEPTH_TEST);
	
	if (!m_exit.isNull())
	{
		glBegin(GL_LINE_STRIP);
		glVertex2f(m_entry.x(), m_entry.y());
		glVertex2f(m_entry.x(), m_exit.y());
		glVertex2f(m_exit.x(), m_exit.y());
		glVertex2f(m_exit.x(), m_entry.y());
		glVertex2f(m_entry.x(), m_entry.y());
		glEnd();
	}
}


void QtGLContext::drawTexture(float ulX, float ulY, float lrX, float lrY)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texNameGL);
	
	switch (m_pixeldepth)
	{
		case 8:
			glTexSubImage2D ( GL_TEXTURE_2D, 0, m_texWidth-m_width, m_texHeight-m_height, m_width, m_height, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_imgP);
			break;
		case 24:
			glTexSubImage2D ( GL_TEXTURE_2D, 0, m_texWidth-m_width, m_texHeight-m_height, m_width, m_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_imgP);
			break;
		case 32:
			glTexSubImage2D ( GL_TEXTURE_2D, 0, m_texWidth-m_width, m_texHeight-m_height, m_width, m_height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, m_imgP);
			break;
	}

	// Reset The Modelview Matrix
	glLoadIdentity();
	glBegin(GL_QUADS);

	if (m_flipped)
	{	
        //flipped vertically
		glTexCoord2f(ulX, ulY);    glVertex2f( -1, -1 );
		glTexCoord2f(ulX, lrY);    glVertex2f( -1,  1 );
		glTexCoord2f(lrX, lrY);    glVertex2f(  1,  1 );
		glTexCoord2f(lrX, ulY);    glVertex2f(  1, -1 );
	}
	else
	{
		glTexCoord2f(ulX, ulY);    glVertex2f( -1,  1 );
		glTexCoord2f(ulX, lrY);    glVertex2f( -1, -1 );
		glTexCoord2f(lrX, lrY);    glVertex2f(  1, -1 );
		glTexCoord2f(lrX, ulY);    glVertex2f(  1,  1 );
	}

	if (m_mirrored)
	{	
        //flipped horicontally
		glTexCoord2f(ulX, ulY);    glVertex2f(  1,  1 );
		glTexCoord2f(ulX, lrY);    glVertex2f(  1, -1 );
		glTexCoord2f(lrX, lrY);    glVertex2f( -1, -1 );
		glTexCoord2f(lrX, ulY);    glVertex2f( -1,  1 );
	}
	else
	{
		glTexCoord2f(ulX, ulY);    glVertex2f( -1,  1 );
		glTexCoord2f(ulX, lrY);    glVertex2f( -1, -1 );
		glTexCoord2f(lrX, lrY);    glVertex2f(  1, -1 );
		glTexCoord2f(lrX, ulY);    glVertex2f(  1,  1 );
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glTranslatef(-1.0,1.0,0);
	glScalef(2.0/m_width, 2.0/m_height, 0.0);
}



unsigned int QtGLContext::NextLargerPowerOfTwo(unsigned int n) {
	unsigned int candidate = n;

	while((candidate & (candidate - 1)) != 0) {
		++candidate;
	}
	return candidate;
}


/*
void QtGLContext::mousePressEvent ( QMouseEvent * event ) 
{
	// remember the current mouse position
	if (!m_imgP)
		return;
	float scale = static_cast<float>(m_width)/static_cast<float>(this->width());
	m_entry.setX(event->pos().x()*scale);
	scale = static_cast<float>(m_height)/ static_cast<float>(this->height());
	m_entry.setY(-event->pos().y()*scale);
	m_exit = QPoint(0,0);
	m_mousePressed = true;
	this->updateGL();
	
	emit saveImage();
}


void QtGLContext::mouseReleaseEvent(QMouseEvent* event)
{
	if (!m_imgP || !m_mousePressed)
		return;
	float scale = static_cast<float>(m_width)/static_cast<float>(this->width());
	m_exit.setX(event->pos().x()*scale);
	scale = static_cast<float>(m_height)/ static_cast<float>(this->height());
	m_exit.setY(-event->pos().y()*scale);
	int minx, maxx, miny, maxy;
	if (m_entry.x() == m_exit.x() || m_entry.y() == m_exit.y())
		return;
	if (m_entry.x() < m_exit.x())
	{
		minx = m_entry.x();
		maxx = m_exit.x();
	}
	else
	{
		minx = m_exit.x();
		maxx = m_entry.x();
	}
	if (-m_entry.y() < -m_exit.y())
	{
		miny = -m_entry.y();
		maxy = -m_exit.y();
	}
	else
	{
		miny = -m_exit.y();
		maxy = -m_entry.y();
	}
	if (maxy >= m_height) maxy = m_height-1;
	if (maxx >= m_width) maxx = m_width-1;
	if (miny < 0) miny = 0;
	if (minx < 0) minx = 0;
	emit boxCreated(minx, miny, maxx, maxy);
	m_mousePressed = false;
	this->updateGL();
}

void QtGLContext::mouseMoveEvent(QMouseEvent* event)
{
	if (!m_imgP || !m_mousePressed)
		return;
	float scale = static_cast<float>(m_width)/static_cast<float>(this->width());
	m_exit.setX(event->pos().x()*scale);
	scale = static_cast<float>(m_height)/ static_cast<float>(this->height());
	m_exit.setY(-event->pos().y()*scale);
	this->updateGL();
}
*/


void QtGLContext::resizeGL(int w, int h)
{	
	// RESIZE
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}


bool QtGLContext::setImage(unsigned char* imgP, const bool flipped)  /*flipped=false*/
//bool QtGLContext::setImage(unsigned char* imgP, const int width, const int height, const int pixeldepth, const bool flipped)  /*flipped=false*/
{
	m_imgP = imgP;
	/*
	m_width = width;
	m_height = height;
	m_pixeldepth = pixeldepth;
	*/
    m_flipped = flipped;

	this->updateGL();

	// only 8, 24 and 32 bit images are supported
	if (!imgP || (m_pixeldepth!=8 && m_pixeldepth!=24 && m_pixeldepth!=32))
		return false;
	
	// Enable Texture Mapping
	glEnable(GL_TEXTURE_2D);
	
	// calculate texture size
	// create texture memory -> put to constructor!
	// calculate texture coordinates for image
	// -> now in setImageData
	
	// tell OpenGL which texture "id" we will be working with.:
	glBindTexture(GL_TEXTURE_2D, m_texNameGL);
	// tell OpenGL that the pixel data which is going to be passed to it is aligned in byte order:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// set the various parameters for the current OpenGL texture:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// tell OpenGL how the texture will act when it is rendered into a scene:
	// The GL_MODULATE attribute allows you to apply effects such as lighting 
	// and coloring to your texture. If you do not want lighting and coloring to effect 
	// your texture and you would like to display the texture unchanged when coloring 
	// is applied replace GL_MODULATE with GL_DECAL
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	switch(m_pixeldepth)
	{
		case 8:
			glTexImage2D(GL_TEXTURE_2D, 0,  GL_LUMINANCE, m_texWidth, m_texHeight, 0,  GL_LUMINANCE, GL_UNSIGNED_BYTE, textureGL); 
			break;
		case 24:
			glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGB, m_texWidth,m_texHeight, 0,  GL_RGB, GL_UNSIGNED_BYTE, textureGL); 
			break;
		case 32:
			glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, m_texWidth,m_texHeight, 0,  GL_RGBA, GL_UNSIGNED_BYTE, textureGL); 
			break;	
		default:
			glDisable(GL_TEXTURE_2D);
			return false;
	}
	
	glDisable(GL_TEXTURE_2D);
	//resetBox();
 	return true;
}


void QtGLContext::enableMirrorMode(int state)
{
	if (state == Qt::Checked)
	{
		m_mirrored = true;
	}
	else
	{
		m_mirrored = false;
	}
}


void QtGLContext::setImageData(const int width, const int height, const int pixeldepth)
{
	static bool firstInit = true;


	m_width = width;
	m_height = height;
	m_pixeldepth = pixeldepth;
	
	// only the first time
	if (firstInit)
	{
		firstInit=false;
		
		// calculate texture size
		m_texWidth  = NextLargerPowerOfTwo(m_width);
		m_texHeight = NextLargerPowerOfTwo(m_height);
		
		// create texture memory -> put to constructor!
		textureGL = new GLubyte[m_texHeight*m_texWidth* (m_pixeldepth>>3)];
		
		// calculate texture coordinates for image
		m_texUpperLeftX = float (m_texWidth-m_width) / (float) (m_texWidth);
		m_texUpperLeftY = float (m_texHeight-m_height) / (float) (m_texHeight);
		m_texLowerRightX = 1.0; // (float) (_texWidth) / (float) _height;
		m_texLowerRightY = 1.0; // (float) (_texHeight) / (float) _width;
	}
}
