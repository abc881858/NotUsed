#include "graphicspixmapobject.h"

GraphicsPixmapObject::GraphicsPixmapObject()
{
    canClick = false;
    setTransformOriginPoint(25,36);
    setTransformationMode(Qt::SmoothTransformation);
}

void GraphicsPixmapObject::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    if(canClick)
    {
        emit canMove();
    }
}
