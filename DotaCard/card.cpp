#include "card.h"
#include <QPainter>
#include <QMetaObject>
#include <QGraphicsSceneMouseEvent>

Card::Card(int value)
    : pixmap(":/png/png/NULL.jpg")
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    ISDN = value;
//    name = allnames.value(ISDN);
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

QString Card::getImage() const
{
    return image;
}

void Card::setImage(const QString &value)
{
    image = value;
}

QString Card::getName() const
{
    return name;
}

void Card::setName(const QString &value)
{
    name = value;
}

void Card::setISDN(int value)
{
    ISDN = value;
}

//void Card::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    if(event->button()==Qt::LeftButton)
//    {
//        emit left_click();
//    }
//    else if(event->button()==Qt::RightButton)
//    {
//        emit right_click();
//    }
//}

Lion::Lion()
{
    setISDN(601);
    setName("dota-Lion");
    setImage(":/pic/monster/dota-Lion.jpg");
}

void Lion::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
//        emit left_click();
    }
//    else if(event->button()==Qt::RightButton)
//    {
//        emit right_click();
//    }

//    if(area=="FieldyardArea" || area=="FieldgroundArea")
//    {
//        //
//    }
}
