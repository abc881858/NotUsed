#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include "card.h"
#include <QHash>
#include <QMetaObject>

class Engine : public QObject
{
    Q_OBJECT

public:
    explicit Engine(QObject *parent = 0);
    QHash<QString, const QMetaObject> metaobjects;
    Card* cloneCard(int ISDN);

signals:

public slots:

};

extern Engine *Youxiwang;

#endif // ENGINE_H
