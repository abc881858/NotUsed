#ifndef AREA_H
#define AREA_H

#include <QGraphicsPixmapItem>

class Card;

class DeckArea : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    static DeckArea* instance();
    void addCard(Card* card);
    Card* takeCard(int index = 0);
    QList<Card*> getMyDeck() const;

private:
    QList<Card*> myDeck;
};

class HandArea : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    static HandArea* instance();
    void adjustCards();
    void addCard(Card* card);
    Card* takeCard(int index);
    QList<Card*> getMyHand() const;

private:
    QList<Card*> myHand;
};

class FieldyardArea : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    static FieldyardArea* instance();
    void initializeCards();
    void adjustCards();
    void addCard(Card* card, bool face = true, bool stand = true);
    Card* takeCard(int index);
    QList<Card*> getMyFieldyard() const;

private:
    QList<Card*> myFieldyard;
};

class FieldgroundArea : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    static FieldgroundArea* instance();
    QList<Card*> getMyFieldground() const;

private:
    QList<Card*> myFieldground;
};

class GraveyardArea : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    static GraveyardArea* instance();
    void adjustCards();
    void addCard(Card* card);
    Card* takeCard(int index);
    QList<Card*> getMyGraveyard() const;

private:
    QList<Card*> myGraveyard;
};

class EnemyDeckArea : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    static EnemyDeckArea* instance();
    QList<Card*> getYourDeck() const;
    void response_addCard(Card* card);
    Card* response_takeCard(int index = 0);

private:
    QList<Card*> yourDeck;
};

class EnemyHandArea : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    static EnemyHandArea* instance();
    void adjustCards();
    QList<Card*> getYourHand() const;
    void response_addCard(Card* card);
    Card* response_takeCard(int index);

private:
    QList<Card*> yourHand;
};

class EnemyFieldyardArea : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    static EnemyFieldyardArea* instance();
    void adjustCards();
    QList<Card*> getYourFieldyard() const;
    void response_addCard(Card* card, bool face = true, bool stand = true);
    Card* response_takeCard(int index);

private:
    QList<Card*> yourFieldyard;
};

class EnemyFieldgroundArea : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    static EnemyFieldgroundArea* instance();
    QList<Card*> getYourFieldground() const;
    //    void response_addCard(Card* card);
    //    Card* response_takeCard(int index);
private:
    QList<Card*> yourFieldground;
};

class EnemyGraveyardArea : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    static EnemyGraveyardArea* instance();
    void adjustCards();
    QList<Card*> getYourGraveyard() const;
    void response_addCard(Card* card);
    Card* response_takeCard(int index);

private:
    QList<Card*> yourGraveyard;
};

#endif // AREA_H
