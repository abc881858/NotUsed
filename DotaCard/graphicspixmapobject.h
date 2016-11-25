#ifndef GRAPHICSPIXMAPOBJECT_H
#define GRAPHICSPIXMAPOBJECT_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>

//Scene中所有可以动的Item都用这个类
class GraphicsPixmapObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    GraphicsPixmapObject();
};

//class GraphicsTextObject : public QGraphicsTextItem
//{
//    Q_OBJECT
//public:
//    GraphicsTextObject();
//};

#endif // GRAPHICSPIXMAPOBJECT_H
