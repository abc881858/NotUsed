#ifndef ENGINE_H
#define ENGINE_H

#include <QHash>
#include <QMetaObject>
#include <QList>

#include "card.h"

class Engine {
public:
    static Engine* instance();
    void loadAllCards();
    Card* cloneCard(int ISDN);

private:
    QHash<int, const QMetaObject*> metaobjects;
    QList<Card*> allcards;
};

#endif // ENGINE_H
