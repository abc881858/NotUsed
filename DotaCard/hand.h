#ifndef HAND_H
#define HAND_H

#include <QObject>

class Hand : public QObject
{
    Q_OBJECT
public:
    explicit Hand(QObject *parent = 0);
};

#endif // HAND_H
