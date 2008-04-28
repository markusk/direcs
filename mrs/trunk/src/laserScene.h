#ifndef LASERSCENE_H
#define LASERSCENE_H

#include <QtGui>


/**
This specialised version of QGraphicsScene is able to react to different mouse events in order to provide direct user interaction.
*/
class LaserScene : public QGraphicsScene
{
	Q_OBJECT

	public:
		LaserScene(QObject* parent = 0);
		~LaserScene();

	signals:
		/**
		This signal is emitted as soon as the robot's position changed.
		The position change is triggered by the mouse move event handler.
		*/
		void robotPositionChanged(QGraphicsSceneMouseEvent* mouseEvent);
	
	
	protected:
		/**
		Reimplementation of QGraphicsView::mousePressEvent().
		*/
		void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);
	
		/**
		Reimplementation of QGraphicsView::mouseMoveEvent().
		*/
		void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent);
	
		/**
		Reimplementation of QGraphicsScene::mouseReleaseEvent().
		*/
		void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent);
	
		/**
		Reimplementation of QGraphicsScene::contextMenuEvent().
		*/
		void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
	
		/**
		Reimplementation of QGraphicsScene::keyPressEvent().
		*/
		void keyPressEvent(QKeyEvent* keyEvent);
};

#endif
