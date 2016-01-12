#include "area.h"
#include <QPainter>

static QRectF DeckAreaRect(0, 0, 50, 72);
static QRectF HandAreaRect(0, 0, 512, 70);
static QRectF FieldyardAreaRect(0, 0, 384, 92);
static QRectF FieldgroundAreaRect(0, 0, 384, 92);
static QRectF EnemyDeckAreaRect(0, 0, 50, 72);
static QRectF EnemyHandAreaRect(0, 0, 512, 70);
static QRectF EnemyFieldyardAreaRect(0, 0, 384, 92);
static QRectF EnemyFieldgroundAreaRect(0, 0, 384, 92);

DeckArea::DeckArea()
    : pixmap(":/png/png/temp.png")
{
}

QRectF DeckArea::boundingRect() const
{
    return DeckAreaRect;
}

void DeckArea::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->drawPixmap(0, 0, pixmap);
}

///////////////////////////////////////////////////////////////

HandArea::HandArea()
    : pixmap(":/png/png/hand.png")
{
}

QRectF HandArea::boundingRect() const
{
    return HandAreaRect;
}

void HandArea::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->drawPixmap(0, 0, pixmap);
}

void HandArea::adjustCards()
{
    if (myHand.isEmpty())
        return;
    int n = myHand.size();
    int card_skip = (n > 5) ? (412 / (n - 1)) : 102;
    for (int i = 0; i < n; i++) {
        myHand[i]->setZValue(0.1 * i);
        myHand[i]->setPos(QPointF(card_skip * i, 0));
    }
}

void HandArea::addCard(Card* card)
{
    //    card->setFace(true);
    //    card->setArea(CardItem::HandArea);
    card->setParentItem(this);
    //    card->setFlags(ItemIsFocusable);
    myHand << card;
    adjustCards();
}

//Card* HandArea::takeCard(int ISDN)
//{
//}

///////////////////////////////////////////////////////////////

FieldyardArea::FieldyardArea()
    : pixmap(":/png/png/field.png")
{
}

QRectF FieldyardArea::boundingRect() const
{
    return FieldyardAreaRect;
}

void FieldyardArea::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->drawPixmap(0, 0, pixmap);
}

///////////////////////////////////////////////////////////////

FieldgroundArea::FieldgroundArea()
    : pixmap(":/png/png/field.png")
{
}

QRectF FieldgroundArea::boundingRect() const
{
    return FieldgroundAreaRect;
}

void FieldgroundArea::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->drawPixmap(0, 0, pixmap);
}

///////////////////////////////////////////////////////////////

EnemyDeckArea::EnemyDeckArea()
    : pixmap(":/png/png/temp.png")
{
}

QRectF EnemyDeckArea::boundingRect() const
{
    return EnemyDeckAreaRect;
}

void EnemyDeckArea::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->drawPixmap(0, 0, pixmap);
}

///////////////////////////////////////////////////////////////

EnemyHandArea::EnemyHandArea()
    : pixmap(":/png/png/hand.png")
{
}

QRectF EnemyHandArea::boundingRect() const
{
    return EnemyHandAreaRect;
}

void EnemyHandArea::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->drawPixmap(0, 0, pixmap);
}

void EnemyHandArea::adjustCards()
{
    if (yourHand.isEmpty())
        return;
    int n = yourHand.size();
    int card_skip = (n > 5) ? (412 / (n - 1)) : 102;
    for (int i = 0; i < n; i++) {
        yourHand[i]->setZValue(0.1 * i);
        yourHand[i]->setPos(QPointF(card_skip * i, 0));
    }
}

void EnemyHandArea::addCard(Card* card)
{
    //    card->setFace(false);
    //    card->setArea(CardItem::EnemyHandArea);
    card->setParentItem(this);
    //    card_item->setFlags(ItemIsFocusable);
    yourHand << card;
    adjustCards();
}

//Card* EnemyHandArea::takeCard(int ISDN)
//{
//}

///////////////////////////////////////////////////////////////

EnemyFieldyardArea::EnemyFieldyardArea()
    : pixmap(":/png/png/field.png")
{
}

QRectF EnemyFieldyardArea::boundingRect() const
{
    return EnemyFieldyardAreaRect;
}

void EnemyFieldyardArea::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->drawPixmap(0, 0, pixmap);
}

///////////////////////////////////////////////////////////////

EnemyFieldgroundArea::EnemyFieldgroundArea()
    : pixmap(":/png/png/field.png")
{
}

QRectF EnemyFieldgroundArea::boundingRect() const
{
    return EnemyFieldgroundAreaRect;
}

void EnemyFieldgroundArea::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->drawPixmap(0, 0, pixmap);
}
