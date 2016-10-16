#include "area.h"

#include <QDebug>

#include "net.h"
#include "card.h"

Q_GLOBAL_STATIC(DeckArea, deckArea)
Q_GLOBAL_STATIC(HandArea, handArea)
Q_GLOBAL_STATIC(FieldyardArea, fieldyardArea)
Q_GLOBAL_STATIC(FieldgroundArea, fieldgroundArea)
Q_GLOBAL_STATIC(GraveyardArea, graveyardArea)
Q_GLOBAL_STATIC(EnemyDeckArea, enemyDeckArea)
Q_GLOBAL_STATIC(EnemyHandArea, enemyHandArea)
Q_GLOBAL_STATIC(EnemyFieldyardArea, enemyFieldyardArea)
Q_GLOBAL_STATIC(EnemyFieldgroundArea, enemyFieldgroundArea)
Q_GLOBAL_STATIC(EnemyGraveyardArea, enemyGraveyardArea)

DeckArea* DeckArea::instance()
{
    return deckArea();
}

HandArea* HandArea::instance()
{
    return handArea();
}

FieldyardArea* FieldyardArea::instance()
{
    return fieldyardArea();
}

FieldgroundArea* FieldgroundArea::instance()
{
    return fieldgroundArea();
}

GraveyardArea* GraveyardArea::instance()
{
    return graveyardArea();
}

EnemyDeckArea* EnemyDeckArea::instance()
{
    return enemyDeckArea();
}

EnemyHandArea* EnemyHandArea::instance()
{
    return enemyHandArea();
}

EnemyFieldyardArea* EnemyFieldyardArea::instance()
{
    return enemyFieldyardArea();
}

EnemyFieldgroundArea* EnemyFieldgroundArea::instance()
{
    return enemyFieldgroundArea();
}

EnemyGraveyardArea* EnemyGraveyardArea::instance()
{
    return enemyGraveyardArea();
}

///////////////////////////////////////////////////////////////
/**
  * @brief 我方卡组区域
  * @author wudongliang
  * @date 2016/4/22
  */

void DeckArea::addCard(Card* card)
{
    card->setParentItem(this);
    card->setIndex(myDeck.size());
    card->setFace(false);
    card->setArea(Deck_Area);
    card->setStand(true);
    myDeck << card;
    Net::instance()->doAddCard(card->getISDN(), Deck_Area, card->getIndex(), false, true);
}

