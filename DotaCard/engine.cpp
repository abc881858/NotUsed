#include "engine.h"

Q_GLOBAL_STATIC(Engine, engine)

Engine *Engine::instance()
{
    return engine();
}

void Engine::loadAllCards()
{
    allcards << new Lion();

    foreach (Card* card, allcards)
    {
        metaobjects.insert(card->getISDN(),card->metaObject());
    }
}

Card* Engine::cloneCard(int ISDN)
{
    const QMetaObject *meta = metaobjects.value(ISDN);
    return qobject_cast<Card *>(meta->newInstance());
}
