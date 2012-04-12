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

#include "compassWidget.h"


CompassWidget::CompassWidget(QWidget *parent) : QGLWidget(parent)
{
	xRot = 0.0;
	yRot = 0.0;
	zRot = 0.0;

	// initial view settings
	m_mouseAngleH = 0.0;
	m_mouseAngleV = 0.0;
	m_mouseLastX = 0;
	m_mouseLastY = 0;

	// initialize quadric pointers
	xAxisCylinder = NULL;
	yAxisCylinder = NULL;
	zAxisCylinder = NULL;
	xAxisCone = NULL;
	yAxisCone = NULL;
	zAxisCone = NULL;

	// initialize texture pointers
	robotTextureFront = 0;
	robotTextureBack = 0;
	robotTextureLeft = 0;
	robotTextureRight = 0;

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

	/*
	// get color from main window background
	// So the background color of the OpenGL widget will be like the background color of the main application!
	\todo buggy. brings a dark gray background?!??  backgroundColor = QApplication::palette().base().color();
	*/
	backgroundColor = Qt::black;
}


CompassWidget::~CompassWidget()
{
	makeCurrent();

	if (robotTextureFront)
		deleteTexture(robotTextureFront);

	if (robotTextureBack)
		deleteTexture(robotTextureBack);

	if (robotTextureLeft)
		deleteTexture(robotTextureLeft);

	if (robotTextureRight)
		deleteTexture(robotTextureRight);

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
/*
	robotImageFront.load(":/images/images/bot_front.png");
	robotImageBack.load(":/images/images/bot_back.png");
	robotImageLeft.load(":/images/images/bot_left.png");
	robotImageRight.load(":/images/images/bot_right.png");
*/
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
//	qglClearColor(backgroundColor.dark());
	glClearColor(0.93f, 0.93f, 0.93f, 0.0f);			   // Grey Background

	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	//----------------------- pfeile -------------------------
	GLfloat LightAmbient[] = {0.25, 0.25, 0.25, 1.0};	// Umgebungslicht
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
	//----------------------- pfeile -------------------------

	cyl_radius = 0.03;
	cyl_height = 0.30;

	cubeWidth  = 0.40;
	cubeHeight = 0.18;
	cubeDepth  = 0.40;

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

	//----------------------- pfeile -------------------------
	glTranslated(0.0, 0.0, -10.0);

	// enable roation
	glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
	//----------------------- pfeile -------------------------

/*
	//-------------------------- texture ------------------------
	glRotatef(m_mouseAngleH, 1.0, 0.0, 0.0);
	glRotatef(m_mouseAngleV, 0.0, 1.0, 0.0);
	//glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
	//-------------------------- texture ------------------------
*/

	//----------------------- pfeile -------------------------
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
	//----------------------- pfeile -------------------------

/*
	//-------------------------- texture ------------------------
	// use mipmapped textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	// \todo should be located elsewhere
	static GLfloat no_mat[] = {0.0, 0.0, 0.0, 1.0};
	static GLfloat mat_diffuse[] = {0.5, 0.5, 0.5, 1.0};
	static GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	static GLfloat low_shininess[] = {2.5};
// 	static GLfloat translucent[] = {1.0, 1.0, 1.0, 0.33};

	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	// bind textures
	robotTextureFront = bindTexture(robotImageFront, GL_TEXTURE_2D, GL_RGBA);
	robotTextureBack = bindTexture(robotImageBack, GL_TEXTURE_2D, GL_RGBA);
	robotTextureLeft = bindTexture(robotImageLeft, GL_TEXTURE_2D, GL_RGBA);
	robotTextureRight = bindTexture(robotImageRight, GL_TEXTURE_2D, GL_RGBA);

	// enable texturing
	glEnable(GL_TEXTURE_2D);

	// move world 7 units away from the current view point
	glTranslated(0.0, 0.0, -7.0);

	// create FRONT texture
	glBindTexture(GL_TEXTURE_2D, robotTextureFront);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);		glVertex3f(-cubeWidth, cubeHeight, cubeHeight); // Top Left			1
	glTexCoord2f(1.0, 0.0);		glVertex3f( cubeWidth, cubeHeight, cubeHeight); // Top Right		2
	glTexCoord2f(1.0, 1.0);		glVertex3f( cubeWidth,-cubeHeight, cubeHeight); // Bottom Right		3
	glTexCoord2f(0.0, 1.0);		glVertex3f(-cubeWidth,-cubeHeight, cubeHeight); // Bottom Left		4
	glEnd();


	// create RIGHT texture
	glBindTexture(GL_TEXTURE_2D, robotTextureRight);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);		glVertex3f( cubeHeight, cubeHeight, cubeWidth); // Top Left			1
	glTexCoord2f(1.0, 0.0);		glVertex3f( cubeHeight, cubeHeight,-cubeWidth); // Top Right		2
	glTexCoord2f(1.0, 1.0);		glVertex3f( cubeHeight,-cubeHeight,-cubeWidth); // Bottom Right		3
	glTexCoord2f(0.0, 1.0);		glVertex3f( cubeHeight,-cubeHeight, cubeWidth); // Bottom Left		4
	glEnd();

	// create LEFT texture
	glBindTexture(GL_TEXTURE_2D, robotTextureLeft);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);		glVertex3f(-cubeHeight, cubeHeight,-cubeWidth); // Top Left			1
	glTexCoord2f(1.0, 0.0);		glVertex3f(-cubeHeight, cubeHeight, cubeWidth); // Top Right		2
	glTexCoord2f(1.0, 1.0);		glVertex3f(-cubeHeight,-cubeHeight, cubeWidth); // Bottom Right		3
	glTexCoord2f(0.0, 1.0);		glVertex3f(-cubeHeight,-cubeHeight,-cubeWidth); // Bottom Left		4
	glEnd();

	// create BACK texture
	glBindTexture(GL_TEXTURE_2D, robotTextureBack);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);		glVertex3f( cubeHeight, cubeHeight,-cubeWidth); // Top Left			1
	glTexCoord2f(1.0, 0.0);		glVertex3f(-cubeHeight, cubeHeight,-cubeWidth); // Top Right		2
	glTexCoord2f(1.0, 1.0);		glVertex3f(-cubeHeight,-cubeHeight,-cubeWidth); // Bottom Right		3
	glTexCoord2f(0.0, 1.0);		glVertex3f( cubeHeight,-cubeHeight,-cubeWidth); // Bottom Left		4
	glEnd();

	glDisable(GL_TEXTURE_2D);
	//-------------------------- texture ------------------------
*/
}


