/*************************************************************************
 *   Copyright (C) 2009 by Markus Knapp                                  *
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

#include "compassWidget.h"


CompassWidget::CompassWidget(QWidget *parent) : QGLWidget(parent)
{
	xRot = 0;
	yRot = 0;
	zRot = 0;
	
	// initialize quadric pointers
	xAxisCylinder = NULL;
	yAxisCylinder = NULL;
	zAxisCylinder = NULL;
	xAxisCone = NULL;
	yAxisCone = NULL;
	zAxisCone = NULL;
	
	// initialize texture pointers
	m_robotTextureAbove = 0;
	
	// real init done at initializeGL()
	cyl_radius = 0.0;
	cyl_height = 0.0;
	
	cubeWidth  = 0.0;
	cubeHeight = 0.0;
	cubeDepth  = 0.0;
	
	// set the colors
	xAxisColor = Qt::red;
	yAxisColor = Qt::green;
	zAxisColor = Qt::blue;
	backgroundColor = Qt::black;
}


CompassWidget::~CompassWidget()
{
	makeCurrent();
		
	if (m_robotTextureAbove)
		deleteTexture(m_robotTextureAbove);
			
	if (zAxisCone)
		gluDeleteQuadric (zAxisCone);
		
	if (yAxisCone)
		gluDeleteQuadric (yAxisCone);
		
	if (xAxisCone)
		gluDeleteQuadric (xAxisCone);
	
	if (zAxisCylinder)
		gluDeleteQuadric (zAxisCylinder);
		
	if (yAxisCylinder)
		gluDeleteQuadric (yAxisCylinder);
		
	if (xAxisCylinder)
		gluDeleteQuadric (xAxisCylinder);
}


void CompassWidget::initializeGL()
{
	robotTextureTop.load(":/images/images/bot_from_above.png");
	
// 	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	qglClearColor(backgroundColor.dark());
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	GLfloat LightAmbient[] = {0.95, 0.95, 0.95, 1.0};	// Umgebungslicht
	GLfloat LightDiffuse[] = {0.4, 0.4, 0.4, 1.0};		// Bei diffusem Licht ist die Richtung des Lichts erkennbar, aus der es kommt
	GLfloat LightSpecular[] = {0.77, 0.77, 0.77, 1.0};	// Glanz
	
	GLfloat LightPosition[] = {0.0, 0.0, 3.0, 1.0};
	GLfloat spot_direction[] = {0.0, 0.0, -1.0}; // Richtung in die das Spotlight zeigt
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 50.0); // gibt den Winkel an, der zwischen Richtung und max. Auswurf besteht
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0); // gibt an, wie stark die lichtstärke nach aussen abnimmt
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL); // neu
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND); // erforderlich, wenn Materialien durchsichtig sind
	
	cyl_radius = 0.03;
	cyl_height = 0.30;
	
	cubeWidth = 0.30;
	cubeHeight = 0.30;
	cubeDepth = 0.30;
	
	xAxisCylinder = gluNewQuadric();
	yAxisCylinder = gluNewQuadric();
	zAxisCylinder = gluNewQuadric();
	
	xAxisCone = gluNewQuadric();
	yAxisCone = gluNewQuadric();
	zAxisCone = gluNewQuadric();
}


void CompassWidget::paintGL()
{
	// clear last scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// reset modelview matrix
	glLoadIdentity();
	
	glTranslated(0.0, 0.0, -10.0);
	
	glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
	
	
	// X cylinder (red)
	qglColor(xAxisColor);
	// object, baseradius, topradius, height, slices, stacks
	gluCylinder(xAxisCylinder, cyl_radius, cyl_radius, cyl_height, 32, 32);
	// move
	glTranslatef(0.0, 0.0, cyl_height);
	qglColor(xAxisColor);
	// X cone
	gluCylinder(xAxisCone, (cyl_radius*1.5), 0.0, cyl_height/2.0, 32, 32);
	
	// Y cylinder (green)
	qglColor(yAxisColor);
	// move back and rotate one axis
	glTranslatef(0.0, 0.0, - (cyl_height - cyl_radius));
	glRotated(90, 1.0, 0.0, 0.0);
	// object, baseradius, topradius, height, slices, stacks
	gluCylinder(yAxisCylinder, cyl_radius, cyl_radius, cyl_height, 32, 32);
	// move
	glTranslatef(0.0, 0.0, cyl_height);
	qglColor(yAxisColor);
	// Y cone
	gluCylinder(yAxisCone, (cyl_radius*1.5), 0.0, cyl_height/2.0, 32, 32);
	
	// Z cylinder (blue)
	qglColor(zAxisColor);
	// move back and rotate one axis
	glTranslatef(0.0, 0.0, - (cyl_height));
	glRotated(90, 0.0, 1.0, 0.0);
	// object, baseradius, topradius, height, slices, stacks
	gluCylinder(zAxisCylinder, cyl_radius, cyl_radius, cyl_height, 32, 32);
	// move
	glTranslatef(0.0, 0.0, cyl_height);
	qglColor(zAxisColor);
	// Z cone
	gluCylinder(zAxisCone, (cyl_radius*1.5), 0.0, cyl_height/2.0, 32, 32);

	//----------------------------------------------------------------------------------
/*	
	// use mipmapped textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
*/	
	// TODO: should be located elsewhere
	static GLfloat no_mat[] = {0.0, 0.0, 0.0, 1.0};
	static GLfloat mat_diffuse[] = {0.5, 0.5, 0.5, 1.0};
	static GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	static GLfloat low_shininess[] = {2.5};
	static GLfloat translucent[] = {1.0, 1.0, 1.0, 0.33};
	
 	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
 	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
 	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
 	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
 	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	// bind textures
	m_robotTextureAbove = bindTexture(robotTextureTop, GL_TEXTURE_2D, GL_RGBA);
	
	// create texture coordinates and enable texturing
	glBindTexture(GL_TEXTURE_2D, m_robotTextureAbove);

	// enable texturing
	glEnable(GL_TEXTURE_2D);

	glTranslatef(0.0, 0.0, 0.0);
	
	
	glBegin(GL_QUADS);
	
	//qglColor(Qt::cyan);  // Set The Color To Orange
	glTexCoord2f(0.0, 0.0);		glVertex3f(-cubeHeight,-cubeHeight, cubeHeight); // Top Left Of The Quad (Bottom)
	glTexCoord2f(1.0, 0.0);		glVertex3f( cubeHeight,-cubeHeight, cubeHeight); // Top Right Of The Quad (Bottom)
	glTexCoord2f(1.0, 1.0);		glVertex3f( cubeHeight,-cubeHeight,-cubeHeight); // Bottom Right Of The Quad (Bottom)
	glTexCoord2f(0.0, 1.0);		glVertex3f(-cubeHeight,-cubeHeight,-cubeHeight); // Bottom Left Of The Quad (Bottom)
	
	// disable texturing
	glDisable(GL_TEXTURE_2D);
