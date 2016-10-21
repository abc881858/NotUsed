#include "roomscene.h"

#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QFile>
#include <QMessageBox>
#include <QPropertyAnimation>

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

    for(int i=0;i<5;i++)
    {
        sword[i].setPixmap(QPixmap(":/png/png/sword.png"));
        sword[i].setPos(QPointF(80*i,0)+FieldyardPos);
        addItem(&sword[i]);
        sword[i].hide();
    }
    currentMove = -1;

    for (int j = 5; j < 10; j++)
    {
        sword[j].setPixmap(QPixmap(":/png/png/sword.png"));
        sword[j].setRotation(180);
        sword[j].setPos(QPointF(320 - 80 * (j-5), 0) + EnemyFieldyardPos);
        addItem(&sword[j]);
        sword[j].hide();
    }

    duifangxingdong = new GraphicsPixmapObject;
    duifangxingdong->setPixmap(QPixmap(":/png/png/dfxd"));
    addItem(duifangxingdong);
    duifangxingdong->hide();

    connect(Net::instance(), SIGNAL(request_doAddCard(QJsonObject)), this, SLOT(response_doAddCard(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_doTakeCard(QJsonObject)), this, SLOT(response_doTakeCard(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_doSetPhase(QJsonObject)), this, SLOT(response_doSetPhase(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_doSetDoing(QJsonObject)), this, SLOT(response_doSetDoing(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_setupDeck()), this, SLOT(response_setupDeck()));
    connect(Net::instance(), SIGNAL(request_startGame()), this, SLOT(response_startGame()));
    connect(Net::instance(), SIGNAL(request_drawPhase()), this, SLOT(response_drawPhase()));
    connect(Net::instance(), SIGNAL(request_standbyPhase()), this, SLOT(response_standbyPhase()));
    connect(Net::instance(), SIGNAL(request_main1Phase()), this, SLOT(response_main1Phase()));
    connect(Net::instance(), SIGNAL(request_doEndOpponentBattlePhase()), this, SLOT(response_doEndOpponentBattlePhase()));
    connect(Net::instance(), SIGNAL(request_askForResponse()), this, SLOT(response_askForResponse()));
    connect(Net::instance(), SIGNAL(request_tellForRequest()), this, SLOT(response_tellForRequest()));

    connect(Net::instance(), SIGNAL(request_CentaurWarrunnerEffect(QJsonObject)), this, SLOT(response_CentaurWarrunnerEffect(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_Effect(QJsonObject)), this, SLOT(response_Effect(QJsonObject)));
}

void RoomScene::doPickTarget() //注意：这是你选择的卡，不是发动效果的卡！选好了卡牌，真正active，并发送Net
{
    Card *card = qobject_cast<Card *>(sender());
    bool oldFace = card->getFace();
    int oldArea = card->getArea();
    bool oldStand = card->getStand();
    int oldIndex = card->getIndex();
    int oldcurrentMove = currentMove;

    int pickRequirement = Rule::instance()->getPickRequirement();
    qDebug() << "Card::doPickTarget() pickRequirement: " << pickRequirement;

    if (pickRequirement == AttackedRequirement)
    {
        sword[currentMove].canMove = false;
        QPropertyAnimation* animation = new QPropertyAnimation(&sword[currentMove], "pos");
        animation->setDuration(1000);
        QPointF startPos = sword[currentMove].pos();
        animation->setStartValue(startPos);
        animation->setEndValue(sword[4+oldIndex].pos());
        animation->setEasingCurve(QEasingCurve::Linear);
        animation->start();
        connect(animation, &QPropertyAnimation::finished, [=]()
            {
                sword[currentMove].hide();
                sword[currentMove].canMove = true;
                currentMove = -1;
                sword[oldcurrentMove].setPos(startPos);
                sword[oldcurrentMove].setRotation(0);
//                Rule::instance()->setIsPickingSource(true);//暂时放这里，以后处理战斗流程放在Net返回结果里
                Rule::instance()->setPickRequirement(NoRequiremente);
            });
    }
    else if (pickRequirement == KeeperoftheLightRequirement)
    {
        //给选择的对方卡牌一个debuff
        card->setDebuff(KeeperoftheLightRequirement);
    }
    else if (pickRequirement == KeeperoftheLightRequiremented)
    {
        //一般只有动对方场地的东西，才会触发Net相关的事仿
        //如果只动自己场地，因为本地的add和take卡牌都做了Net发射，所以可以跳过
        HandArea::instance()->addCard(FieldyardArea::instance()->takeCard(oldIndex));
    }
    else if (pickRequirement == LionRequirement)
    {
        if (oldFace)
        {
            card->setFace(false);
            card->setArea(EnemyFieldyard_Area);
            card->setStand(false);
        }
        else
        {
            //不应该去操作任何 EnemyArea 的addCard 和takeCard，因为这会触发对方再次发送给我add和take，记住！
            //EnemyGraveyardArea::instance()->response_addCard(EnemyFieldyardArea::instance()->response_takeCard(index));
       }
    }

    QJsonObject parameter;
    parameter.insert("pickRequirement", pickRequirement); //int型
    parameter.insert("oldFace", oldFace);
    parameter.insert("oldArea", oldArea); //点击的卡的旧位置
    parameter.insert("oldStand", oldStand);
    parameter.insert("oldIndex", oldIndex);
    parameter.insert("newFace", card->getFace());
    parameter.insert("newArea", card->getArea()); //点击的卡的新位置
    parameter.insert("newStand", card->getStand());
    parameter.insert("newIndex", card->getIndex());
    parameter.insert("oldcurrentMove", oldcurrentMove);
    QJsonObject object;
    qDebug() << "I have actived some card's effect! The effect is : " << pickRequirement;
    object.insert("request", "Effect");
    object.insert("parameter", parameter);
    Net::instance()->write(object);
}

void RoomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (currentMove != -1 && sword[currentMove].canMove)
    {
        QPointF p1 = sword[currentMove].pos() + QPointF(25,36);
        QPointF p2 = sword[currentMove].pos() + QPointF(25,0);
        QPointF p4 = event->scenePos();
        qreal angle = QLineF(p1,p4).angleTo(QLineF(p1,p2));
        sword[currentMove].setRotation(angle); //angle是弧度?
    }
    QGraphicsScene::mouseMoveEvent(event);
}

//void RoomScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    QGraphicsScene::mousePressEvent(event);
//    QGraphicsItem* item = itemAt(event->pos(),QTransform());
//}

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
        connect(card, SIGNAL(pickTarget()), this, SLOT(doPickTarget())); //选择对方卡牌时触
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
    if(phase == 3)
    {
        //TODO: 1.是否可用在setPhase::MyBP的时候加载？
        //2.就算在这加载，也不一定要全部显示为宝剑吧？
        for (Card* card : EnemyFieldyardArea::instance()->getYourFieldyard())
        {
            if (card->getFace() && card->getStand())
            {
                //Fieldyard 的index是从1-5 对应的sword数组下标 0-4
                //EnemyFieldyard 的index是从 1-5, 对应的sword数组下标 9-5
                sword[4 + card->getIndex()].show();
            }
        }
     }
}

void RoomScene::response_doSetDoing(QJsonObject jsonObject)
{
    int isDoing = jsonObject["doing"].toBool();
    if(isDoing)
    {
        duifangxingdong->show();

        QPropertyAnimation *animation = new QPropertyAnimation(duifangxingdong, "pos");
        animation->setDuration(1000);
        animation->setStartValue(QPointF(210,0));
        animation->setEndValue(QPointF(210,20));
        animation->setEasingCurve(QEasingCurve::OutBounce);
        animation->start();
    }
    else
    {
        duifangxingdong->hide();
    }
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
        connect(card, SIGNAL(pickTarget()), this, SLOT(doPickTarget()));
        connect(card, &Card::pressSword, [=](int index)
            {
                    qDebug() << "slot sword pressSword";
                    currentMove = index - 1;
                    sword[index - 1].canMove = true;
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
    Rule::instance()->setDoing(true);
    Rule::instance()->setPhase(Rule::myDP);
    Card* card = DeckArea::instance()->takeCard();
    HandArea::instance()->addCard(card);
    Net::instance()->sendMessage(20001);//TODO: 准备修改服务器实现
}

void RoomScene::response_standbyPhase()
{
    Rule::instance()->setPhase(Rule::mySP);
    FieldyardArea::instance()->initializeCards(); //一回合一次攻防转换
    Rule::instance()->setOneTurnOneNormalSummon(true);
    QList<Card*> allcards;
    allcards << DeckArea::instance()->getMyDeck()
             << HandArea::instance()->getMyHand()
             << FieldyardArea::instance()->getMyFieldyard()
             << FieldgroundArea::instance()->getMyFieldground()
             << GraveyardArea::instance()->getMyGraveyard(); //TODO: 暂时还没有除外区和额外区
    for (Card* card : allcards) //遍历所有卡牌，确保一回合一次，不会因为area变化而多次选发
    {
        card->setOneTurnOneEffect(true);//看卡牌是否写明一回合一次，比如有送入墓地必发选发的效果
        card->setOneTurnHandEffect(true);
        card->setOneTurnOneAttack(true);
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

    Rule::Phase phase = Rule::instance()->getphase();

    if (phase == Rule::myBP)
    {
        for (Card* card : FieldyardArea::instance()->getMyFieldyard())
        {
            if (card->getFace() && card->getStand())
            {
                //Fieldyard 的index是从1-5的
                sword[card->getIndex() - 1].show();
            }
        }
    }
    else if (phase == Rule::myEP)
    {
        Rule::instance()->setDoing(false);
        Net::instance()->sendMessage(70001);
    }
}

void RoomScene::response_CentaurWarrunnerEffect(QJsonObject jsonObject)
{
    bool all = jsonObject["all"].toBool();
    for (Card* card : EnemyFieldyardArea::instance()->getYourFieldyard())
    {
        //可以修改EnemyArea的face、area、stand，但不能去AddCard、TakeCard，记住！
        if (card->getFace())
        {
            card->setStand(all);
        }
    }
}

void RoomScene::response_Effect(QJsonObject object)
{
    int pickRequirement = object["pickRequirement"].toInt();
    qDebug() << "response_Effect pickRequirement: " << pickRequirement;
    if (pickRequirement == AttackedRequirement)
    {
        //收到Net消息的对端做出反应
        int oldcurrentMove = object["oldcurrentMove"].toInt();
        int oldIndex = object["oldIndex"].toInt();
        QPointF startPos = sword[5 + oldcurrentMove].pos();
        QPointF p1 = sword[oldIndex - 1].pos();
        qreal angle = QLineF(p1, startPos).angleTo(QLineF(p1, p1 + QPointF(0, -1)));
        sword[5 + oldcurrentMove].setRotation(180 + angle);
        QPropertyAnimation* animation = new QPropertyAnimation(&sword[5 + oldcurrentMove], "pos");
        animation->setDuration(1000);
        animation->setStartValue(startPos);
        animation->setEndValue(sword[oldIndex - 1].pos());
        animation->setEasingCurve(QEasingCurve::Linear);
        animation->start();
        connect(animation, &QPropertyAnimation::finished, [=]()
            {
                sword[5 + oldcurrentMove].hide();
                sword[5 + oldcurrentMove].setPos(startPos);
                sword[5 + oldcurrentMove].setRotation(180);
            });
    }
    else if (pickRequirement == KeeperoftheLightRequirement)
    {
        //    int oldArea = jsonObject["oldArea"].toInt();
        //    int oldIndex = jsonObject["oldIndex"].toInt();
        //    if (oldArea == Hand_Area)
        //    {
        //        //EnemyHandArea::instance()->response_addCard(EnemyFieldyardArea::instance()->response_takeCard(oldIndex));
        //    }
        //    else if (oldArea == Fieldyard_Area)
        //    {
        //        Card* card = FieldyardArea::instance()->getMyFieldyard().at(oldIndex);
        //        //选择的怪兽在进行攻击宣言前必须丢弃一张手牌
        //        card->setDebuff(KeeperoftheLightRequirement);
        //    }
    }
    else if (pickRequirement == KeeperoftheLightRequiremented)
    {

    }
    else if (pickRequirement == LionRequirement)
    {
        qDebug() << "response_LionEffect";
        int index = object["index"].toInt();
        Card* card = FieldyardArea::instance()->getMyFieldyard().at(index);
        if (card->getFace())
        {
            card->setFace(false);
            card->setArea(Fieldyard_Area);
            card->setStand(false);
        }
        else
        {
            GraveyardArea::instance()->addCard(FieldyardArea::instance()->takeCard(index));
        }
    }
    else
    {
        //
    }


}
