#ifndef CARD_H
#define CARD_H

#include <QGraphicsObject>
#include <QHash>

//const static QHash<int, QString> allnames = {
//    { 601, "dota-Lion" },
//    { 602, "dota-Rubick" },
//    { 603, "dota-Tusk" },
//    { 604, "dota-Magnus" },
//    { 605, "dota-NyxAssassin" },
//    { 606, "dota-KeeperoftheLight" },
//    { 607, "dota-CentaurWarrunner" },
//    { 608, "dota-Undying" },
//    { 609, "dota-VengefulSpirit" },
//    { 610, "dota-Zeus" }
//};

class Card : public QGraphicsObject {
    Q_OBJECT
public:
    explicit Card(int value = -1);
    int getISDN() const;
    void setISDN(int value);

    QString getName() const;
    void setName(const QString &value);

    QString getImage() const;
    void setImage(const QString &value);

protected:
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void hoverEnterEvent(QGraphicsSceneHoverEvent*);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*);
//    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    QPixmap pixmap;
    int ISDN;
    QString name;
    QString area;
    QString image;
    QString description;
signals:
    void hover(QString);
//    void left_click();
//    void right_click();
};

class Lion : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE Lion();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CARD_H
