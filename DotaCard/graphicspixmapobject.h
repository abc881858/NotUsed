#ifndef GRAPHICSPIXMAPOBJECT_H
#define GRAPHICSPIXMAPOBJECT_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

//Scene中所有可以动的Item都用这个类
class GraphicsPixmapObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    GraphicsPixmapObject();
};

#endif // GRAPHICSPIXMAPOBJECT_H
