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

#ifndef COMPASSWIDGET_H
#define COMPASSWIDGET_H

#include <QtGui>
#include <QtOpenGL>
#include <QGLWidget>

#include <math.h>


class CompassWidget : public QGLWidget
{
	Q_OBJECT

	public:
		CompassWidget(QWidget *parent = 0);
		~CompassWidget();
	
		QSize minimumSizeHint() const;
		QSize sizeHint() const;

	public slots:
		void setXRotation(int angle);
		void setYRotation(int angle);
		void setZRotation(int angle);
		
		/**
		This is an additional member function, provided for convenience.
		
		@param xAngle should be between 0.0 and 360.0
		@param yAngle should be between 0.0 and 360.0
		@param zAngle should be between 0.0 and 360.0
		*/
		void setAllRotations(float xAngle, float yAngle, float zAngle);

	signals:
		void xRotationChanged(int angle);
		void yRotationChanged(int angle);
		void zRotationChanged(int angle);


	protected:
		void initializeGL();
		void paintGL();
		void resizeGL(int width, int height);
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);


	private:
		void normalizeAngle(int *angle);
	
		GLUquadricObj* xAxis;
		GLUquadricObj* yAxis;
		GLUquadricObj* zAxis;
		GLUquadricObj* xAxisCone;
		GLUquadricObj* yAxisCone;
		GLUquadricObj* zAxisCone;
	
		int xRot;
		int yRot;
		int zRot;
		GLdouble cyl_radius;
		GLdouble cyl_height;
		QPoint lastPos;
		QColor xAxisColor;
		QColor yAxisColor;
		QColor zAxisColor;
		QColor backgroundColor;
};

#endif
