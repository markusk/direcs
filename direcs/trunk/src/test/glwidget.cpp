/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the example classes of the Qt Toolkit.
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
** exception, Nokia gives you certain additional rights. These rights
** are described in the Nokia Qt GPL Exception version 1.3, included in
** the file GPL_EXCEPTION.txt in this package.
**
** Qt for Windows(R) Licensees
** As a special exception, Nokia, as the sole copyright holder for Qt
** Designer, grants users of the Qt/Eclipse Integration plug-in the
** right for the Qt/Eclipse Integration to link to functionality
** provided by Qt Designer and its related libraries.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/

#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"

//! [0]
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    object = 0;
    xRot = 0;
    yRot = 0;
    zRot = 0;

    xAxisColor = Qt::red;
    yAxisColor = Qt::green;
    zAxisColor = Qt::blue;
    backgroundColor = Qt::black;
}
//! [0]

//! [1]
GLWidget::~GLWidget()
{
    makeCurrent();
    glDeleteLists(object, 1);
}
//! [1]

//! [2]
QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}
//! [2]

//! [3]
QSize GLWidget::sizeHint() const
//! [3] //! [4]
{
    return QSize(400, 400);
}
//! [4]

//! [5]
void GLWidget::setXRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}
//! [5]

void GLWidget::setYRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

//! [6]
void GLWidget::initializeGL()
{
    qglClearColor(backgroundColor.dark());
    object = makeObject();
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}
//! [6]

//! [7]
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -10.0);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
    glCallList(object);
}
//! [7]

//! [8]
void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}
//! [8]

//! [9]
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}
//! [9]

//! [10]
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}
//! [10]

GLuint GLWidget::makeObject()
{
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);

    glBegin(GL_QUADS);

    GLdouble x1 = +0.06;
    GLdouble y1 = -0.14;
    GLdouble x2 = +0.14;
    GLdouble y2 = -0.06;
    GLdouble x3 = +0.08;
    GLdouble y3 = +0.00;
    GLdouble x4 = +0.30;
    GLdouble y4 = +0.22;
    
    GLdouble xAxisX1 = -0.05;
    GLdouble xAxisY1 = -0.20;
    GLdouble xAxisX2 = +0.05;
    GLdouble xAxisY2 = -0.20;
    GLdouble xAxisX3 = +0.05;
    GLdouble xAxisY3 = +0.20;
    GLdouble xAxisX4 = -0.05;
    GLdouble xAxisY4 = +0.20;
    
    GLdouble yAxisX1 = -0.05;
    GLdouble yAxisY1 = -0.05;
    GLdouble yAxisX2 = +0.20;
    GLdouble yAxisY2 = -0.05;
    GLdouble yAxisX3 = +0.20;
    GLdouble yAxisY3 = +0.05;
    GLdouble yAxisX4 = -0.05;
    GLdouble yAxisY4 = +0.05;
    

    // org: quad(x1, y1, x2, y2, y2, x2, y1, x1, xAxisColor); // waagerechter T-Strich, Ober- und Unterfläche
    quad(xAxisX1, xAxisY1, xAxisX2, xAxisY2, xAxisX3, xAxisY3, xAxisX4, xAxisY4, xAxisColor);
    // org: quad(x3, y3, x4, y4, y4, x4, y3, x3, yAxisColor); // senkrechter T-Strich, Ober- und Unterfläche
    quad(yAxisX1, yAxisY1, yAxisX2, yAxisY2, yAxisX3, yAxisY3, yAxisX4, yAxisY4, yAxisColor);

    extrude(xAxisX1, xAxisY1, xAxisX2, xAxisY2, xAxisColor); // kleine seitenfläche unten
    extrude(xAxisX3, xAxisY3, xAxisX4, xAxisY4, xAxisColor); // kleine seitenfläche oben
    extrude(xAxisX4, xAxisY4, xAxisX1, xAxisY1, xAxisColor); // große seitenfläche links
    extrude(xAxisX2, xAxisY2, xAxisX3, xAxisY3, xAxisColor); // große seitenfläche rechts

//     extrude(x1, y1, x2, y2, xAxisColor); //  waagerechter T-Strich, Aussenfläche rechts
//     extrude(x2, y2, y2, x2, xAxisColor); //  waagerechter T-Strich, Aussenfläche unten (Boden)
//     extrude(y2, x2, y1, x1, xAxisColor); //  waagerechter T-Strich, Aussenfläche links **
//     extrude(y1, x1, x1, y1, xAxisColor); //  waagerechter T-Strich, Aussenfläche oben (Dach)
//     extrude(x3, y3, x4, y4, yAxisColor); //  senkrechter T-Strich, Aussenfläche rechts
//     extrude(x4, y4, y4, x4, yAxisColor); //  senkrechter T-Strich, Aussenfläche unten (Boden) *
//     extrude(y4, x4, y3, x3, yAxisColor); //  senkrechter T-Strich, Aussenfläche links

    const double Pi = 3.14159265358979323846;
    const int NumSectors = 200;

// kreis
/*
    for (int i = 0; i < NumSectors; ++i) {
        double angle1 = (i * 2 * Pi) / NumSectors;
        GLdouble x5 = 0.30 * sin(angle1);
        GLdouble y5 = 0.30 * cos(angle1);
        GLdouble x6 = 0.20 * sin(angle1);
        GLdouble y6 = 0.20 * cos(angle1);

        double angle2 = ((i + 1) * 2 * Pi) / NumSectors;
        GLdouble x7 = 0.20 * sin(angle2);
        GLdouble y7 = 0.20 * cos(angle2);
        GLdouble x8 = 0.30 * sin(angle2);
        GLdouble y8 = 0.30 * cos(angle2);

        quad(x5, y5, x6, y6, x7, y7, x8, y8);

        extrude(x6, y6, x7, y7);
        extrude(x8, y8, x5, y5);
    }
*/

    glEnd();

    glEndList();
    return list;
}

void GLWidget::quad(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2, GLdouble x3, GLdouble y3, GLdouble x4, GLdouble y4, QColor color)
{
    qglColor(color);

    // definition von eckpunkten
    glVertex3d(x1, y1, -0.05);
    glVertex3d(x2, y2, -0.05);
    glVertex3d(x3, y3, -0.05);
    glVertex3d(x4, y4, -0.05);

    glVertex3d(x4, y4, +0.05);
    glVertex3d(x3, y3, +0.05);
    glVertex3d(x2, y2, +0.05);
    glVertex3d(x1, y1, +0.05);
}

void GLWidget::extrude(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2, QColor color)
{
    // extrude = durchdrücken

    qglColor(color.dark(250 + int(100 * x1)));

    glVertex3d(x1, y1, +0.05); // -6  14
    glVertex3d(x2, y2, +0.05); // -14  6
    glVertex3d(x2, y2, -0.05); // -14 6
    glVertex3d(x1, y1, -0.05); // -6 14
}

void GLWidget::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}
