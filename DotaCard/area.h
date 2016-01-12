#ifndef AREA_H
#define AREA_H

#include <QGraphicsObject>
#include "card.h"

class DeckArea : public QGraphicsObject {
    Q_OBJECT
public:
    DeckArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    QPixmap pixmap;
    QList<Card*> myDeck;
};

class HandArea : public QGraphicsObject {
    Q_OBJECT
public:
    HandArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    QPixmap pixmap;
    QList<Card*> myHand;
    void adjustCards();
    void addCard(Card* card);
//    Card* takeCard(int ISDN);
};

class FieldyardArea : public QGraphicsObject {
    Q_OBJECT
public:
    FieldyardArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    QPixmap pixmap;
    QList<Card*> myFieldyard;
};

class FieldgroundArea : public QGraphicsObject {
    Q_OBJECT
public:
    FieldgroundArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    QPixmap pixmap;
    QList<Card*> myFieldground;
};

class EnemyDeckArea : public QGraphicsObject {
    Q_OBJECT
public:
    EnemyDeckArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    QPixmap pixmap;
    QList<Card*> yourDeck;
};

class EnemyHandArea : public QGraphicsObject {
    Q_OBJECT
public:
    EnemyHandArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    QPixmap pixmap;
    QList<Card*> yourHand;
    void adjustCards();
    void addCard(Card* card);
//    Card* takeCard(int ISDN);
};

class EnemyFieldyardArea : public QGraphicsObject {
    Q_OBJECT
public:
    EnemyFieldyardArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    QPixmap pixmap;
    QList<Card*> yourFieldyard;
};

class EnemyFieldgroundArea : public QGraphicsObject {
    Q_OBJECT
public:
    EnemyFieldgroundArea();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    QPixmap pixmap;
    QList<Card*> yourFieldground;
};

#endif // AREA_H
