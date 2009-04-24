#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"


GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
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


GLWidget::~GLWidget()
{
    makeCurrent();
    
    gluDeleteQuadric (q);
}


QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}


QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}


void GLWidget::setXRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}


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


void GLWidget::initializeGL()
{
    qglClearColor(backgroundColor.dark());
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
//     glEnable(GL_CULL_FACE); // nur aussen seiten füllen
    
    
    cyl_radius = 0.05;
    cyl_height = 0.40;

    qglColor(Qt::blue);
    q = gluNewQuadric();
}


void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -10.0);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

    // zylinder zeichnen
    gluCylinder(q, (GLdouble)cyl_radius, (GLdouble)cyl_radius, (GLdouble)cyl_height, 32, 32);
}


void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}


void GLWidget::mouseMoveEvent(QMouseEvent *event)
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


void GLWidget::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}
