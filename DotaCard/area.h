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
    QList<Card*> getMyFieldyard() const;
    void addCard(Card* card, bool face = true, bool stand = true);
    Card* takeCard(int place);

    void initializeCards();
    void initializePlace();
    int testAddCard();

    struct Place
    {
        bool canPlace; //该位置是否可用
        int at; //来自List的哪里
        QPointF pos; //myFieldyard[i]->setPos
    } one, two, three, four, five;

private:
    QList<Card*> myFieldyard;

signals:
    void showWord(int);
    void hideWord(int);
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
    QList<Card*> getYourFieldyard() const;
    void initializePlace();
    void response_addCard(Card* card, int place, bool face = true, bool stand = true);
    Card* response_takeCard(int index);

    struct Place
    {
        bool canPlace;
        int at;
        QPointF pos; //从右往左排
    } one, two, three, four, five;

private:
    QList<Card*> yourFieldyard;

signals:
    void showWord(int);
    void hideWord(int);
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
