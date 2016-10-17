#include "graphicspixmapobject.h"
#include <QGraphicsSceneMouseEvent>

GraphicsPixmapObject::GraphicsPixmapObject()
{
    canClick = false;
    setTransformOriginPoint(25,36);
}

void GraphicsPixmapObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() & Qt::LeftButton)
    {
        if (canClick)
        {
            emit canMove();
        }
    }
}