Card* DeckArea::takeCard(int index)
{
    Q_ASSERT(index == 0);
    Card* card = myDeck.takeFirst();
    Net::instance()->doTakeCard(Deck_Area, card->getIndex());
    for(Card* item: myDeck)
    {
        item->setIndex(item->getIndex()-1);
    }
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

void HandArea::adjustCards() //TODO 非抽卡阶段addCard 需要flush
{
    if (myHand.isEmpty())
        return;
    int n = myHand.size();
    int card_skip = (n > 5) ? (412 / (n - 1)) : 102;
    for (int i = 0; i < n; i++)
    {
        myHand[i]->setZValue(0.1 * i);
        myHand[i]->setPos(QPointF(card_skip * i, 0));
        myHand[i]->setIndex(i);
    }
}

void HandArea::addCard(Card* card)
{
    card->setParentItem(this);
    card->setFace(true);
    card->setArea(Hand_Area);
    card->setStand(true);
    myHand << card;
    adjustCards();

    Net::instance()->doAddCard(card->getISDN(), Hand_Area, card->getIndex(), true, true); //号、区、位、表、攻
}

Card* HandArea::takeCard(int index)
{
    Card* card = myHand.takeAt(index);
    Net::instance()->doTakeCard(Hand_Area, card->getIndex());
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
    int card_skip = 80;
    for (int i = 0; i < n; i++)
    {
        myFieldyard[i]->setPos(QPointF(card_skip * i, 0));
        myFieldyard[i]->setIndex(i);
    }
    //TODO: 这里有问题，不能采用handarea的雷同处理，需要修改
}

void FieldyardArea::addCard(Card* card, bool face, bool stand)
{
    card->setParentItem(this);
    card->setFace(face);
    card->setArea(Fieldyard_Area);
    card->setStand(stand);
    myFieldyard << card;
    adjustCards();

    Net::instance()->doAddCard(card->getISDN(), Fieldyard_Area, card->getIndex(), face, stand);
}

Card* FieldyardArea::takeCard(int index)
{
    qDebug() << "FieldyardArea::takeCard index: " << index;
    Card* card = myFieldyard.takeAt(index);
    Net::instance()->doTakeCard(Fieldyard_Area, card->getIndex());
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

void GraveyardArea::addCard(Card* card)
{
    qDebug() << "GraveyardArea::addCard";
    card->setParentItem(this);
    card->setFace(true);
    card->setArea(Graveyard_Area);
    card->setStand(true);
    myGraveyard << card;
    adjustCards();

    Net::instance()->doAddCard(card->getISDN(), Graveyard_Area, card->getIndex(), true, true);
}

Card* GraveyardArea::takeCard(int index)
{
    Card* card = myGraveyard.takeAt(index);
    Net::instance()->doTakeCard(Graveyard_Area, card->getIndex());
    return card;
}

QList<Card*> GraveyardArea::getMyGraveyard() const
{
    return myGraveyard;
}

void GraveyardArea::adjustCards()
{
    qDebug() << "EnemyGraveyardArea's adjustCards.";
    if (myGraveyard.isEmpty())
        return;
    int n = myGraveyard.size();
    for (int i = 0; i < n; i++)
    {
        myGraveyard[i]->setPos(0, 0);
        myGraveyard[i]->setIndex(i);
    }
}

///////////////////////////////////////////////////////////////
/**
  * @brief 敌方卡组区域
  * @author wudongliang
  * @date 2016/4/22
  */

void EnemyDeckArea::response_addCard(Card* card)
{
    card->setParentItem(this);
    card->setIndex(yourDeck.size());
    card->setFace(false);
    card->setStand(true);
    card->setArea(EnemyDeck_Area);
    yourDeck << card;
}

Card* EnemyDeckArea::response_takeCard(int index)
{
    Q_UNUSED(index);
    Card* card = yourDeck.takeFirst();
    int n = yourDeck.size();
    for (int i = 0; i < n; i++)
    {
        yourDeck[i]->setIndex(i);
    }
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

void EnemyHandArea::adjustCards() //修改为从右往左排
{
    if (yourHand.isEmpty())
        return;
    int n = yourHand.size();
    int card_skip = (n > 5) ? (412 / (n - 1)) : 102;
    for (int i = 0; i < n; i++)
    {
        yourHand[i]->setZValue(0.1 * i);
        yourHand[i]->setPos(QPointF(card_skip * i, 0));
        yourHand[i]->setIndex(i);
    }
}

QList<Card*> EnemyHandArea::getYourHand() const
{
    return yourHand;
}

void EnemyHandArea::response_addCard(Card* card) //TODO 非抽卡阶段addCard 需要flush
{
    card->setParentItem(this);
    card->setFace(false);
    card->setArea(EnemyHand_Area);
    card->setStand(true);
    yourHand << card;
    adjustCards();
}

Card* EnemyHandArea::response_takeCard(int index)
{
    Card* card = yourHand.takeAt(index);
    adjustCards();
    return card;
}

///////////////////////////////////////////////////////////////
/**
  * @brief 敌方怪兽卡牌摆放区域
  * @author wudongliang
  * @date 2016/9/2
  */

QList<Card*> EnemyFieldyardArea::getYourFieldyard() const
{
    return yourFieldyard;
}

void EnemyFieldyardArea::response_addCard(Card* card, bool face, bool stand)
{
    card->setParentItem(this);
    card->setFace(face);
    card->setArea(EnemyFieldyard_Area);
    card->setStand(stand);
    yourFieldyard << card;
    adjustCards();
}

Card* EnemyFieldyardArea::response_takeCard(int index)
{
    qDebug() << "EnemyFieldyardArea::response_takeCard index: " << index;
    Card* card = yourFieldyard.takeAt(index);
    adjustCards();
    return card;
}

void EnemyFieldyardArea::adjustCards()
{
    qDebug() << "EnemyFieldyardArea's adjustCards.";
    if (yourFieldyard.isEmpty())
        return;
    int n = yourFieldyard.size();
    int card_skip = 80;
    for (int i = 0; i < n; i++)
    {
        yourFieldyard[i]->setPos(QPointF(card_skip * (4 - i), 0));
        yourFieldyard[i]->setIndex(i);
    }
    //TODO: 这里有问题，不能采用handarea的雷同处理，需要修改
}

///////////////////////////////////////////////////////////////
/**
  * @brief 敌方魔法陷阱卡牌摆放区域
  * @author wudongliang
  * @date 2016/9/2
  */

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

QList<Card*> EnemyGraveyardArea::getYourGraveyard() const
{
    return yourGraveyard;
}

void EnemyGraveyardArea::response_addCard(Card* card)
{
    qDebug() << "EnemyGraveyardArea::response_addCard";
    card->setParentItem(this);
    card->setFace(true);
    card->setArea(EnemyGraveyard_Area);
    card->setStand(true);
    yourGraveyard << card;
    adjustCards();
}

Card* EnemyGraveyardArea::response_takeCard(int index)
{
    Card* card = yourGraveyard.takeAt(index);
    adjustCards();
    return card;
}

void EnemyGraveyardArea::adjustCards()
{
    qDebug() << "EnemyGraveyardArea's adjustCards.";
    if (yourGraveyard.isEmpty())
        return;
    int n = yourGraveyard.size();
    for (int i = 0; i < n; i++)
    {
        yourGraveyard[i]->setPos(0, 0);
        yourGraveyard[i]->setIndex(i);
    }
}
