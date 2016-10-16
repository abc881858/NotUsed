#include "roomscene.h"

#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QFile>
#include <QMessageBox>

#include "engine.h"
#include "rule.h"
#include "net.h"
#include "card.h"

//#define DeckAreaRect QSize(50, 72)
//#define HandAreaRect QSize(512, 75)
//#define FieldyardAreaRect QSize(384, 92)
//#define FieldgroundAreaRect QSize(384, 92)
//#define GraveyardAreaRect QSize(50, 72)

//#define EnemyDeckAreaRect QSize(50, 72)
//#define EnemyHandAreaRect QSize(512, 75)
//#define EnemyFieldyardAreaRect QSize(384, 92)
//#define EnemyFieldgroundAreaRect QSize(384, 92)
//#define EnemyGraveyardAreaRect QSize(50, 72)

#define DeckPos QPointF(485, 426)
#define HandPos QPointF(19, 529)
#define FieldyardPos QPointF(94, 317)
#define FieldgroundPos QPointF(94, 424)
#define GraveyardPos QPointF(0, 0)

#define EnemyDeckPos QPointF(14, 105)
#define EnemyHandPos QPointF(17, -71)
#define EnemyFieldyardPos QPointF(94, 213)
#define EnemyFieldgroundPos QPointF(94, 105)
#define EnemyGraveyardPos QPointF(0, 0)