/*

	qglColor(yAxisColor);                      // Set The Color To Red
	glVertex3f( cubeHeight, cubeHeight, cubeHeight);                  // Top Right Of The Quad (Front)
	glVertex3f(-cubeHeight, cubeHeight, cubeHeight);                  // Top Left Of The Quad (Front)
	glVertex3f(-cubeHeight,-cubeHeight, cubeHeight);                  // Bottom Left Of The Quad (Front)
	glVertex3f( cubeHeight,-cubeHeight, cubeHeight);                  // Bottom Right Of The Quad (Front)

	qglColor(Qt::yellow);                      // Set The Color To Yellow
	glVertex3f( cubeHeight,-cubeHeight,-cubeHeight);                  // Bottom Left Of The Quad (Back)
	glVertex3f(-cubeHeight,-cubeHeight,-cubeHeight);                  // Bottom Right Of The Quad (Back)
	glVertex3f(-cubeHeight, cubeHeight,-cubeHeight);                  // Top Right Of The Quad (Back)
	glVertex3f( cubeHeight, cubeHeight,-cubeHeight);                  // Top Left Of The Quad (Back)

	qglColor(zAxisColor);                      // Set The Color To Blue
	glVertex3f(-cubeHeight, cubeHeight, cubeHeight);                  // Top Right Of The Quad (Left)
	glVertex3f(-cubeHeight, cubeHeight,-cubeHeight);                  // Top Left Of The Quad (Left)
	glVertex3f(-cubeHeight,-cubeHeight,-cubeHeight);                  // Bottom Left Of The Quad (Left)
	glVertex3f(-cubeHeight,-cubeHeight, cubeHeight);                  // Bottom Right Of The Quad (Left)

	qglColor(Qt::magenta);                      // Set The Color To Violet
	glVertex3f( cubeHeight, cubeHeight,-cubeHeight);                  // Top Right Of The Quad (Right)
	glVertex3f( cubeHeight, cubeHeight, cubeHeight);                  // Top Left Of The Quad (Right)
	glVertex3f( cubeHeight,-cubeHeight, cubeHeight);                  // Bottom Left Of The Quad (Right)
	glVertex3f( cubeHeight,-cubeHeight,-cubeHeight);                  // Bottom Right Of The Quad (Right)
*/
	glEnd();                                                // Done Drawing The Quad
}


void CompassWidget::resizeGL(int width, int height)
{
	int side = qMin(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
	glMatrixMode(GL_MODELVIEW);
}


void CompassWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
}


void CompassWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();
	
	if (event->buttons() & Qt::LeftButton)
	{
		setXRotation(xRot + 8 * dy);
		setYRotation(yRot + 8 * dx);
	} else if (event->buttons() & Qt::RightButton)
	{
		setXRotation(xRot + 8 * dy);
		setZRotation(zRot + 8 * dx);
	}
	lastPos = event->pos();
}


void CompassWidget::normalizeAngle(int *angle)
{
	while (*angle < 0)
		*angle += 360 * 16;
	while (*angle > 360 * 16)
		*angle -= 360 * 16;
}


QSize CompassWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}


QSize CompassWidget::sizeHint() const
{
	return QSize(800, 800);
}


void CompassWidget::setXRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != xRot)
	{
		xRot = angle;
		//emit xRotationChanged(angle);
		updateGL();
	}
}


void CompassWidget::setYRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != yRot)
	{
		yRot = angle;
		//emit yRotationChanged(angle);
		updateGL();
	}
}


void CompassWidget::setZRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != zRot)
	{
		zRot = angle;
		//emit zRotationChanged(angle);
		updateGL();
	}
}


void CompassWidget::setAllRotations(float xAngle, float yAngle, float zAngle)
{
// 	setXRotation( (int) (xAngle*16) );
// 	setYRotation( (int) (yAngle*16) );
// 	setZRotation( (int) (zAngle*16) );
	setXRotation( (int) xAngle );
	setYRotation( (int) yAngle );
	setZRotation( (int) zAngle );
}
