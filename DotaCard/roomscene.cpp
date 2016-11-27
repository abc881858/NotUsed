#include "roomscene.h"
#include "card.h"
#include "engine.h"
#include "net.h"
#include "rule.h"
#include <QDebug>
#include <QFile>
#include <QGraphicsSceneContextMenuEvent>
#include <QMessageBox>
#include <QPropertyAnimation>

#define DeckPos QPointF(485, 426)
#define HandPos QPointF(19, 529)
#define FieldyardPos QPointF(94, 317)
#define FieldgroundPos QPointF(94, 424)
#define GraveyardPos QPointF(485, 330)

#define EnemyDeckPos QPointF(14, 105)
#define EnemyHandPos QPointF(17, -71)
#define EnemyFieldyardPos QPointF(91, 213)
#define EnemyFieldgroundPos QPointF(91, 105)
#define EnemyGraveyardPos QPointF(15, 200)

RoomScene::RoomScene(QObject* parent)
    : QGraphicsScene(parent)
{
    setBackgroundBrush(QBrush(QPixmap(":/png/png/b.png")));

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

    HandArea::instance()->initializeBlue();
    EnemyHandArea::instance()->initializeRed();

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

    for (int i = 0; i < 5; i++)
    {
        sword[i].setPixmap(QPixmap(":/png/png/sword.png"));
        sword[i].setPos(QPointF(80 * i, 0) + FieldyardPos);
        addItem(&sword[i]);
        sword[i].hide();
    }

    for (int j = 5; j < 10; j++)
    {
        sword[j].setPixmap(QPixmap(":/png/png/sword.png"));
        sword[j].setRotation(180);
        sword[j].setPos(QPointF(320 - 80 * (j - 5), 0) + EnemyFieldyardPos);
        addItem(&sword[j]);
        sword[j].hide();
    }

    for (int k = 0; k < 5; k++)
    {
        word[k].setPos(90 + 78 * k, 390);
        addItem(&word[k]);
        word[k].setDefaultTextColor(Qt::white);
        word[k].hide();
    }

    for (int l = 5; l < 10; l++)
    {
        word[l].setPos(402 - 78 * (l - 5), 192);
        addItem(&word[l]);
        word[l].setDefaultTextColor(Qt::white);
        word[l].hide();
    }

    connect(FieldyardArea::instance(), &FieldyardArea::showWord, [=](int i) {
        Card* card = FieldyardArea::instance()->getMyFieldyard().at(i);
        int index = card->getIndex();
        word[index].setPlainText(QString::number(card->getCurrentATK()).append("/ ").append(QString::number(card->getCurrentDEF())));
        word[index].show();
    });

    connect(FieldyardArea::instance(), &FieldyardArea::hideWord, [=](int i) {
        Card* card = FieldyardArea::instance()->getMyFieldyard().at(i);
        int index = card->getIndex();
        word[index].setPlainText("");
        word[index].hide();
    });

    connect(EnemyFieldyardArea::instance(), &EnemyFieldyardArea::showWord, [=](int i) {
        Card* card = EnemyFieldyardArea::instance()->getYourFieldyard().at(i);
        int index = card->getIndex();
        word[index + 5].setPlainText(QString::number(card->getCurrentATK()).append("/ ").append(QString::number(card->getCurrentDEF())));
        word[index + 5].show();
    });

    connect(EnemyFieldyardArea::instance(), &EnemyFieldyardArea::hideWord, [=](int i) {
        Card* card = EnemyFieldyardArea::instance()->getYourFieldyard().at(i);
        int index = card->getIndex();
        word[index + 5].setPlainText("");
        word[index + 5].hide();
    });

    duifangxingdong = new GraphicsPixmapObject;
    duifangxingdong->setPixmap(QPixmap(":/png/png/dfxd"));
    addItem(duifangxingdong);
    duifangxingdong->hide();

    mySmallFrame.setPixmap(QPixmap(":/png/png/selectBlue.png"));
    yourSmallFrame.setPixmap(QPixmap(":/png/png/selectRed.png"));
    addItem(&mySmallFrame);
    addItem(&yourSmallFrame);
    mySmallFrame.hide();
    yourSmallFrame.hide();

    battleSourceCard = nullptr;
    battleDestinationCard = nullptr;
    activeEffectCard = nullptr;

    connect(Net::instance(), SIGNAL(request_doAddCard(QJsonObject)), this, SLOT(response_doAddCard(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_doTakeCard(QJsonObject)), this, SLOT(response_doTakeCard(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_doSetPhase(QJsonObject)), this, SLOT(response_doSetPhase(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_doSetDoing(QJsonObject)), this, SLOT(response_doSetDoing(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_doAddMyLP(QJsonObject)), this, SLOT(response_doAddMyLP(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_doAddYourLP(QJsonObject)), this, SLOT(response_doAddYourLP(QJsonObject)));
    connect(Net::instance(), SIGNAL(request_setupDeck()), this, SLOT(response_setupDeck()));
    connect(Net::instance(), SIGNAL(request_startGame()), this, SLOT(response_startGame()));
    connect(Net::instance(), SIGNAL(request_drawPhase()), this, SLOT(response_drawPhase()));
    connect(Net::instance(), SIGNAL(request_standbyPhase()), this, SLOT(response_standbyPhase()));
    connect(Net::instance(), SIGNAL(request_main1Phase()), this, SLOT(response_main1Phase()));
    connect(Net::instance(), SIGNAL(request_askForResponse()), this, SLOT(response_askForResponse()));
    connect(Net::instance(), SIGNAL(request_tellForRequest()), this, SLOT(response_tellForRequest()));
    connect(Net::instance(), SIGNAL(request_Effect(QJsonObject)), this, SLOT(response_Effect(QJsonObject)));
}

void RoomScene::doActiveEffect()
{
    Card* card = qobject_cast<Card*>(sender());
    qDebug() << "slot effect activate";
    if(card->getArea()==Hand_Area)
    {
        card->setOneTurnHandEffect(false);
    }
    if(card->getArea()==Fieldyard_Area)
    {
        card->setOneTurnOneEffect(false);
    }
    activeEffectCard = card;
    card->effect();
}

void RoomScene::doPressSword()
{
    Card* card = qobject_cast<Card*>(sender());
    qDebug() << "slot sword pressed";
    Rule::instance()->setPickRequirement(AttackedRequirement);
    card->setOneTurnOneAttack(false);
    battleSourceCard = card;

    if(EnemyFieldyardArea::instance()->getYourFieldyard().isEmpty())
    {
        int source = battleSourceCard->getIndex();
        QPointF p1 = sword[source].pos() + QPointF(25, 36);
        QPointF p2 = sword[source].pos() + QPointF(25, 0);
        qreal angle = QLineF(p1, QPointF(width()/2,0)).angleTo(QLineF(p1, p2));
        sword[source].setRotation(angle);
        QPropertyAnimation* animation = new QPropertyAnimation(&sword[source], "pos");
        animation->setDuration(300);
        QPointF startPos = sword[source].pos();
        animation->setStartValue(startPos);
        animation->setEndValue(QPointF(width()/2,0));
        animation->setEasingCurve(QEasingCurve::Linear);
        animation->start();
        connect(animation, &QPropertyAnimation::finished, [=]() {
            sword[source].hide();
            sword[source].setPos(startPos);
            sword[source].setRotation(0);
            Rule::instance()->setDoing(false);
            QJsonObject parameter;
            parameter.insert("pickRequirement", AttackedRequirement);
            parameter.insert("destination", -1);
            parameter.insert("source", source);
            QJsonObject object;
            object.insert("request", "Effect");
            object.insert("parameter", parameter);
            Net::instance()->write(object);
        });
    }
}

void RoomScene::doPickTarget() //注意：这是你选择的卡，不是发动效果的卡！选好了卡牌，真正active，并发送Net
{
    Card* card = qobject_cast<Card*>(sender());
    int pickRequirement = Rule::instance()->getPickRequirement();
    qDebug() << "Card::doPickTarget() pickRequirement: " << pickRequirement;

    if (pickRequirement == AttackedRequirement)
    {
        //无论表侧里侧，都可以选择作为目标target
        //不再判断isMonster了，前排肯定是怪嘛
        if (card->getArea() != EnemyFieldyard_Area)
        {
            return;
        }
        battleDestinationCard = card;
        int source = battleSourceCard->getIndex();//主动攻击方
        int destination = battleDestinationCard->getIndex();//被攻击方
        QPropertyAnimation* animation = new QPropertyAnimation(&sword[source], "pos");
        animation->setDuration(300);
        QPointF startPos = sword[source].pos();
        animation->setStartValue(startPos);
        animation->setEndValue(sword[5 + destination].pos());
        animation->setEasingCurve(QEasingCurve::Linear);
        animation->start();
        connect(animation, &QPropertyAnimation::finished, [=]() {
            sword[source].hide();
            sword[source].setPos(startPos);
            sword[source].setRotation(0);
            Rule::instance()->setDoing(false);
            QJsonObject parameter;
            parameter.insert("pickRequirement", pickRequirement); //int型
            parameter.insert("destination", destination);
            parameter.insert("source", source);
            QJsonObject object;
            object.insert("request", "Effect");
            object.insert("parameter", parameter);
            Net::instance()->write(object);
        });
    }
    else if (pickRequirement == KeeperoftheLightRequirement)
    {
        if(card->getArea()!=EnemyFieldyard_Area || !card->getFace())
        {
            return;
        }
        //给选择的对方卡牌一个debuff
        card->setBuff_602(true);
        Rule::instance()->setPickRequirement(NoRequirement);
        QJsonObject parameter;
        parameter.insert("pickRequirement", pickRequirement); //int型
        QJsonObject object;
        qDebug() << "I have actived some card's effect! The effect is : " << pickRequirement;
        object.insert("request", "Effect");
        object.insert("parameter", parameter);
        Net::instance()->write(object);
    }
    else if (pickRequirement == KeeperoftheLightRequirement2)
    {
        if(card->getArea()!=Fieldyard_Area || !card->getFace())
        {
            return;
        }
        //一般只有动对方场地的东西，才会触发Net相关的事仿
        //如果只动自己场地，因为本地的add和take卡牌都做了Net发射，所以可以跳过
        int oldIndex = card->getIndex();
        HandArea::instance()->addCard(FieldyardArea::instance()->takeCard(oldIndex));
        Rule::instance()->setPickRequirement(NoRequirement);
        QJsonObject parameter;
        parameter.insert("pickRequirement", pickRequirement); //int型
        QJsonObject object;
        qDebug() << "I have actived some card's effect! The effect is : " << pickRequirement;
        object.insert("request", "Effect");
        object.insert("parameter", parameter);
        Net::instance()->write(object);
    }
    else if (pickRequirement == LionRequirement)
    {
        if(card->getArea()!=EnemyFieldyard_Area)
        {
            return;
        }
        if (card->getFace())
        {
            card->setFace(false);
            card->setStand(false);
        }
        else
        {
            //不应该去操作任何 EnemyArea 的addCard 和takeCard，因为这会触发对方再次发送给我add和take，记住！
            //EnemyGraveyardArea::instance()->response_addCard(EnemyFieldyardArea::instance()->response_takeCard(index));
        }
        Rule::instance()->setPickRequirement(NoRequirement);
        QJsonObject parameter;
        parameter.insert("pickRequirement", pickRequirement); //int型
        parameter.insert("index", card->getIndex());
        QJsonObject object;
        qDebug() << "I have actived some card's effect! The effect is : " << pickRequirement;
        object.insert("request", "Effect");
        object.insert("parameter", parameter);
        Net::instance()->write(object);
    }
    else if (pickRequirement == MagnusRequirement)
    {
        if(card->getArea()!=Fieldyard_Area || !card->getFace())
        {
            return;
        }
        card->setBuff_604(true);
        Rule::instance()->setPickRequirement(NoRequirement);
        card->setCurrentATK(card->getCurrentATK() + 400);
        word[card->getIndex()].setPlainText(QString::number(card->getCurrentATK()).append("/ ").append(QString::number(card->getCurrentDEF())));
        QJsonObject parameter;
        parameter.insert("pickRequirement", pickRequirement);
        parameter.insert("index", card->getIndex());
        QJsonObject object;
        object.insert("request", "Effect");
        object.insert("parameter", parameter);
        Net::instance()->write(object);
    }
    else if (pickRequirement == NyxAssassinRequirement1)
    {
        if(card->getArea()!=Hand_Area)
        {
            return;
        }
        GraveyardArea::instance()->addCard(HandArea::instance()->takeCard(card->getIndex()));
        QMessageBox::information(0, QString(tr("active NyxAssassin's effect")), QString(tr("select enemy fieldyard face-up card")));
        Rule::instance()->setPickRequirement(NyxAssassinRequirement3);
    }
    else if (pickRequirement == NyxAssassinRequirement2)
    {
        if(card->getArea()!=Hand_Area)
        {
            return;
        }
        GraveyardArea::instance()->addCard(HandArea::instance()->takeCard(card->getIndex()));
        Rule::instance()->setPickRequirement(NoRequirement);
        activeEffectCard->setCurrentATK(activeEffectCard->getCurrentATK()+600);
        //TODO: 不会成为攻击和卡的效果对象
    }
    else if (pickRequirement == NyxAssassinRequirement3)
    {
        if(card->getArea()!=EnemyFieldyard_Area || card->getFace())
        {
            int addLP = card->getLevel() / 2;
            addLP *= 400;
            addLP = 0 - addLP;
            emit addYourLP(addLP);
            Net::instance()->doAddYourLP(addLP);
            Rule::instance()->setPickRequirement(NoRequirement);
            QJsonObject parameter;
            parameter.insert("pickRequirement", pickRequirement);
            QJsonObject object;
            object.insert("request", "Effect");
            object.insert("parameter", parameter);
            Net::instance()->write(object);
        }
    }
}

void RoomScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (battleSourceCard!=nullptr && !EnemyFieldyardArea::instance()->getYourFieldyard().isEmpty())
    {
        QPointF p1 = sword[battleSourceCard->getIndex()].pos() + QPointF(25, 36);
        QPointF p2 = sword[battleSourceCard->getIndex()].pos() + QPointF(25, 0);
        QPointF p4 = event->scenePos();
        qreal angle = QLineF(p1, p4).angleTo(QLineF(p1, p2));
        sword[battleSourceCard->getIndex()].setRotation(angle); //angle是弧度?
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void RoomScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::RightButton && battleSourceCard!=nullptr)
    {
        Rule::instance()->setPickRequirement(NoRequirement);
        battleSourceCard->setOneTurnOneAttack(true);
        sword[battleSourceCard->getIndex()].setRotation(0);
        battleSourceCard = nullptr;
    }
    QGraphicsScene::mousePressEvent(event);
}

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
        Card* card = Engine::instance()->cloneCard(ISDN); //TODO: 现在方便调试，加入对方手牌的hover
        connect(card, &Card::hoverEnter, [=]() {
            QString name = card->getName();
            QString description = card->getDescription();
            emit hover(name, description);

            int area = card->getArea();
            QPointF pos = card->scenePos()-QPointF(3,3);
            if(area == Hand_Area)
            {
                HandArea::instance()->showFrame(card->getIndex());
            }
            if(area == EnemyHand_Area)
            {
                EnemyHandArea::instance()->showFrame(card->getIndex());
            }
            if(area == Deck_Area || area == Fieldground_Area || area == Graveyard_Area)
            {
                mySmallFrame.setPos(pos);
                mySmallFrame.show();
            }
            if(area == Fieldyard_Area)
            {
                if(card->getStand())
                {
                    mySmallFrame.setRotation(0);
                }
                else
                {
                    mySmallFrame.setRotation(90);
                }
                mySmallFrame.setPos(pos);
                mySmallFrame.show();
            }
            if(area == EnemyDeck_Area || area == EnemyFieldyard_Area || area == EnemyGraveyard_Area)
            {
                yourSmallFrame.setPos(pos);
                yourSmallFrame.show();
            }
            if(area == EnemyFieldground_Area)
            {
                if(card->getStand())
                {
                    yourSmallFrame.setRotation(0);
                }
                else
                {
                    yourSmallFrame.setRotation(90);
                }
                yourSmallFrame.setPos(pos);
                yourSmallFrame.show();
            }
        });
        connect(card, &Card::hoverLeave, [=]() {
            int area = card->getArea();
            if(area == Hand_Area)
            {
                HandArea::instance()->hideFrame();
            }
            if(area == EnemyHand_Area)
            {
                EnemyHandArea::instance()->hideFrame();
            }
            if(area == Deck_Area || area == Fieldyard_Area || area == Fieldground_Area || area == Graveyard_Area)
            {
                mySmallFrame.hide();
            }
            if(area == EnemyDeck_Area || area == EnemyFieldyard_Area || area == EnemyFieldground_Area || area == EnemyGraveyard_Area)
            {
                yourSmallFrame.hide();
            }
        });
        connect(card, SIGNAL(pickTarget()), this, SLOT(doPickTarget())); //选择对方卡牌时触
        connect(card, SIGNAL(activeEffect()), this, SLOT(doActiveEffect()));

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
    //        break;
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
    if (phase == 3)
    {
        //TODO: 1.是否可用在setPhase::MyBP的时候加载？
        //2.就算在这加载，也不一定要全部显示为宝剑吧？
        for (Card* card : EnemyFieldyardArea::instance()->getYourFieldyard())
        {
            if (card->getFace() && card->getStand())
            {
                //Fieldyard 的index是从1-5 对应的sword数组下标 0-4
                //EnemyFieldyard 的index是从 1-5, 对应的sword数组下标 9-5
                sword[5 + card->getIndex()].show();
            }
        }
    }
}

void RoomScene::response_doAddMyLP(QJsonObject jsonObject)
{
    int addLP = jsonObject["addMyLP"].toInt();
    emit addYourLP(addLP);
}

void RoomScene::response_doAddYourLP(QJsonObject jsonObject)
{
    int addLP = jsonObject["addYourLP"].toInt();
    emit addMyLP(addLP);
}

void RoomScene::response_doSetDoing(QJsonObject jsonObject)
{
    bool isDoing = jsonObject["doing"].toBool();
    if (isDoing)
    {
        duifangxingdong->show();

        QPropertyAnimation* animation = new QPropertyAnimation(duifangxingdong, "pos");
        animation->setDuration(1000);
        animation->setStartValue(QPointF(210, 0));
        animation->setEndValue(QPointF(210, 20));
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

        connect(card, &Card::hoverEnter, [=]() {
            QString name = card->getName();
            QString description = card->getDescription();
            emit hover(name, description);

            int area = card->getArea();
            QPointF pos = card->scenePos()-QPointF(3,3);
            if(area == Hand_Area)
            {
                HandArea::instance()->showFrame(card->getIndex());
            }
            if(area == EnemyHand_Area)
            {
                EnemyHandArea::instance()->showFrame(card->getIndex());
            }
            if(area == Deck_Area || area == Fieldground_Area || area == Graveyard_Area)
            {
                mySmallFrame.setPos(pos);
                mySmallFrame.show();
            }
            if(area == Fieldyard_Area)
            {
                if(card->getStand())
                {
                    mySmallFrame.setRotation(0);
                }
                else
                {
                    mySmallFrame.setRotation(90);
                }
                mySmallFrame.setPos(pos);
                mySmallFrame.show();
            }
            if(area == EnemyDeck_Area || area == EnemyFieldyard_Area || area == EnemyGraveyard_Area)
            {
                yourSmallFrame.setPos(pos);
                yourSmallFrame.show();
            }
            if(area == EnemyFieldground_Area)
            {
                if(card->getStand())
                {
                    yourSmallFrame.setRotation(0);
                }
                else
                {
                    yourSmallFrame.setRotation(90);
                }
                yourSmallFrame.setPos(pos);
                yourSmallFrame.show();
            }
        });

        connect(card, &Card::hoverLeave, [=]() {
            int area = card->getArea();
            if(area == Hand_Area)
            {
                HandArea::instance()->hideFrame();
            }
            if(area == EnemyHand_Area)
            {
                EnemyHandArea::instance()->hideFrame();
            }
            if(area == Deck_Area || area == Fieldyard_Area || area == Fieldground_Area || area == Graveyard_Area)
            {
                mySmallFrame.hide();
            }
            if(area == EnemyDeck_Area || area == EnemyFieldyard_Area || area == EnemyFieldground_Area || area == EnemyGraveyard_Area)
            {
                yourSmallFrame.hide();
            }
        });

        connect(card, &Card::normalSummon, [=]() {
            HandArea::instance()->takeCard(card->getIndex());
            FieldyardArea::instance()->addCard(card, true, true);
        });
        connect(card, &Card::setCard, [=]() {
            HandArea::instance()->takeCard(card->getIndex());
            FieldyardArea::instance()->addCard(card, false, false);
        });
        connect(card, &Card::tribute, [=]() {
            FieldyardArea::instance()->takeCard(card->getIndex());
            GraveyardArea::instance()->addCard(card);
        });
        connect(card, SIGNAL(pickTarget()), this, SLOT(doPickTarget()));
        connect(card, SIGNAL(pressSword()), this, SLOT(doPressSword()));
        connect(card, SIGNAL(activeEffect()), this, SLOT(doActiveEffect()));
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
    Net::instance()->sendMessage(20001); //TODO: 准备修改服务器实现
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
        card->setOneTurnOneEffect(true); //看卡牌是否写明一回合一次，比如有送入墓地必发选发的效果
        card->setOneTurnHandEffect(true);
        card->setOneTurnOneAttack(true);
    }

    Net::instance()->sendMessage(30001);
}

void RoomScene::response_main1Phase()
{
    Rule::instance()->setPhase(Rule::myM1);
}

void RoomScene::response_askForResponse()
{
    Rule::instance()->setDoing(true);

    bool responsible = false;
    for (Card* card : FieldyardArea::instance()->getMyFieldyard())
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
        if (Rule::instance()->getPickRequirement() == AttackedRequirement)
        {
            if (battleDestinationCard == nullptr)
            {
                int addLP = 0 - battleSourceCard->getCurrentATK();
                emit addYourLP(addLP);
                Net::instance()->doAddYourLP(addLP);
            }
            else if (battleDestinationCard->getFace())
            {
                if (battleDestinationCard->getStand())
                {
                    if (battleSourceCard->getCurrentATK() >= battleDestinationCard->getCurrentATK())
                    {
                        int addLP = battleSourceCard->getCurrentATK()-battleDestinationCard->getCurrentATK();
                        emit addYourLP(addLP);
                        Net::instance()->doAddYourLP(addLP);
                        int index = battleDestinationCard->getIndex();
                        QJsonObject parameter;
                        parameter.insert("pickRequirement", lostTheBattle); //int型
                        parameter.insert("index", index);
                        QJsonObject object;
                        object.insert("request", "Effect");
                        object.insert("parameter", parameter);
                        Net::instance()->write(object);
                    }
                    if (battleSourceCard->getCurrentATK() <= battleDestinationCard->getCurrentATK())
                    {
                        int addLP = battleSourceCard->getCurrentATK()-battleDestinationCard->getCurrentATK();
                        emit addMyLP(addLP);
                        Net::instance()->doAddMyLP(addLP);
                        Card* card = FieldyardArea::instance()->takeCard(battleSourceCard->getIndex());
                        GraveyardArea::instance()->addCard(card);
                    }
                }
                else
                {
                    //表侧防御表示
                }
            }
            Rule::instance()->setPickRequirement(NoRequirement);
            battleSourceCard = nullptr;
            battleDestinationCard = nullptr;
        }
    }
    else if (phase == Rule::myEP)
    {
        Rule::instance()->setDoing(false);
        Net::instance()->sendMessage(70001);

        //处理结束流程之后，例如buff清除
        for (Card* card : FieldyardArea::instance()->getMyFieldyard())
        {
            if (card->getBuff_604())
            {
                card->setBuff_604(false);
                card->setCurrentATK(card->getATK());
                //refreshWord;
            }
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
        int source = object["source"].toInt();
        int destination = object["destination"].toInt();
        QPointF startPos = sword[5 + source].pos();
        QPointF p1 = destination==-1 ? QPointF(width()/2,height()) : sword[destination].pos();
        qreal angle = QLineF(p1, startPos).angleTo(QLineF(p1, p1 + QPointF(0, -1)));
        sword[5 + source].setRotation(180 + angle);
        QPropertyAnimation* animation = new QPropertyAnimation(&sword[5 + source], "pos");
        animation->setDuration(300);
        animation->setStartValue(startPos);
        animation->setEndValue(p1);
        animation->setEasingCurve(QEasingCurve::Linear);
        animation->start();
        connect(animation, &QPropertyAnimation::finished, [=]() {
            sword[5 + source].hide();
            sword[5 + source].setPos(startPos);
            sword[5 + source].setRotation(180);
            response_askForResponse();
        });
    }
    else if (pickRequirement == lostTheBattle)
    {
        int index = object["index"].toInt();
        GraveyardArea::instance()->addCard(FieldyardArea::instance()->takeCard(index));
    }
    else if (pickRequirement == CentaurWarrunnerEffect)
    {
        bool all = object["all"].toBool();
        for (Card* card : EnemyFieldyardArea::instance()->getYourFieldyard())
        {
            //可以修改EnemyArea的face、area、stand，但不能去AddCard、TakeCard，记住！
            if (card->getFace())
            {
                card->setStand(all);
            }
        }
    }
    else if (pickRequirement == KeeperoftheLightRequirement)
    {
    }
    else if (pickRequirement == KeeperoftheLightRequirement2)
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
            card->setStand(false);
        }
        else
        {
            GraveyardArea::instance()->addCard(FieldyardArea::instance()->takeCard(index));
        }
    }
    else if (pickRequirement == MagnusRequirement)
    {
        int index = object["index"].toInt();
        Card* card = EnemyFieldyardArea::instance()->getYourFieldyard().at(index);
        card->setBuff_604(true);
        card->setCurrentATK(card->getCurrentATK() + 400);
        word[card->getIndex()+5].setPlainText(QString::number(card->getCurrentATK()).append("/ ").append(QString::number(card->getCurrentDEF())));
    }
}
