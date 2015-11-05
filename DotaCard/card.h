#ifndef CARD_H
#define CARD_H

#include <QObject>

class Card : public QObject
{
    Q_OBJECT
public:
    explicit Card(QObject *parent = 0);

    QString name;
    enum Primary{ null, strength, intelligence, agility }; //主属性 无、力量、智力、敏捷
    Q_ENUM(Primary)
    int star; //0-8
    QString imagePath; //图片路径
//    enum Type{ null, strength, intelligence, agility }; //种族
//    Q_ENUM(Primary)
    QString description; //效果描述


signals:

public slots:
};

#endif // CARD_H
