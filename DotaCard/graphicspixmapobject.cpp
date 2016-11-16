#include "graphicspixmapobject.h"
#include "rule.h"
#include <QDebug>

GraphicsPixmapObject::GraphicsPixmapObject()
{
    canMove = false; // roomScene是否可以通过mouseMoveEvent移动宝剑指向,keyi
    setTransformOriginPoint(25, 36);
    setTransformationMode(Qt::SmoothTransformation);
}
