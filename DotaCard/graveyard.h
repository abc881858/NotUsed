#ifndef GRAVEYARD_H
#define GRAVEYARD_H

#include <QObject>

class Graveyard : public QObject
{
    Q_OBJECT
public:
    explicit Graveyard(QObject *parent = 0);

signals:

public slots:
};

#endif // GRAVEYARD_H
