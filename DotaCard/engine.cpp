#include "engine.h"

Engine *Youxiwang = NULL;

Engine::Engine(QObject *parent) : QObject(parent)
{
    metaobjects.insert(601, Lion::staticMetaObject());
}

Card* Engine::cloneCard(int ISDN)
{
    QMetaObject meta = metaobjects.value(ISDN);
    Card *card = qobject_cast<Card *>(meta.newInstance());
    return card;
}
