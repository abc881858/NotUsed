#include "card.h"
#include <QPainter>
#include <QMetaObject>
#include <QGraphicsSceneMouseEvent>

Card::Card()
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    pixmap = QString(":/png/png/NULL.jpg");
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
    if (area == "DeckArea" || area == "EnemyDeckArea")
    {
        return;
    }
    if (area != "EnemyHandArea") {
        pixmap = QPixmap(image);
    }
    pixmap = pixmap.scaled(100, 145, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter->drawPixmap(0, 0, pixmap);
}

void Card::hoverEnterEvent(QGraphicsSceneHoverEvent*)
{
    if (area == "HandArea") {
        setY(-35);
    }
    else if (area == "EnemyHandArea") {
        setY(35);
    }
    emit hover(name);
}

void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
{
    if (area == "HandArea" || area == "EnemyHandArea") {
        setY(0);
    }
}

bool Card::getStand() const
{
    return stand;
}

void Card::setStand(bool value)
{
    stand = value;
}

bool Card::getFace() const
{
    return face;
}

void Card::setFace(bool value)
{
    face = value;
}

QString Card::getDescription() const
{
    return description;
}

void Card::setDescription(const QString &value)
{
    description = value;
}

QString Card::getArea() const
{
    return area;
}

void Card::setArea(const QString &value)
{
    area = value;
}

QString Card::getImage() const
{
    return image;
}

void Card::setImage(const QString& value)
{
    image = value;
}

QString Card::getName() const
{
    return name;
}

void Card::setName(const QString& value)
{
    name = value;
}

void Card::setISDN(int value)
{
    ISDN = value;
}

///////////////////////////////////////////////////////////

CentaurWarrunner::CentaurWarrunner()//半人马酋长
{
    setISDN(601);
    setName("dota-CentaurWarrunner");
    setImage(":/pic/monster/dota-CentaurWarrunner.jpg");
}

KeeperoftheLight::KeeperoftheLight()//光之守卫
{
    setISDN(602);
    setName("dota-KeeperoftheLight");
    setImage(":/pic/monster/dota-KeeperoftheLight.jpg");
}

Lion::Lion()//恶魔巫师
{
    setISDN(603);
    setName("dota-Lion");
    setImage(":/pic/monster/dota-Lion.jpg");
}

Magnus::Magnus()//半人猛犸
{
    setISDN(604);
    setName("dota-Magnus");
    setImage(":/pic/monster/dota-Magnus.jpg");
}

NyxAssassin::NyxAssassin()//地穴刺客
{
    setISDN(605);
    setName("dota-NyxAssassin");
    setImage(":/pic/monster/dota-NyxAssassin.jpg");
}

Rubick::Rubick()//大魔导师
{
    setISDN(606);
    setName("dota-Rubick");
    setImage(":/pic/monster/dota-Rubick.jpg");
}

Tusk::Tusk()//巨牙海民
{
    setISDN(607);
    setName("dota-Tusk");
    setImage(":/pic/monster/dota-Tusk.jpg");
}

Undying::Undying()//不朽尸王
{
    setISDN(608);
    setName("dota-Undying");
    setImage(":/pic/monster/dota-Undying.jpg");
}

VengefulSpirit::VengefulSpirit()//复仇之魂
{
    setISDN(609);
    setName("dota-VengefulSpirit");
    setImage(":/pic/monster/dota-VengefulSpirit.jpg");
}

Zeus::Zeus()//奥林匹斯之王
{
    setISDN(610);
    setName("dota-Zeus");
    setImage(":/pic/monster/dota-Zeus.jpg");
}
