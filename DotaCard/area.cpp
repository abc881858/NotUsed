#include "area.h"
#include <QPainter>
#include <QDebug>

#include "net.h"

static QRectF DeckAreaRect(0, 0, 50, 72);
static QRectF HandAreaRect(0, 0, 512, 70);
static QRectF FieldyardAreaRect(0, 0, 384, 92);
static QRectF FieldgroundAreaRect(0, 0, 384, 92);
static QRectF GraveyardAreaRect(0, 0, 50, 72);
static QRectF EnemyDeckAreaRect(0, 0, 50, 72);
static QRectF EnemyHandAreaRect(0, 0, 512, 70);
static QRectF EnemyFieldyardAreaRect(0, 0, 384, 92);
static QRectF EnemyFieldgroundAreaRect(0, 0, 384, 92);
static QRectF EnemyGraveyardAreaRect(0, 0, 50, 72);

///////////////////////////////////////////////////////////////
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

    Net::instance()->doAddCard(card->getISDN(), Card::DeckArea, card->getIndex(), false, true);
}

Card* DeckArea::takeCard(int index)
{
    Q_ASSERT(index == 0);
    Card* card = myDeck.takeFirst();
    Net::instance()->doTakeCard(Card::DeckArea, card->getIndex());
    return card;
}

QList<Card*> DeckArea::getMyDeck() const
{
    return myDeck;
}

///////////////////////////////////////////////////////////////
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
    for (int i = 0; i < n; i++)
    {
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

    Net::instance()->doAddCard(card->getISDN(), Card::HandArea, card->getIndex(), true, true); //号、区、位、表、攻
}

Card* HandArea::takeCard(int index)
{
    Card* card = myHand.takeAt(index);
    Net::instance()->doTakeCard(Card::HandArea, card->getIndex());
    adjustCards();
    return card;
}

QList<Card*> HandArea::getMyHand() const
{
    return myHand;
}

///////////////////////////////////////////////////////////////
/**
  * @brief 我方怪兽卡牌摆放区域
  * @author wudongliang
  * @date 2016/9/2
  */

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
    for (Card* card : myFieldyard)
    {
        card->setChangePosition(true);
    }
}

void FieldyardArea::adjustCards()
{
    qDebug() << "FieldyardArea's adjustCards.";
    if (myFieldyard.isEmpty())
        return;
    int n = myFieldyard.size();
    int card_skip = 102;
    for (int i = 0; i < n; i++)
    {
        myFieldyard[i]->setPos(QPointF(card_skip * i, 0));
    }
    //TODO: 这里有问题，不能采用handarea的雷同处理，需要修改
}

void FieldyardArea::addCard(Card* card, bool face, bool stand)
{
    card->setParentItem(this);
    card->setIndex(myFieldyard.size());
    card->setFace(face);
    card->setStand(stand);
    card->setArea(Card::FieldyardArea);
    myFieldyard << card;
    adjustCards();

    Net::instance()->doAddCard(card->getISDN(), Card::FieldyardArea, card->getIndex(), face, stand);
}

Card* FieldyardArea::takeCard(int index)
{
    Card* card = myFieldyard.takeAt(index);
    Net::instance()->doTakeCard(Card::FieldyardArea, card->getIndex());
    return card;
}

QList<Card*> FieldyardArea::getMyFieldyard() const
{
    return myFieldyard;
}

///////////////////////////////////////////////////////////////
/**
  * @brief 我方魔法陷阱卡牌摆放区域
  * @author wudongliang
  * @date 2016/9/2
  */

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

QList<Card*> FieldgroundArea::getMyFieldground() const
{
    return myFieldground;
}

///////////////////////////////////////////////////////////////
/**
  * @brief 我方墓地区域
  * @author wudongliang
  * @date 2016/9/2
  */

GraveyardArea::GraveyardArea()
    : pixmap(":/png/png/temp.png")
{
}

QRectF GraveyardArea::boundingRect() const
{
    return GraveyardAreaRect;
}

void GraveyardArea::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->drawPixmap(0, 0, pixmap);
}

void GraveyardArea::addCard(Card* card)
{
    card->setParentItem(this);
    card->setIndex(myGraveyard.size());
    card->setFace(true);
    card->setStand(true);
    card->setArea(Card::GraveyardArea);
    myGraveyard << card;
    adjustCards();

    Net::instance()->doAddCard(card->getISDN(), Card::GraveyardArea, card->getIndex(), true, true);
}

Card* GraveyardArea::takeCard(int index)
{
    Card* card = myGraveyard.takeAt(index);
    Net::instance()->doTakeCard(Card::GraveyardArea, card->getIndex());
    return card;
}

QList<Card*> GraveyardArea::getMyGraveyard() const
{
    return myGraveyard;
}

void GraveyardArea::adjustCards()
{
    qDebug() << "EnemyGraveyardArea's adjustCards.";
    //    if (myGraveyard.isEmpty())
    //        return;
    //    int n = yourFieldyard.size();
    //    int card_skip = 102;
    //    for (int i = 0; i < n; i++)
    //    {
    //        yourFieldyard[i]->setPos(QPointF(card_skip * (3 - i), 0));
    //    }
}

///////////////////////////////////////////////////////////////
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
}

void EnemyDeckArea::response_addCard(Card* card)
{
    card->setParentItem(this);
    card->setIndex(yourDeck.size());
    card->setFace(false);
    card->setStand(true);
    card->setArea(Card::EnemyDeckArea);
    yourDeck << card;
}

