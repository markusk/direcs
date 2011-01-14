/***************************************************************************
 *  This file is part of the OpenKinect Project. http://www.openkinect.org *
 *                                                                         *
 *  Copyright (c) 2010 individual OpenKinect contributors.                 *
 *  See the CONTRIB file for details.                                      *
 *                                                                         *
 *  This file is part of direcs.                                           *
 *  www.direcs.de                                                          *
 *                                                                         *
 *  direcs is free software: you can redistribute it and/or modify it      *
 *  under the terms of the GNU General Public License as published         *
 *  by the Free Software Foundation, version 3 of the License.             *
 *                                                                         *
 *  direcs is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with direcs. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                         *
 ***************************************************************************/

#include "RGBWindow.h"
#include <iostream>
#include <QDebug>
#include <QKinect.h>

//----------------------------------------------------------------------------------------------------------------------
RGBWindow::RGBWindow(
											QWidget *_parent
					) :
					QGLWidget(_parent)
{

  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());
	// start the timer to re-draw as quick as possible
	startTimer(0);
	// create space for our image data
	m_rgb.resize(640*480*3);
	// default to RGB mode
	m_mode=0;
}

// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.

//----------------------------------------------------------------------------------------------------------------------
void RGBWindow::initializeGL()
{

  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0);
	// set the texture data
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, & m_rgbTexture);
	glBindTexture(GL_TEXTURE_2D, m_rgbTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void RGBWindow::resizeGL(
						int _w,
						int _h
					   )
{
	glViewport(0,0,_w,_h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0, 640, 480, 0, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void RGBWindow::paintGL()
{
	// ok this is using immediate mode GL but will do for a sample
	// see my other NGL versions which are much faster
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QKinect *kinect=QKinect::instance();
	if(m_mode ==0)
	{
		kinect->getRGB(m_rgb);
	}
	else if(m_mode == 1)
	{
//		kinect->getDepth(m_rgb);
	}
	glBindTexture(GL_TEXTURE_2D, m_rgbTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &m_rgb[0]);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);

	glBegin(GL_TRIANGLE_FAN);
		glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
		glTexCoord2f(0, 0); glVertex3f(0,0,0);
		glTexCoord2f(1, 0); glVertex3f(640,0,0);
		glTexCoord2f(1, 1); glVertex3f(640,480,0);
		glTexCoord2f(0, 1); glVertex3f(0,480,0);


		// - - - - - -
		// drawPointClou

		if(m_mode == 1)
		{
			// ofScale(400, 400, 400);
			int w = 640;
			int h = 480;
			// ofRotateY(mouseX);
			// already done above: float* distancePixels = kinect->getDepth();

			glBegin(GL_POINTS);
			int step = 2;
			for (int y = 0; y < h; y += step)
			{
				for(int x = 0; x < w; x += step)
				{
					mx = 1;
					my = 1;
					mz = 1;
					ofPoint cur = kinect->getWorldCoordinateFor(x, y);
					glVertex3f(cur.x, cur.y, cur.z);
				}
			}
			// glEnd();
		}
		// - - - - - -


	glEnd();
}

//----------------------------------------------------------------------------------------------------------------------
void RGBWindow::mouseMoveEvent (
							   QMouseEvent * _event
							  )
{

  updateGL();
}


//----------------------------------------------------------------------------------------------------------------------
void RGBWindow::mousePressEvent (
								QMouseEvent * _event
							   )
{

}

//----------------------------------------------------------------------------------------------------------------------
void RGBWindow::mouseReleaseEvent (
								  QMouseEvent * _event
								 )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false

}

void RGBWindow::timerEvent(
						  QTimerEvent *_event
						 )
{
	// re-draw GL
	updateGL();

}

RGBWindow::~RGBWindow()
{

}



