#ifndef CARD_H
#define CARD_H

#include <QGraphicsObject>
#include <QHash>

class Card : public QGraphicsObject {
    Q_OBJECT
public:
    Card();
    int getISDN() const;
    void setISDN(int value);

    QString getName() const;
    void setName(const QString& value);

    QString getImage() const;
    void setImage(const QString& value);

    QString getArea() const;
    void setArea(const QString &value);

    QString getDescription() const;
    void setDescription(const QString &value);

    bool getFace() const;
    void setFace(bool value);

    bool getStand() const;
    void setStand(bool value);

protected:
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void hoverEnterEvent(QGraphicsSceneHoverEvent*);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*);

private:
    QPixmap pixmap;
    int ISDN;
    QString name;
    QString image;
    QString area;
    QString description;
    bool face;
    bool stand;

signals:
    void hover(QString);
};

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
