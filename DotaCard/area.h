#ifndef AREA_H
#define AREA_H

#include <QGraphicsObject>

#include "card.h"

class DeckArea : public QGraphicsObject
{
    Q_OBJECT
public:
    DeckArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    void addCard(Card* card);
    Card* takeCard(int index = 0);
    QList<Card*> getMyDeck() const;

private:
    QPixmap pixmap;
    QList<Card*> myDeck;
};

class HandArea : public QGraphicsObject
{
    Q_OBJECT
public:
    HandArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    void adjustCards();
    void addCard(Card* card);
    Card* takeCard(int index);
    QList<Card*> getMyHand() const;

private:
    QPixmap pixmap;
    QList<Card*> myHand;
};

class FieldyardArea : public QGraphicsObject
{
    Q_OBJECT
public:
    FieldyardArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    //    void refreshCardFlags();
    void initializeCards();
    void adjustCards();
    void addCard(Card* card, bool face = true, bool stand = true);
    Card* takeCard(int index);
    QList<Card*> getMyFieldyard() const;

private:
    QPixmap pixmap;
    QList<Card*> myFieldyard;
};

class FieldgroundArea : public QGraphicsObject
{
    Q_OBJECT
public:
    FieldgroundArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    QList<Card*> getMyFieldground() const;

private:
    QPixmap pixmap;
    QList<Card*> myFieldground;
};

class GraveyardArea : public QGraphicsObject
{
    Q_OBJECT
public:
    GraveyardArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    void adjustCards();
    void addCard(Card* card);
    Card* takeCard(int index);
    QList<Card*> getMyGraveyard() const;

private:
    QPixmap pixmap;
    QList<Card*> myGraveyard;
};

class EnemyDeckArea : public QGraphicsObject
{
    Q_OBJECT
public:
    EnemyDeckArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    void addCard(Card* card);
    Card* takeCard(int index = 0);
    QList<Card*> getYourDeck() const;

    void response_addCard(Card* card);
    Card* response_takeCard(int index);

private:
    QPixmap pixmap;
    QList<Card*> yourDeck;
};

class EnemyHandArea : public QGraphicsObject
{
    Q_OBJECT
public:
    EnemyHandArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    void adjustCards();
    void addCard(Card* card);
    Card* takeCard(int index);
    QList<Card*> getYourHand() const;

    void response_addCard(Card* card);
    Card* response_takeCard(int index);

private:
    QPixmap pixmap;
    QList<Card*> yourHand;
};

class EnemyFieldyardArea : public QGraphicsObject
{
    Q_OBJECT
public:
    EnemyFieldyardArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    void adjustCards();
    void addCard(Card* card, bool face = true, bool stand = true);
    Card* takeCard(int index);
    QList<Card*> getYourFieldyard() const;

    void response_addCard(Card* card, bool face = true, bool stand = true);
    Card* response_takeCard(int index);

private:
    QPixmap pixmap;
    QList<Card*> yourFieldyard;
};

class EnemyFieldgroundArea : public QGraphicsObject
{
    Q_OBJECT
public:
    EnemyFieldgroundArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    QList<Card*> getYourFieldground() const;

    //    void response_addCard(Card* card);
    //    Card* response_takeCard(int index);

private:
    QPixmap pixmap;
    QList<Card*> yourFieldground;
};

class EnemyGraveyardArea : public QGraphicsObject
{
    Q_OBJECT
public:
    EnemyGraveyardArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    void adjustCards();
    void addCard(Card* card);
    Card* takeCard(int index);
    QList<Card*> getYourGraveyard() const;

    void response_addCard(Card* card);
    Card* response_takeCard(int index);

private:
    QPixmap pixmap;
    QList<Card*> yourGraveyard;
};

#endif // AREA_H
