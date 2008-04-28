#include "laserScene.h"


LaserScene::LaserScene(QObject* parent) : QGraphicsScene(parent)
{
}


LaserScene::~LaserScene()
{}


void LaserScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	// only one emit!
	// no other emits while keep the button pressed!
	//mouseEvent->accept();
	emit robotPositionChanged(mouseEvent);
	
	//QGraphicsScene::mousePressEvent(mouseEvent);
}


void LaserScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	if(mouseEvent->buttons() & Qt::LeftButton)
	{
		// Notify the gui about position update
		emit robotPositionChanged(mouseEvent);
	}

	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void LaserScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
/*
	if (m_NewLink != NULL) {
		if(m_NewNodeType == BeamNode) {
			BeamNodeItem* beamNode = dynamic_cast<BeamNodeItem*>(itemAt(mouseEvent->scenePos()));
			if(beamNode!=NULL) {
				ComponentLinkItem* beamLink = new ComponentLinkItem(
						BeamLink,
       						m_NewLinkNode,
						beamNode);
				
				// Connect link to me for position update notifications
				connect(this, SIGNAL(componentPositionChanged()),
					beamLink, SLOT(nodePositionChanged()));
				
				// Connect me to link for general update notifications
				connect(beamLink, SIGNAL(linkModified()),
					this, SLOT(sceneItemChanged()));

				// Add link to scene
				addItem(beamLink);

				// Add link to parent components' inventory
				ComponentItem* startComponent = dynamic_cast<ComponentItem*>(m_NewLinkNode->parentItem());
				ComponentItem* endComponent = dynamic_cast<ComponentItem*>(beamNode->parentItem());
				startComponent->addComponentLinkItem(beamLink);
				endComponent->addComponentLinkItem(beamLink);

				// Configure link
				beamLink->setStartComponentID(startComponent->Component()->InstanceID());
				beamLink->setEndComponentID(endComponent->Component()->InstanceID());

				// Notify others that we got a new link
				emit sceneItemModified();
			}

		}
		else if(m_NewNodeType == ControlNode) {
			ControlNodeItem* controlNode = dynamic_cast<ControlNodeItem*>(itemAt(mouseEvent->scenePos()));
			if(controlNode!=NULL) {
				ComponentLinkItem* controlLink = new ComponentLinkItem(
						ControlLink,
						m_NewLinkNode,
						controlNode);

				// Connect link to me for position update notifications
				connect(this, SIGNAL(componentPositionChanged()),
					controlLink, SLOT(nodePositionChanged()));
				
				// Connect me to link for general update notifications
				connect(controlLink, SIGNAL(linkModified()),
					this, SLOT(sceneItemChanged()));

				// Add link to scene
				addItem(controlLink);

				// Add link to parent components' inventory
				ComponentItem* startComponent = dynamic_cast<ComponentItem*>(m_NewLinkNode->parentItem());
				ComponentItem* endComponent = dynamic_cast<ComponentItem*>(controlNode->parentItem());
				startComponent->addComponentLinkItem(controlLink);
				endComponent->addComponentLinkItem(controlLink);

				// Configure link
				controlLink->setStartComponentID(startComponent->Component()->InstanceID());
				controlLink->setEndComponentID(endComponent->Component()->InstanceID());

				// Notify others that we got a new link
				emit sceneItemModified();
			}
		}
		
		removeItem(m_NewLink);
		delete m_NewLink;
		m_NewLink = NULL;
		m_NewNodeType = None;
	}

*/
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}


void LaserScene::wheelEvent(QGraphicsSceneWheelEvent* wheelEvent)
{
	//wheelEvent->accept();
	emit wheelZoom(wheelEvent);
	
	//QGraphicsScene::wheelEvent(wheelEvent);
}


void LaserScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
/*
	// Verify whether this event originates from a component or a link
	ComponentItem* componentItem = dynamic_cast<ComponentItem*>(itemAt(event->scenePos()));
	ComponentLinkItem* linkItem = dynamic_cast<ComponentLinkItem*>(itemAt(event->scenePos()));
	
	if(componentItem) {
		// Create component context menu
		QMenu componentMenu;
		QAction* rotateAction = componentMenu.addAction("Rotate 45" + QString(QChar(176)));
		rotateAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
		rotateAction->setIcon(QIcon(":/icons/resources/context_rotate.png"));
		componentMenu.addSeparator();
		QAction* deleteAction = componentMenu.addAction("Delete");
		deleteAction->setShortcut(QKeySequence::Delete);
		deleteAction->setIcon(QIcon(":/icons/resources/context_delete.png"));
		QAction* selectedAction = componentMenu.exec(event->screenPos());

		if(selectedAction == rotateAction) {
			// Rotate item (and those others selected) by 45 degrees (CCW)
			foreach(QGraphicsItem* item, selectedItems()) {
				// We need this downcast to use our own rotate method
				if((componentItem = dynamic_cast<ComponentItem*>(item))) {
					componentItem->rotate(45);
				}
			}
			// Notify others we got modified
			emit sceneItemModified();
		}
		else if(selectedAction == deleteAction) {
			// Remove/delete component (and those others selected)
			foreach(QGraphicsItem* item, selectedItems()) {
				removeItem(item);
				delete item;
			}
			// Notify others we got modified
			emit sceneItemModified();
		}
	}
	else if(linkItem) {
		// Create link context menu
		QMenu linkMenu;
		QAction* uniLinkAction = linkMenu.addAction("Unidirectional");
		uniLinkAction->setCheckable(true);
		QAction* biLinkAction = linkMenu.addAction("Bidirectional");
		biLinkAction->setCheckable(true);
		linkMenu.addSeparator();
		QAction* deleteAction = linkMenu.addAction("Delete");
		deleteAction->setIcon(QIcon(":/icons/resources/context_delete.png"));
		deleteAction->setShortcut(QKeySequence::Delete);

		// Configure context menu
		if(linkItem->Directionality() == Unidirectional) {
			uniLinkAction->setChecked(true);
		}
		else {
			biLinkAction->setChecked(true);
		}

		// Show menu and store selected action
		QAction* selectedAction = linkMenu.exec(event->screenPos());

		// Set new mode (or delete link) and notify others we got modified
		if(selectedAction == uniLinkAction) {
			linkItem->setDirectionality (Unidirectional);
			emit sceneItemModified();
		}
		else if(selectedAction == biLinkAction) {
			linkItem->setDirectionality(Bidirectional);
			emit sceneItemModified();
		}
		else if(selectedAction == deleteAction) {
			removeItem(linkItem);
			delete linkItem;
			emit sceneItemModified();
		}
	}

*/
	update();
	event->accept();
}


void LaserScene::keyPressEvent(QKeyEvent* keyEvent)
{
	/*
	if(keyEvent->key() == Qt::Key_R && keyEvent->modifiers() & Qt::ControlModifier) {
		ComponentItem* componentItem;
		// Rotate selected items by 45 degrees (CCW)
		foreach(QGraphicsItem* item, selectedItems()) {
			// We need this downcast to use our own rotate method
			if((componentItem = dynamic_cast<ComponentItem*>(item))) {
				componentItem->rotate(45);
			}
		}
		// Notify others we got modified
		emit sceneItemModified();

		keyEvent->accept();
	}
	else if(keyEvent->matches(QKeySequence::Delete)) {
		// Remove/delete selected components
		foreach(QGraphicsItem* item, selectedItems()) {
			removeItem(item);
			delete item;
		}
		// Notify others we got modified
		emit sceneItemModified();

		keyEvent->accept();
	}
	else {
	*/
		QGraphicsScene::keyPressEvent(keyEvent);
	//}
}
