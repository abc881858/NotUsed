#include "graphicspixmapobject.h"
#include <QDebug>

GraphicsPixmapObject::GraphicsPixmapObject()
{
    canClick = false;
    canMove = false; // roomScene是否可以通过mouseMoveEvent移动宝剑指向
    setTransformOriginPoint(25,36);
    setTransformationMode(Qt::SmoothTransformation);
}

void GraphicsPixmapObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "GraphicsPixmapObject::mousePressEvent";
    if(event->button() & Qt::LeftButton)
    {
        if (canClick)
        {
            emit doMove();
        }
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}