void CompassWidget::resizeGL(int width, int height)
{
	int side = qMin(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side); // \todo check this viewport stuff

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
	if (event->buttons() & Qt::LeftButton)
	{
		if(lastPos.x() != 0)
		{
			m_mouseAngleH -= (lastPos.y() - event->y());
			m_mouseAngleH = m_mouseAngleH < 360 ? m_mouseAngleH : 0;
			m_mouseAngleH = m_mouseAngleH >= 0 ? m_mouseAngleH : 359;
		}
		if(lastPos.y() != 0)
		{
			m_mouseAngleV += (lastPos.x() - event->x());
			m_mouseAngleV = m_mouseAngleV < 90 ? m_mouseAngleV : 90;
			m_mouseAngleV = m_mouseAngleV > -90 ? m_mouseAngleV : -90;
		}

		updateGL();

		// store current x and y pos
		lastPos = event->pos();
	}
	else if (event->buttons() & Qt::RightButton)
	{
		/*
		if(lastPos.y() != 0)
		{
			m_cameraZoom -= (lastPos.y() - event->y());
			m_cameraZoom = m_cameraZoom >= m_cameraZoomLBound ? m_cameraZoom : m_cameraZoomLBound;
			m_cameraZoom = m_cameraZoom >= m_cameraZoomUBound ? m_cameraZoomUBound : m_cameraZoom;
		}
		*/

		updateGL();

		// store current x and y pos
		lastPos = event->pos();
	}
}


void CompassWidget::normalizeAngle(float *angle)
{
	while (*angle < 0.0)
		*angle += 360.0 * 16.0;
	while (*angle > 360.0 * 16.0)
		*angle -= 360.0 * 16.0;
}


void CompassWidget::setXRotation(float angle)
{
	normalizeAngle(&angle);
	if (angle != xRot)
	{
		xRot = angle;
		//emit xRotationChanged(angle);
		updateGL();
	}
}


void CompassWidget::setYRotation(float angle)
{
	normalizeAngle(&angle);
	if (angle != yRot)
	{
		yRot = angle;
		//emit yRotationChanged(angle);
		updateGL();
	}
}


void CompassWidget::setZRotation(float angle)
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
	setXRotation( xAngle );
	setYRotation( yAngle );
	setZRotation( zAngle );
}


QSize CompassWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}


QSize CompassWidget::sizeHint() const
{
	return QSize(800, 800);
}


void CompassWidget::setBackgroundColor(QColor col)
{
	backgroundColor = col;
}
