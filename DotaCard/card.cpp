#include "card.h"
#include <QPainter>
#include <QMetaObject>

Card::Card(int value)
    : pixmap(":/png/png/NULL.jpg")
{
    ISDN = value;
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    switch (ISDN) {
    case 601:
        name = "dota-CentaurWarrunner";
        break;
    case 602:
        name = "dota-KeeperoftheLight";
        break;
    case 603:
        name = "dota-Lion";
        break;
    case 604:
        name = "dota-Magnus";
        break;
    case 605:
        name = "dota-NyxAssassin";
        break;
    case 606:
        name = "dota-Rubick";
        break;
    case 607:
        name = "dota-Tusk";
        break;
    case 608:
        name = "dota-Undying";
        break;
    case 609:
        name = "dota-VengefulSpirit";
        break;
    case 610:
        name = "dota-Zeus";
        break;
    default:
        break;
    }
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
    if(QString(parentObject()->metaObject()->className())!="EnemyHandArea")
    {
        pixmap = QPixmap(getPixmapPath());
    }
    pixmap = pixmap.scaled(100, 145, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter->drawPixmap(0, 0, pixmap);
}

QString Card::getPixmapPath()
{
    return QString(":/pic/monster/%1.jpg").arg(name);
}

void Card::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    if(QString(parentObject()->metaObject()->className())=="HandArea")
    {
        setY(-35);
    }
    else if(QString(parentObject()->metaObject()->className())=="EnemyHandArea")
    {
        setY(35);
    }
    emit hover(getPixmapPath());
}

void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    QString area(parentObject()->metaObject()->className());
    if(area=="HandArea" || area=="EnemyHandArea")
    {
        setY(0);
    }
}
