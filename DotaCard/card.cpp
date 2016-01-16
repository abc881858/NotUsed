#include "card.h"
#include <QPainter>
#include <QMetaObject>

Card::Card(int value)
    : pixmap(":/png/png/NULL.jpg")
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    ISDN = value;
    name = allnames.value(ISDN);
    area = parentObject()->metaObject()->className();
    image = QString(":/pic/monster/%1.jpg").arg(name);
}

int Card::getISDN() const
{
    return ISDN;
}

QRectF Card::boundingRect() const
{
    return QRectF(0, 0, 100, 145);
}

void Card::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if(area!="EnemyHandArea")
    {
        pixmap = QPixmap(image);
    }
    pixmap = pixmap.scaled(100, 145, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter->drawPixmap(0, 0, pixmap);
}

void Card::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    if(area=="HandArea")
    {
        setY(-35);
    }
    else if(area=="EnemyHandArea")
    {
        setY(35);
    }
    emit hover(image);
}

void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    if(area=="HandArea" || area=="EnemyHandArea")
    {
        setY(0);
    }
}
