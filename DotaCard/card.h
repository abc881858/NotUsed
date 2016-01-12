#ifndef CARD_H
#define CARD_H

#include <QGraphicsObject>

class Card : public QGraphicsObject {
    Q_OBJECT

public:
    explicit Card(int value = -1);
    QString name;
    QString description;
    int getISDN() const;

    QString getPixmapPath();
protected:
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
private:
    int ISDN;
    QPixmap pixmap;

signals:
    void hover(QString);
public slots:
};

#endif // CARD_H