Card* EnemyDeckArea::takeCard(int index)
{
    Q_ASSERT(index == 0);
    Card* card = yourDeck.takeFirst();
    //    Net::instance()->doTakeCard(Card::EnemyDeckArea, card->getIndex());
    return card;
}

Card* EnemyDeckArea::response_takeCard(int index)
{
    Q_UNUSED(index);
    Card* card = yourDeck.takeFirst();
    return card;
}

QList<Card*> EnemyDeckArea::getYourDeck() const
{
    return yourDeck;
}

///////////////////////////////////////////////////////////////
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
    for (int i = 0; i < n; i++)
    {
        yourHand[i]->setZValue(0.1 * i);
        yourHand[i]->setPos(QPointF(card_skip * i, 0));
    }
}

void EnemyHandArea::addCard(Card* card)
{
    card->setParentItem(this);
    card->setIndex(yourHand.size());
    card->setFace(false);
    card->setStand(true);
    card->setArea(Card::EnemyHandArea);
    yourHand << card;
    adjustCards();
}

Card* EnemyHandArea::takeCard(int index)
{
    Card* card = yourHand.takeAt(index);
    adjustCards();
    return card;
}

QList<Card*> EnemyHandArea::getYourHand() const
{
    return yourHand;
}

void EnemyHandArea::response_addCard(Card* card)
{
    card->setParentItem(this);
    card->setIndex(yourHand.size());
    card->setFace(false);
    card->setStand(true);
    card->setArea(Card::EnemyHandArea);
    yourHand << card;
    adjustCards();
}

Card* EnemyHandArea::response_takeCard(int index)
{
    Card* card = yourHand.takeAt(index);
    return card;
}

///////////////////////////////////////////////////////////////
/**
  * @brief 敌方怪兽卡牌摆放区域
  * @author wudongliang
  * @date 2016/9/2
  */

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

void EnemyFieldyardArea::addCard(Card* card, bool face, bool stand)
{
    card->setParentItem(this);
    card->setIndex(yourFieldyard.size());
    card->setFace(face);
    card->setStand(stand);
    card->setArea(Card::EnemyFieldyardArea);
    yourFieldyard << card;
    adjustCards();
}

Card* EnemyFieldyardArea::takeCard(int index)
{
    Card* card = yourFieldyard.takeAt(index);
    return card;
}

QList<Card*> EnemyFieldyardArea::getYourFieldyard() const
{
    return yourFieldyard;
}

void EnemyFieldyardArea::response_addCard(Card* card)
{
    card->setParentItem(this);
    card->setIndex(yourFieldyard.size());
    card->setFace(card->getFace());
    card->setStand(card->getStand());
    card->setArea(Card::EnemyFieldyardArea);
    yourFieldyard << card;
    adjustCards();
}

Card* EnemyFieldyardArea::response_takeCard(int index)
{
    Card* card = yourFieldyard.takeAt(index);
    return card;
}

void EnemyFieldyardArea::adjustCards()
{
    qDebug() << "EnemyFieldyardArea's adjustCards.";
    if (yourFieldyard.isEmpty())
        return;
    int n = yourFieldyard.size();
    int card_skip = 102;
    for (int i = 0; i < n; i++)
    {
        yourFieldyard[i]->setPos(QPointF(card_skip * (3 - i), 0));
    }
    //TODO: 这里有问题，不能采用handarea的雷同处理，需要修改
}

///////////////////////////////////////////////////////////////
/**
  * @brief 敌方魔法陷阱卡牌摆放区域
  * @author wudongliang
  * @date 2016/9/2
  */

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

QList<Card*> EnemyFieldgroundArea::getYourFieldground() const
{
    return yourFieldground;
}

///////////////////////////////////////////////////////////////
/**
  * @brief 敌方墓地区域
  * @author wudongliang
  * @date 2016/9/2
  */

EnemyGraveyardArea::EnemyGraveyardArea()
    : pixmap(":/png/png/temp.png")
{
}

QRectF EnemyGraveyardArea::boundingRect() const
{
    return EnemyGraveyardAreaRect;
}

void EnemyGraveyardArea::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->drawPixmap(0, 0, pixmap);
}

void EnemyGraveyardArea::addCard(Card* card)
{
    card->setParentItem(this);
    card->setIndex(yourGraveyard.size());
    card->setFace(true);
    card->setStand(true);
    card->setArea(Card::EnemyFieldyardArea);
    yourGraveyard << card;
    adjustCards();
}

Card* EnemyGraveyardArea::takeCard(int index)
{
    Card* card = yourGraveyard.takeAt(index);
    return card;
}

QList<Card*> EnemyGraveyardArea::getYourGraveyard() const
{
    return yourGraveyard;
}

void EnemyGraveyardArea::response_addCard(Card* card)
{
    card->setParentItem(this);
    card->setIndex(yourGraveyard.size());
    card->setFace(true);
    card->setStand(true);
    card->setArea(Card::EnemyFieldyardArea);
    yourGraveyard << card;
    adjustCards();
}

Card* EnemyGraveyardArea::response_takeCard(int index)
{
    Card* card = yourGraveyard.takeAt(index);
    return card;
}

void EnemyGraveyardArea::adjustCards()
{
    qDebug() << "EnemyGraveyardArea's adjustCards.";
    //    if (yourFieldyard.isEmpty())
    //        return;
    //    int n = yourFieldyard.size();
    //    int card_skip = 102;
    //    for (int i = 0; i < n; i++)
    //    {
    //        yourFieldyard[i]->setPos(QPointF(card_skip * (3 - i), 0));
    //    }
}
