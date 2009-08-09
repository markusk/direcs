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
	
	// real init done at initializeGL()
	cyl_radius = 0.0;
	cyl_height = 0.0;
	
	cubeWidth  = 0.0;
	cubeHeight = 0.0;
	cubeDepth  = 0.0;
}


CompassWidget::~CompassWidget()
{
	makeCurrent();
		
	if (cube)
		gluDeleteQuadric (cube);
		
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
	xAxisColor = Qt::red;
	yAxisColor = Qt::green;
	zAxisColor = Qt::blue;
	cubeColor = Qt::yellow;
	backgroundColor = Qt::black;
	
	
	qglClearColor(backgroundColor.dark());
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    
	glEnable(GL_DEPTH_TEST);
	
	GLfloat LightAmbient[] = {0.25, 0.25, 0.25, 1.0};		// Umgebungslicht
	GLfloat LightDiffuse[] = {0.4, 0.4, 0.4, 1.0};		// Bei diffusem Licht ist die Richtung des Lichts erkennbar, aus der es kommt
	GLfloat LightSpecular[] = {0.77, 0.77, 0.77, 1.0};		// Glanz
	
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
	
	cube = gluNewQuadric();
	
	gluQuadricNormals(xAxisCylinder, GLU_SMOOTH);
	gluQuadricNormals(yAxisCylinder, GLU_SMOOTH);
	gluQuadricNormals(zAxisCylinder, GLU_SMOOTH);
	
	gluQuadricNormals(xAxisCone, GLU_SMOOTH);
	gluQuadricNormals(yAxisCone, GLU_SMOOTH);
	gluQuadricNormals(zAxisCone, GLU_SMOOTH);
	
	gluQuadricNormals(cube, GLU_SMOOTH);
}


void CompassWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	// the cube
	qglColor(cubeColor);
	// object, baseradius, topradius, height, slices, stacks
	//gluCube(cubeWidth, cubeHeight, cubeDepth); // FIXME: gibts nicht?
	// move
	//glTranslatef(0.0, 0.0, cyl_height);
	
	float rquad = 0.15;
	cubeHeight = cyl_height;
	
	//glTranslatef(1.5f,0.0f,-7.0f);                          // Move Right And Into The Screen
	glTranslated(0.0, 0.0, 0.0);
	
	//glRotatef(rquad,1.0f,1.0f,1.0f);                        // Rotate The Cube On X, Y & Z
	glBegin(GL_QUADS);                                      // Start Drawing The Cube
	
	glColor3f(0.0f,1.0f,0.0f);                      // Set The Color To Green
	glVertex3f( cubeHeight, cubeHeight,-cubeHeight);                  // Top Right Of The Quad (Top)
	glVertex3f(-cubeHeight, cubeHeight,-cubeHeight);                  // Top Left Of The Quad (Top)
	glVertex3f(-cubeHeight, cubeHeight, cubeHeight);                  // Bottom Left Of The Quad (Top)
	glVertex3f( cubeHeight, cubeHeight, cubeHeight);                  // Bottom Right Of The Quad (Top)
	
	glColor3f(1.0f,0.5f,0.0f);                      // Set The Color To Orange
	glVertex3f( cubeHeight,-cubeHeight, cubeHeight);                  // Top Right Of The Quad (Bottom)
	glVertex3f(-cubeHeight,-cubeHeight, cubeHeight);                  // Top Left Of The Quad (Bottom)
	glVertex3f(-cubeHeight,-cubeHeight,-cubeHeight);                  // Bottom Left Of The Quad (Bottom)
	glVertex3f( cubeHeight,-cubeHeight,-cubeHeight);                  // Bottom Right Of The Quad (Bottom)
	
	glColor3f(1.0f,0.0f,0.0f);                      // Set The Color To Red
	glVertex3f( cubeHeight, cubeHeight, cubeHeight);                  // Top Right Of The Quad (Front)
	glVertex3f(-cubeHeight, cubeHeight, cubeHeight);                  // Top Left Of The Quad (Front)
	glVertex3f(-cubeHeight,-cubeHeight, cubeHeight);                  // Bottom Left Of The Quad (Front)
	glVertex3f( cubeHeight,-cubeHeight, cubeHeight);                  // Bottom Right Of The Quad (Front)

	glColor3f(1.0f,1.0f,0.0f);                      // Set The Color To Yellow
	glVertex3f( cubeHeight,-cubeHeight,-cubeHeight);                  // Bottom Left Of The Quad (Back)
	glVertex3f(-cubeHeight,-cubeHeight,-cubeHeight);                  // Bottom Right Of The Quad (Back)
	glVertex3f(-cubeHeight, cubeHeight,-cubeHeight);                  // Top Right Of The Quad (Back)
	glVertex3f( cubeHeight, cubeHeight,-cubeHeight);                  // Top Left Of The Quad (Back)

	glColor3f(0.0f,0.0f,1.0f);                      // Set The Color To Blue
	glVertex3f(-cubeHeight, cubeHeight, cubeHeight);                  // Top Right Of The Quad (Left)
	glVertex3f(-cubeHeight, cubeHeight,-cubeHeight);                  // Top Left Of The Quad (Left)
	glVertex3f(-cubeHeight,-cubeHeight,-cubeHeight);                  // Bottom Left Of The Quad (Left)
	glVertex3f(-cubeHeight,-cubeHeight, cubeHeight);                  // Bottom Right Of The Quad (Left)

	glColor3f(1.0f,0.0f,1.0f);                      // Set The Color To Violet
	glVertex3f( cubeHeight, cubeHeight,-cubeHeight);                  // Top Right Of The Quad (Right)
	glVertex3f( cubeHeight, cubeHeight, cubeHeight);                  // Top Left Of The Quad (Right)
	glVertex3f( cubeHeight,-cubeHeight, cubeHeight);                  // Bottom Left Of The Quad (Right)
	glVertex3f( cubeHeight,-cubeHeight,-cubeHeight);                  // Bottom Right Of The Quad (Right)

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
