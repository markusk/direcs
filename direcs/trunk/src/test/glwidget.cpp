#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"


GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    
    // initialize quadric pointers
    xAxis = NULL;
    yAxis = NULL;
    zAxis = NULL;
    xAxisCone = NULL;
    yAxisCone = NULL;
    zAxisCone = NULL;
}


GLWidget::~GLWidget()
{
    makeCurrent();
      
    if (zAxisCone)
      gluDeleteQuadric (zAxisCone);
      
    if (yAxisCone)
      gluDeleteQuadric (yAxisCone);
      
    if (xAxisCone)
      gluDeleteQuadric (xAxisCone);

  if (zAxis)
      gluDeleteQuadric (zAxis);
      
    if (yAxis)
      gluDeleteQuadric (yAxis);
      
    if (xAxis)
      gluDeleteQuadric (xAxis);
}


void GLWidget::initializeGL()
{
    xAxisColor = Qt::red;
    yAxisColor = Qt::green;
    zAxisColor = Qt::blue;
    backgroundColor = Qt::black;


    qglClearColor(backgroundColor.dark());
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    
    glEnable(GL_DEPTH_TEST);

/*
    GLfloat LightAmbient[] = {0.3, 0.3, 0.3, 1.0};
    GLfloat LightDiffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat LightSpecular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat LightPosition[] = {0.0, 0.0, 3.0, 1.0};
    GLfloat spot_direction[] = {0.0, 0.0, -1.0};
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 50.0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0);
    
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
*/  

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);


    cyl_radius = 0.03;
    cyl_height = 0.30;

    xAxis = gluNewQuadric();
    yAxis = gluNewQuadric();
    zAxis = gluNewQuadric();

    xAxisCone = gluNewQuadric();
    yAxisCone = gluNewQuadric();
    zAxisCone = gluNewQuadric();

    gluQuadricNormals(xAxis, GLU_SMOOTH);
    gluQuadricNormals(yAxis, GLU_SMOOTH);
    gluQuadricNormals(zAxis, GLU_SMOOTH);

    gluQuadricNormals(xAxisCone, GLU_SMOOTH);
    gluQuadricNormals(yAxisCone, GLU_SMOOTH);
    gluQuadricNormals(zAxisCone, GLU_SMOOTH);
}


void GLWidget::paintGL()
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
    gluCylinder(xAxis, cyl_radius, cyl_radius, cyl_height, 32, 32);
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
    gluCylinder(yAxis, cyl_radius, cyl_radius, cyl_height, 32, 32);
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
    gluCylinder(zAxis, cyl_radius, cyl_radius, cyl_height, 32, 32);
    // move
    glTranslatef(0.0, 0.0, cyl_height);
    qglColor(zAxisColor);
    // Z cone
    gluCylinder(zAxisCone, (cyl_radius*1.5), 0.0, cyl_height/2.0, 32, 32);
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


QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}


QSize GLWidget::sizeHint() const
{
    return QSize(800, 800);
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
