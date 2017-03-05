#include "graphicspixmapobject.h"
#include "rule.h"
#include <QDebug>

GraphicsPixmapObject::GraphicsPixmapObject()
{
//    canMove = false; // roomScene是否可以通过mouseMoveEvent移动宝剑指向,keyi
    setTransformOriginPoint(25, 36);
    setTransformationMode(Qt::SmoothTransformation);
    z = 1;
}

void GraphicsPixmapObject::timerEvent(QTimerEvent *)
{
    ++z;
    setPixmap(QPixmap(QString(":/png/phase_name/e_ba_b%1.png").arg(z)));
    update();

    if(z==8)
    {
        z = 1;
        killTimer(id);
        setVisible(false);
    }
}

void GraphicsPixmapObject::effect()
{
    id = startTimer(100);
}
