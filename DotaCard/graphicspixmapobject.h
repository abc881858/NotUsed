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
    void effect();

protected:
    void timerEvent(QTimerEvent*);

private:
    int z;
    int id;

signals:
    void finish();
};

#endif // GRAPHICSPIXMAPOBJECT_H