RoomScene::RoomScene(QObject* parent)
    : QGraphicsScene(parent)
{
    setBackgroundBrush(QBrush(QPixmap(":/png/png/b.png")));

//    myContextMenu = new QMenu;
//    goBP = new QAction("goBP", myContextMenu);
//    goM2 = new QAction("goM2", myContextMenu);
//    goEP = new QAction("goEP", myContextMenu);
//    myContextMenu->addAction(goBP);
//    myContextMenu->addAction(goM2);
//    myContextMenu->addAction(goEP);
//    connect(goBP, SIGNAL(triggered(bool)), this, SLOT(actionBP(bool)));
//    connect(goM2, SIGNAL(triggered(bool)), this, SLOT(actionM2(bool)));
//    connect(goEP, SIGNAL(triggered(bool)), this, SLOT(actionEP(bool)));

    DeckArea::instance()->setPixmap(QPixmap(":/png/png/deck.png"));
    HandArea::instance()->setPixmap(QPixmap(":/png/png/hand.png"));
    FieldyardArea::instance()->setPixmap(QPixmap(":/png/png/fieldyard.png"));
    FieldgroundArea::instance()->setPixmap(QPixmap(":/png/png/fieldground.png"));
    GraveyardArea::instance()->setPixmap(QPixmap(":/png/png/graveyard.png"));

    EnemyDeckArea::instance()->setPixmap(QPixmap(":/png/png/enemydeck.png"));
    EnemyHandArea::instance()->setPixmap(QPixmap(":/png/png/enemyhand.png"));
    EnemyFieldyardArea::instance()->setPixmap(QPixmap(":/png/png/enemyfieldyard.png"));
    EnemyFieldgroundArea::instance()->setPixmap(QPixmap(":/png/png/enemyfieldground.png"));
    EnemyGraveyardArea::instance()->setPixmap(QPixmap(":/png/png/enemygraveyard.png"));

    DeckArea::instance()->setPos(DeckPos);
    HandArea::instance()->setPos(HandPos);
    FieldyardArea::instance()->setPos(FieldyardPos);
    FieldgroundArea::instance()->setPos(FieldgroundPos);
    GraveyardArea::instance()->setPos(GraveyardPos);
    EnemyDeckArea::instance()->setPos(EnemyDeckPos);
    EnemyHandArea::instance()->setPos(EnemyHandPos);
    EnemyFieldyardArea::instance()->setPos(EnemyFieldyardPos);
    EnemyFieldgroundArea::instance()->setPos(EnemyFieldgroundPos);
    EnemyGraveyardArea::instance()->setPos(EnemyGraveyardPos);

    FieldyardArea::instance()->initializePlace();
    EnemyFieldyardArea::instance()->initializePlace();

    addItem(DeckArea::instance());
    addItem(HandArea::instance());
    addItem(FieldyardArea::instance());
    addItem(FieldgroundArea::instance());
    addItem(GraveyardArea::instance());
    addItem(EnemyDeckArea::instance());
    addItem(EnemyHandArea::instance());
    addItem(EnemyFieldyardArea::instance());
    addItem(EnemyFieldgroundArea::instance());
    addItem(EnemyGraveyardArea::instance());

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
    connect(Net::instance(), SIGNAL(request_tellForRequest()), this, SLOT(response_tellForRequest()));

    connect(Net::instance(), SIGNAL(request_CentaurWarrunnerEffect(QJsonObject)), this, SLOT(response_CentaurWarrunnerEffect(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_KeeperoftheLightEffect(QJsonObject)), this, SLOT(response_KeeperoftheLightEffect(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_LionEffect(QJsonObject)), this, SLOT(response_LionEffect(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_MagnusEffect(QJsonObject)), this, SLOT(response_MagnusEffect(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_NyxAssassinEffect(QJsonObject)), this, SLOT(response_NyxAssassinEffect(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_RubickEffect(QJsonObject)), this, SLOT(response_RubickEffect(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_TuskEffect(QJsonObject)), this, SLOT(response_TuskEffect(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_UndyingEffect(QJsonObject)), this, SLOT(response_UndyingEffect(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_VengefulSpiritEffect(QJsonObject)), this, SLOT(response_VengefulSpiritEffect(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_ZeusEffect(QJsonObject)), this, SLOT(response_ZeusEffect(QJsonObject)));
}

//void RoomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
//{
//    if (event->button() == Qt::RightButton)
//    {
//        myContextMenu->exec(event->screenPos());
//    }
//    QGraphicsScene::mouseReleaseEvent(event);
//}

//void RoomScene::actionBP(bool)
//{
//    Rule::instance()->setPhase(Rule::myBP);
//    Rule::instance()->setDoing(false);
//}

//void RoomScene::actionM2(bool)
//{
//    Rule::instance()->setPhase(Rule::myM2);
//    Rule::instance()->setDoing(false);
//}

//void RoomScene::actionEP(bool)
//{
//    Rule::instance()->setPhase(Rule::myEP);
//    Rule::instance()->setDoing(false);
//}

//这里就是为什么不能主动调EnemyArea::instance()->response_addCard的原因
void RoomScene::response_doAddCard(QJsonObject jsonObject)
{
    int ISDN = jsonObject["ISDN"].toInt();
    int area = jsonObject["area"].toInt();
    int index = jsonObject["index"].toInt();
    bool face = jsonObject["face"].toBool();
    bool stand = jsonObject["stand"].toBool();

    switch (area)
    {
    case Deck_Area:
    {
        Card* card = Engine::instance()->cloneCard(ISDN);//TODO: 现在方便调试，加入对方手牌的hover
        connect(card, &Card::hover, [=]()
            {
                QString name = card->getName();
                QString description = card->getDescription();
                emit hover(name, description);
            });
        EnemyDeckArea::instance()->response_addCard(card);
        break;
    }
    case Hand_Area:
        EnemyHandArea::instance()->response_addCard(enemyTakedCard);
        break;
    case Fieldyard_Area:
        EnemyFieldyardArea::instance()->response_addCard(enemyTakedCard, index, face, stand);
        break;
//    case Fieldground_Area: //还没实现这个函数，等魔陷卡做的时候实现
//        EnemyFieldgroundArea::instance()->response_addCard(enemyTakedCard);
        break;
    case Graveyard_Area:
        EnemyGraveyardArea::instance()->response_addCard(enemyTakedCard);
        break;
    default:
        break;
    }
}

//这里就是为什么不能主动调EnemyArea::instance()->response_takeCard
void RoomScene::response_doTakeCard(QJsonObject jsonObject)
{
    int area = jsonObject["area"].toInt();
    int index = jsonObject["index"].toInt();

    switch (area)
    {
    case Deck_Area:
        enemyTakedCard = EnemyDeckArea::instance()->response_takeCard(index);
        break;
    case Hand_Area:
        enemyTakedCard = EnemyHandArea::instance()->response_takeCard(index);
        break;
    case Fieldyard_Area:
        enemyTakedCard = EnemyFieldyardArea::instance()->response_takeCard(index);
        break;
//    case Fieldground_Area: //还没实现这个函数，等魔陷卡做的时候实现
//        EnemyFieldgroundArea::instance()->response_takeCard(index);
//        break;
    case Graveyard_Area:
        enemyTakedCard = EnemyGraveyardArea::instance()->response_takeCard(index);
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
        DeckArea::instance()->addCard(card);

        connect(card, &Card::hover, [=]()
            {
                QString name = card->getName();
                QString description = card->getDescription();
                emit hover(name, description);
            });

        connect(card, &Card::normalSummon, [=]()
            {
                HandArea::instance()->takeCard(card->getIndex());
                FieldyardArea::instance()->addCard(card, true, true);
            });
        connect(card, &Card::setCard, [=]()
            {
                HandArea::instance()->takeCard(card->getIndex());
                FieldyardArea::instance()->addCard(card, false, false);
            });
        connect(card, &Card::tribute, [=]()
            {
                FieldyardArea::instance()->takeCard(card->getIndex());
                GraveyardArea::instance()->addCard(card);
            });
    }
    file.close();

    Net::instance()->sendMessage(2000);
}

void RoomScene::response_startGame()
{
    for (int i = 0; i < 5; i++)
    {
        Card* card = DeckArea::instance()->takeCard(0);
        HandArea::instance()->addCard(card);
    }
    Net::instance()->sendMessage(3000);
}

void RoomScene::response_drawPhase()
{
    Rule::instance()->setPhase(Rule::myDP);
    Card* card = DeckArea::instance()->takeCard();
    HandArea::instance()->addCard(card);
    Net::instance()->sendMessage(20001);
}

void RoomScene::response_standbyPhase()
{
    Rule::instance()->setPhase(Rule::mySP);
    FieldyardArea::instance()->initializeCards(); //一回合一次攻防转换
    Rule::instance()->setDoing(true);
    Rule::instance()->setOneTurnOneNormalSummon(true);
    for (Card* card : FieldyardArea::instance()->getMyFieldyard())
    {
        card->setOneTurnOneEffect(true);
        card->setOneTurnHandEffect(true);
    }
    for (Card* card : HandArea::instance()->getMyHand())
    {
        card->setOneTurnOneEffect(true);
        card->setOneTurnHandEffect(true);
    }
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

//    Rule::Phase phase = Rule::instance()->getphase();
//    if (phase == Rule::myDP || phase == Rule::mySP || phase == Rule::myM1
//        || phase == Rule::myBP || phase == Rule::myM2 || phase == Rule::myEP)
//    {
//        return;
//    }

    bool responsible = false;
    for (Card* card : FieldyardArea::instance()->getMyFieldyard())
    {
        if (card->testEffect())
        {
            responsible = true;
        }
    }
    //    for (Card* card : FieldgroundArea::instance()->getMyFieldground()) //暂时还没有后场魔陷卡
    //    {
    //        if (card->testEffect())
    //        {
    //            responsible = true;
    //        }
    //    }

    if (!responsible)
    {
        //没有可以Active的卡牌，我方进入“非 isDoing”模式，通知对方行动
        Rule::instance()->setDoing(false);
        Net::instance()->sendMessage(777);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(tr("if chain or not?"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        int ret = msgBox.exec();
        if (ret == QDialog::Rejected)
        {
            Rule::instance()->setDoing(false);
            Net::instance()->sendMessage(777);
        }
        else
        {
            msgBox.hide();
        }
    }
}

void RoomScene::response_tellForRequest()
{
    Rule::instance()->setDoing(true);

    if (Rule::instance()->getphase() == Rule::myEP)
    {
        Rule::instance()->setDoing(false);
        Net::instance()->sendMessage(70001);
    }
}
