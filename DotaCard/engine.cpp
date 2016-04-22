#include "engine.h"

Q_GLOBAL_STATIC(Engine, engine)

Engine* Engine::instance()
{
    return engine();
}

void Engine::loadAllCards()
{
    allcards << new CentaurWarrunner()
             << new KeeperoftheLight()
             << new Lion()
             << new Magnus()
             << new NyxAssassin()
             << new Rubick()
             << new Tusk()
             << new Undying()
             << new VengefulSpirit()
             << new Zeus();

    foreach (Card* card, allcards) {
        metaobjects.insert(card->getISDN(), card->metaObject());
    }
}

Card* Engine::cloneCard(int ISDN)
{
    const QMetaObject* meta = metaobjects.value(ISDN);
    Q_ASSERT(meta != 0);
    return qobject_cast<Card*>(meta->newInstance());
}
