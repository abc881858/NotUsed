#include "roomscene.h"
#include "engine.h"

static const QPointF DeckPos(485, 426);
static const QPointF HandPos(19, 529);
static const QPointF FieldyardPos(94, 317);
static const QPointF FieldgroundPos(94, 424);
static const QPointF EnemyDeckPos(14, 105);
static const QPointF EnemyHandPos(17, -71);
static const QPointF EnemyFieldyardPos(94, 213);
static const QPointF EnemyFieldgroundPos(94, 105);

RoomScene::RoomScene(QObject* parent)
    : QGraphicsScene(parent)
{
    setBackgroundBrush(QBrush(QPixmap(":/png/png/b.png")));

    // create DeckArea
    deckarea = new DeckArea;
    addItem(deckarea);
    deckarea->setPos(DeckPos);
    // create HandArea
    handarea = new HandArea;
    addItem(handarea);
    handarea->setPos(HandPos);
    // create FieldyardArea
    fieldyardarea = new FieldyardArea;
    addItem(fieldyardarea);
    fieldyardarea->setPos(FieldyardPos);
    // create FieldgroundArea
    fieldgroundarea = new FieldgroundArea;
    addItem(fieldgroundarea);
    fieldgroundarea->setPos(FieldgroundPos);

    // create Enemy
    enemydeckarea = new EnemyDeckArea;
    addItem(enemydeckarea);
    enemydeckarea->setPos(EnemyDeckPos);
    enemyhandarea = new EnemyHandArea;
    addItem(enemyhandarea);
    enemyhandarea->setPos(EnemyHandPos);
    enemyfieldyardarea = new EnemyFieldyardArea;
    addItem(enemyfieldyardarea);
    enemyfieldyardarea->setPos(EnemyFieldyardPos);
    enemyfieldgroundarea = new EnemyFieldgroundArea;
    addItem(enemyfieldgroundarea);
    enemyfieldgroundarea->setPos(EnemyFieldgroundPos);
}

void RoomScene::setupDeck(QList<int> list)
{
    foreach (int ISDN, list) {
        Card* card = Engine::instance()->cloneCard(ISDN);
        deckarea->addCard(card);

        QObject::connect(card, &Card::hover, [=] () {
            QString name = card->getName();
            emit hover(name);
        });

        QObject::connect(card, &Card::doNormalSummon, [=] () {
            QString name = card->getName();

        });
        QObject::connect(card, &Card::doSetCard, [=] () {
            QString name = card->getName();

        });
//        QObject::connect(card, &Card::doChain, [=] () {
//            QString name = card->getName();
//            emit hover(name);
//        });
//        QObject::connect(card, &Card::doEffect, [=] () {
//            QString name = card->getName();
//            emit hover(name);
//        });
//        QObject::connect(card, &Card::doSpecialSummon, [=] () {
//            QString name = card->getName();
//            emit hover(name);
//        });

//        connect(card,SIGNAL(doNormalSummon()),this,SLOT(doNormalSummon()));
//        connect(card,SIGNAL(doSetCard()),this,SLOT(doSetCard()));
//        connect(card,SIGNAL(doChain()),this,SLOT(doChain()));
//        connect(card,SIGNAL(doEffect()),this,SLOT(doEffect()));
//        connect(card,SIGNAL(doSpecialSummon()),this,SLOT(doSpecialSummon()));
    }
}

void RoomScene::setupEnemyDeck(QList<int> list)
{
    foreach (int ISDN, list) {
        Card* card = Engine::instance()->cloneCard(ISDN);
        enemydeckarea->addCard(card);
    }
}

QList<int> RoomScene::startMyGame()
{
    QList<int> list;
    for (int i = 0; i < 5; i++) {
        Card* card = deckarea->takeFirstCard();
        handarea->addCard(card);
        list << card->getISDN();
    }
    return list;
}

QList<int> RoomScene::startYourGame()
{
    for (int i = 0; i < 5; i++) {
        Card* card = enemydeckarea->takeFirstCard();
        enemyhandarea->addCard(card);
    }

    return startMyGame();
}

void RoomScene::drawMyPhase()
{
    for (int i = 0; i < 5; i++) {
        Card* card = enemydeckarea->takeFirstCard();
        enemyhandarea->addCard(card);
    }

    Card* card = deckarea->takeFirstCard();
    handarea->addCard(card);
}

void RoomScene::drawYourPhase()
{
    Card* card = enemydeckarea->takeFirstCard();
    enemyhandarea->addCard(card);
}

void RoomScene::endYourPhase()
{
    Card* card = deckarea->takeFirstCard();
    handarea->addCard(card);
}

void RoomScene::initializeFieldyard()
{
    fieldyardarea->initializeCards();
}
