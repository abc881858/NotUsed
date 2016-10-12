#include "roomscene.h"

#include <QDebug>
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include <QFile>
#include <QMessageBox>

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

    connect(Net::instance(), SIGNAL(request_doAddCard(QJsonObject)), this, SLOT(response_doAddCard(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_doTakeCard(QJsonObject)), this, SLOT(response_doTakeCard(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_doSetPhase(QJsonObject)), this, SLOT(response_doSetPhase(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_setupDeck()), this, SLOT(response_setupDeck()));
    connect(Net::instance(), SIGNAL(request_startGame()), this, SLOT(response_startGame()));
    connect(Net::instance(), SIGNAL(request_drawPhase()), this, SLOT(response_drawPhase()));
    connect(Net::instance(), SIGNAL(request_standbyPhase()), this, SLOT(response_standbyPhase()));
    connect(Net::instance(), SIGNAL(request_main1Phase()), this, SLOT(response_main1Phase()));
    connect(Net::instance(), SIGNAL(request_doEndOpponentBattlePhase()), this, SLOT(response_doEndOpponentBattlePhase()));
    connect(Net::instance(), SIGNAL(request_askForResponse()), this, SLOT(response_askForResponse()));
}

//void RoomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
//{
//    if (event->button() == Qt::RightButton)
//    {
//        myContextMenu->exec(event->screenPos());
//    }
//    QGraphicsScene::mouseReleaseEvent(event);
//}

void RoomScene::actionBP(bool)
{
    Rule::instance()->setPhase(Rule::myBP);
    Rule::instance()->setDoing(false);
}

void RoomScene::actionM2(bool)
{
    Rule::instance()->setPhase(Rule::myM2);
    Rule::instance()->setDoing(false);
}

void RoomScene::actionEP(bool)
{
    Rule::instance()->setPhase(Rule::myEP);
    Rule::instance()->setDoing(false);
}

void RoomScene::response_doAddCard(QJsonObject jsonObject)
{
    int ISDN = jsonObject["ISDN"].toInt();
    int area = jsonObject["area"].toInt();
//    int index = jsonObject["index"].toInt();
    bool face = jsonObject["face"].toBool();
    bool stand = jsonObject["stand"].toBool();

    switch (area)
    {
    case 1:
    {
        Card* card = Engine::instance()->cloneCard(ISDN);
        connect(card, &Card::hover, [=]()
            {
                QString name = card->getName();
                emit hover(name);
            });
        enemydeckarea->response_addCard(card);
        break;
    }
    case 2:
        enemyhandarea->response_addCard(enemyTakedCard);
        break;
    case 3:
        enemyfieldyardarea->response_addCard(enemyTakedCard, face, stand);
        break;
    //    case 4
    //        EnemyFieldgroundArea->response_addCard(enemyTakedCard);
    //        break;
    case 5:
        enemygraveyardarea->response_addCard(enemyTakedCard);
        break;
    default:
        break;
    }
}

void RoomScene::response_doTakeCard(QJsonObject jsonObject)
{
    int area = jsonObject["area"].toInt();
    int index = jsonObject["index"].toInt();

    switch (area)
    {
    case 1:
        enemyTakedCard = enemydeckarea->response_takeCard(index);
        break;
    case 2:
        enemyTakedCard = enemyhandarea->response_takeCard(index);
        break;
    case 3:
        enemyTakedCard = enemyfieldyardarea->response_takeCard(index);
        break;
    //    case 4:
    //        EnemyFieldgroundArea->response_takeCard(index);
    //        break;
    case 5:
        enemyTakedCard = enemygraveyardarea->response_takeCard(index);
        break;
    default:
        break;
    }
}

void RoomScene::response_doSetPhase(QJsonObject jsonObject)
{
    int phase = jsonObject["phase"].toInt();
    Rule::instance()->setPhase(phase + 6);
}

void RoomScene::response_setupDeck()
{
    QFile file("test1.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream text_stream(&file);
    while (!text_stream.atEnd())
    {
        int ISDN = text_stream.readLine().toInt();
        Card* card = Engine::instance()->cloneCard(ISDN);
        deckarea->addCard(card);

        connect(card, &Card::hover, [=]()
            {
                QString name = card->getName();
                emit hover(name);
            });

        connect(card, &Card::normalSummon, [=]()
            {
                handarea->takeCard(card->getIndex());
                fieldyardarea->addCard(card, true, true);
            });
        connect(card, &Card::setCard, [=]()
            {
                handarea->takeCard(card->getIndex());
                fieldyardarea->addCard(card, false, false);
            });
        connect(card, &Card::tribute, [=]()
            {
                fieldyardarea->takeCard(card->getIndex());
                graveyardarea->addCard(card);
            });
    }
    file.close();

    Net::instance()->sendMessage(2000);
}

void RoomScene::response_startGame()
{
    for (int i = 0; i < 5; i++)
    {
        Card* card = deckarea->takeCard(0);
        handarea->addCard(card);
    }
    Net::instance()->sendMessage(3000);
}

void RoomScene::response_drawPhase()
{
    Rule::instance()->setPhase(Rule::myDP);
    Card* card = deckarea->takeCard();
    handarea->addCard(card);
    Net::instance()->sendMessage(20001);
}

void RoomScene::response_standbyPhase()
{
    Rule::instance()->setPhase(Rule::mySP);
    fieldyardarea->initializeCards();
    Rule::instance()->setOneTurnOneNormalSummon(true);
    Net::instance()->sendMessage(30001);
}

void RoomScene::response_main1Phase()
{
    Rule::instance()->setPhase(Rule::myM1);
}

void RoomScene::response_doEndOpponentBattlePhase()
{
    Rule::instance()->setPhase(Rule::myM2);
}

void RoomScene::response_askForResponse()
{
    Rule::instance()->setDoing(true);

    Rule::Phase phase = Rule::instance()->getphase();
    if(phase == Rule::myDP || phase == Rule::mySP || phase == Rule::myM1
       || phase == Rule::myBP || phase == Rule::myM2 || phase == Rule::myEP)
    {
        return;
    }

    bool responsible = false;
    for (Card* card : fieldyardarea->getMyFieldyard())
    {
        if (card->testEffect())
        {
            responsible = true;
        }
    }
    for (Card* card : fieldgroundarea->getMyFieldground())
    {
        if (card->testEffect())
        {
            responsible = true;
        }
    }

    if (!responsible)
    {
        //没有可以Active的卡牌，我方进入“非 isDoing”模式，通知对方行动
        Rule::instance()->setDoing(false);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("if chain or not?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Close)
        {
            Rule::instance()->setDoing(false);
        }
        else
        {
            msgBox.hide();
        }
    }
}
