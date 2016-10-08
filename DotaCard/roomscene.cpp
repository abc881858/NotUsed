#include "roomscene.h"

#include <QDebug>
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>

#include "engine.h"
#include "rule.h"
#include "net.h"

static const QPointF DeckPos(485, 426);
static const QPointF HandPos(19, 529);
static const QPointF FieldyardPos(94, 317);
static const QPointF FieldgroundPos(94, 424);
static const QPointF EnemyDeckPos(14, 105);
static const QPointF EnemyHandPos(17, -71);
static const QPointF EnemyFieldyardPos(94, 213);
static const QPointF EnemyFieldgroundPos(94, 105);
static const QPointF GraveyardPos(0, 0);
static const QPointF EnemyGraveyardPos(0, 0);

RoomScene::RoomScene(QObject* parent)
    : QGraphicsScene(parent)
{
    setBackgroundBrush(QBrush(QPixmap(":/png/png/b.png")));

    myContextMenu = new QMenu;
    goBP = new QAction("goBP", myContextMenu);
    goM2 = new QAction("goM2", myContextMenu);
    goEP = new QAction("goEP", myContextMenu);
    myContextMenu->addAction(goBP);
    myContextMenu->addAction(goM2);
    myContextMenu->addAction(goEP);

    connect(goBP, SIGNAL(triggered(bool)), this, SLOT(actionBP(bool)));
    connect(goM2, SIGNAL(triggered(bool)), this, SLOT(actionM2(bool)));
    connect(goEP, SIGNAL(triggered(bool)), this, SLOT(actionEP(bool)));

    // create DeckArea
    deckarea = new DeckArea;
    deckarea->setPos(DeckPos);
    // create HandArea
    handarea = new HandArea;
    handarea->setPos(HandPos);
    // create FieldyardArea
    fieldyardarea = new FieldyardArea;
    fieldyardarea->setPos(FieldyardPos);
    // create FieldgroundArea
    fieldgroundarea = new FieldgroundArea;
    fieldgroundarea->setPos(FieldgroundPos);
    // create GraveyardArea
    graveyardarea = new GraveyardArea;
    graveyardarea->setPos(GraveyardPos);

    // create Enemy
    enemydeckarea = new EnemyDeckArea;
    enemydeckarea->setPos(EnemyDeckPos);
    enemyhandarea = new EnemyHandArea;
    enemyhandarea->setPos(EnemyHandPos);
    enemyfieldyardarea = new EnemyFieldyardArea;
    enemyfieldyardarea->setPos(EnemyFieldyardPos);
    enemyfieldgroundarea = new EnemyFieldgroundArea;
    enemyfieldgroundarea->setPos(EnemyFieldgroundPos);
    enemygraveyardarea = new EnemyGraveyardArea;
    enemygraveyardarea->setPos(EnemyGraveyardPos);

    addItem(deckarea);
    addItem(handarea);
    addItem(fieldyardarea);
    addItem(fieldgroundarea);
    addItem(graveyardarea);
    addItem(enemydeckarea);
    addItem(enemyhandarea);
    addItem(enemyfieldyardarea);
    addItem(enemyfieldgroundarea);
    addItem(enemygraveyardarea);

    connect(Net::instance(), SIGNAL(setupDeck(QList<int>)), this, SLOT(setupDeck(QList<int>)));
    connect(Net::instance(), SIGNAL(request_doAddCard(QJsonObject)), this, SLOT(doAddCard(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_doTakeCard(QJsonObject)), this, SLOT(doTakeCard(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_doSetPhase(QJsonObject)), this, SLOT(doSetPhase(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_startGame()), this, SLOT(startGame()));
    connect(Net::instance(), SIGNAL(request_drawPhase()), this, SLOT(drawPhase()));
    connect(Net::instance(), SIGNAL(request_standbyPhase()), this, SLOT(standbyPhase()));
    connect(Net::instance(), SIGNAL(request_main1Phase()), this, SLOT(main1Phase()));
    connect(Net::instance(), SIGNAL(request_battlePhase()), this, SLOT(battlePhase()));
    connect(Net::instance(), SIGNAL(request_main2Phase()), this, SLOT(main2Phase()));
    connect(Net::instance(), SIGNAL(request_endPhase()), this, SLOT(endPhase()));
}

void RoomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        myContextMenu->exec(event->screenPos());
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void RoomScene::actionBP(bool)
{
    Rule::instance()->setPhase(Rule::myBP);
    Net::instance()->sendMessage(50001);
    Rule::instance()->setIsWaiting(true);
}

void RoomScene::actionM2(bool)
{
    Rule::instance()->setPhase(Rule::myM2);
    Net::instance()->sendMessage(60001);
    Rule::instance()->setIsWaiting(true);
}

void RoomScene::actionEP(bool)
{
    Rule::instance()->setPhase(Rule::myEP);
    Net::instance()->sendMessage(70001);
    Rule::instance()->setIsWaiting(true);
}

void RoomScene::setupDeck(QList<int> list)
{
    for (int ISDN : list)
    {
        Card* card = Engine::instance()->cloneCard(ISDN);
        deckarea->addCard(card);

        connect(card, &Card::hover, [=]()
            {
                QString name = card->getName();
                emit hover(name);
            });
    }
}

void RoomScene::doAddCard(QJsonObject jsonObject)
{
    int ISDN = jsonObject["ISDN"].toInt();
    int area = jsonObject["area"].toInt();
    int index = jsonObject["index"].toInt();
    bool face = jsonObject["face"].toBool();
    bool stand = jsonObject["stand"].toBool();

    Card* card = Engine::instance()->cloneCard(ISDN);
    card->setIndex(index);
    card->setFace(face);
    card->setStand(stand);

    switch (area)
    {
    case Card::DeckArea:
        enemydeckarea->response_addCard(card);
        break;
    case Card::HandArea:
        enemyhandarea->response_addCard(card);
        break;
    case Card::FieldyardArea:
        enemyfieldyardarea->response_addCard(card);
        break;
    //    case Card::FieldgroundArea:
    //        EnemyFieldgroundArea->response_addCard(ISDN, index, face, stand);
    //        break;
    case Card::GraveyardArea:
        enemygraveyardarea->response_addCard(card);
        break;
    default:
        break;
    }
}

void RoomScene::doTakeCard(QJsonObject jsonObject)
{
    int area = jsonObject["area"].toInt();
    int index = jsonObject["index"].toInt();

    switch (area)
    {
    case Card::DeckArea:
        enemydeckarea->response_takeCard(index);
        break;
    case Card::HandArea:
        enemyhandarea->response_takeCard(index);
        break;
    case Card::FieldyardArea:
        enemyfieldyardarea->response_takeCard(index);
        break;
    //    case Card::FieldgroundArea:
    //        EnemyFieldgroundArea->response_takeCard(index);
    //        break;
    case Card::GraveyardArea:
        enemygraveyardarea->response_takeCard(index);
        break;
    default:
        break;
    }
}

void RoomScene::doSetPhase(QJsonObject jsonObject)
{
    int phase = jsonObject["phase"].toInt();
    Rule::instance()->setPhase(phase+6);
}

void RoomScene::startGame()
{
    for (int i = 0; i < 5; i++)
    {
        Card* card = deckarea->takeCard(0);
        handarea->addCard(card);
    }
    Net::instance()->sendMessage(20000);
}

void RoomScene::drawPhase()
{
    Rule::instance()->setPhase(Rule::myDP);
    Card* card = deckarea->takeCard();
    handarea->addCard(card);
    Net::instance()->sendMessage(20001);
}

void RoomScene::standbyPhase()
{
    Rule::instance()->setPhase(Rule::mySP);
    fieldyardarea->initializeCards();
    Rule::instance()->setOneTurnOneNormalSummon(true);
    Net::instance()->sendMessage(30001);
}

void RoomScene::main1Phase()
{
    Rule::instance()->setPhase(Rule::myM1);
}

void RoomScene::battlePhase()
{
    Rule::instance()->setPhase(Rule::myBP);
}

void RoomScene::main2Phase()
{
    Rule::instance()->setPhase(Rule::myM2);
}

void RoomScene::endPhase()
{
    Rule::instance()->setPhase(Rule::myEP);
    Net::instance()->sendMessage(70001);
}
