#ifndef CARD_H
#define CARD_H

#include <QGraphicsObject>
#include <QHash>
#include <QMouseEvent>
#include <QCursor>

class Card : public QGraphicsObject {
    Q_OBJECT
public:
    explicit Card();

    enum CardFlag {
        Effect = 0x1, //可以发动怪兽效果
        FlipSummon = 0x2, //可以翻转召唤
        DefencePosition = 0x4, //可以防守表示
        AttackPosition = 0x8, //可以攻击表示
        NormalSummon = 0x10,
        SetCard = 0x20,
        SpecialSummon = 0x40,
        Attack = 0x80,
        Chain = 0x100
    };
    Q_DECLARE_FLAGS(CardFlags, CardFlag)

    enum {
        NoArea,
        DeckArea,
        HandArea,
        EnemyDeckArea,
        EnemyHandArea,
        FieldyardArea
    };

    CardFlags getCardFlags() const;
    void setCardFlag(CardFlag flag, bool enabled = true);
    void setCardFlags(CardFlags flags);

    int getISDN() const;
    void setISDN(int value);

    QString getName() const;
    void setName(const QString& value);

    QString getImage() const;
    void setImage(const QString& value);

    int getArea() const;
    void setArea(int value);

    QString getDescription() const;
    void setDescription(const QString& value);

    bool getFace() const;
    void setFace(bool value);

    bool getStand() const;
    void setStand(bool value);

    bool getInActive() const;
    void setInActive(bool value);

    QCursor nextCurrent();

protected:
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void hoverEnterEvent(QGraphicsSceneHoverEvent*);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

private:
    QPixmap pixmap;
    int ISDN;
    QString name;
    QString image;
    int area;
    QString description;
    bool face;
    bool stand;
    bool inActive;
    quint32 myflags: 20;
//    Status cardStatus; //右键可以显示的全部cursor
//    Status currentStatus; //当前如果鼠标移上去该显示的cursor

signals:
    void hover(QString);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Card::CardFlags)

///////////////////////////////////////////////

class CentaurWarrunner : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE CentaurWarrunner();
};

class KeeperoftheLight : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE KeeperoftheLight();
};

class Lion : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE Lion();
};

class Magnus : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE Magnus();
};

class NyxAssassin : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE NyxAssassin();
};

class Rubick : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE Rubick();
};

class Tusk : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE Tusk();
};

class Undying : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE Undying();
};

class VengefulSpirit : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE VengefulSpirit();
};

class Zeus : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE Zeus();
};

#endif // CARD_H
