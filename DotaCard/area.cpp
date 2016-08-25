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

/**
  * @brief 我方卡组区域
  * @author wudongliang
  * @date 2016/4/22
  */

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

void DeckArea::addCard(Card* card)
{
    card->setParentItem(this);
    card->setIndex(myDeck.size());
    card->setFace(false);
    card->setStand(true);
    card->setArea(Card::DeckArea);

    myDeck << card;
    //    adjustCards();
}

Card* DeckArea::takeFirstCard()
{
    return myDeck.takeFirst();
}

/**
  * @brief 我方手牌区域
  * @author wudongliang
  * @date 2016/4/22
  */

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
    card->setParentItem(this);
    card->setIndex(myHand.size());
    card->setFace(true);
    card->setStand(true);
    card->setArea(Card::HandArea);
    myHand << card;
    adjustCards();
}

Card* HandArea::takeCard(int index)
{
    //0 <= index < size()
    return myHand.takeAt(index);
}

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

void FieldyardArea::initializeCards()
{
    foreach (Card* card, myFieldyard) {
        card->setChangePosition(true);
    }
}

void FieldyardArea::addCard(Card* card, bool face, bool stand)
{
    card->setParentItem(this);
    card->setIndex(myFieldyard.size());
    card->setFace(face);
    card->setStand(stand);
    card->setArea(Card::HandArea);
    myFieldyard << card;
    adjustCards();
}

void FieldyardArea::adjustCards()
{
    if (myFieldyard.isEmpty())
        return;
//    int n = myFieldyard.size();
//    int card_skip = (n > 5) ? (412 / (n - 1)) : 102;
//    for (int i = 0; i < n; i++) {
//        myFieldyard[i]->setZValue(0.1 * i);
//        myFieldyard[i]->setPos(QPointF(card_skip * i, 0));
//    }
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

/**
  * @brief 敌方卡组区域
  * @author wudongliang
  * @date 2016/4/22
  */

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

void EnemyDeckArea::addCard(Card* card)
{
    card->setParentItem(this);
    card->setIndex(yourDeck.size());
    card->setFace(false);
    card->setStand(true);
    card->setArea(Card::EnemyDeckArea);

    yourDeck << card;
    //    adjustCards();
}

Card* EnemyDeckArea::takeFirstCard()
{
    return yourDeck.takeFirst();
}

/**
  * @brief 敌方手牌区域
  * @author wudongliang
  * @date 2016/4/22
  */

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
    card->setParentItem(this);
    card->setIndex(yourHand.size());
    card->setStand(true);
    card->setFace(false);
    card->setArea(Card::EnemyHandArea);

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
