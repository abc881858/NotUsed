#include "roomscene.h"
#include "engine.h"
#include <QDebug>
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include "rule.h"
#include "net.h"
#include <QMessageBox>
#include <QPushButton>

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
}

void RoomScene::setupDeck(QList<int> list)
{
    foreach (int ISDN, list)
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

void RoomScene::setupEnemyDeck(QList<int> list)
{
    foreach (int ISDN, list)
    {
        Card* card = Engine::instance()->cloneCard(ISDN);
        enemydeckarea->addCard(card);
    }
}

QList<int> RoomScene::startMyGame()
{
    QList<int> list;
    for (int i = 0; i < 5; i++)
    {
        Card* card = deckarea->takeFirstCard();
        handarea->addCard(card);
        list << card->getISDN();
    }
    return list;
}

QList<int> RoomScene::startYourGame()
{
    for (int i = 0; i < 5; i++)
    {
        Card* card = enemydeckarea->takeFirstCard();
        enemyhandarea->addCard(card);
    }

    return startMyGame();
}

void RoomScene::drawMyPhase()
{
    for (int i = 0; i < 5; i++)
    {
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

void RoomScene::doActionCommand(int parameter, int index)
{
    //999对应的是我方发起事件，我方的处理
    //888对应的是对方发起事件，我方的处理

    //对方发起了命令，我方对应要执行的操作
    switch (parameter)
    {
    /*!
     * \brief 半人马酋长
     *
     * ①将这张卡作为祭品发动，强制结束对方的战斗阶段。
     * 若这张卡装备了“dota-跳刀”，
     * 则可以改为丢弃一张手牌发动
     *
     * ②你的每回合一次，
     * 你可以让自己场上名字带有“dota”的怪兽全部变为攻击表示或防守表示，
     * 若这张卡装备了“dota-阿哈利姆神杖”时，
     * 同时令自己场上名字带有“dota”的怪兽的攻击力（或防御力）上升自己原本攻击力（或防御力）的一半。
     *
     */
    case 99991:
    {
        //普通召唤
        Card* card = handarea->takeCard(index);
        qDebug() << "card isdn from: " << card->getISDN();
        fieldyardarea->addCard(card);
        break;
    }
    case 88881:
    {
        Card* card = enemyhandarea->takeCard(index);
        qDebug() << "card isdn from: " << card->getISDN();
        enemyfieldyardarea->addCard(card);
        break;
    }
    case 99992:
    {
        //解放（在前场作为祭品）
        Card* card = fieldyardarea->takeCard(index);
        qDebug() << "card isdn from: " << card->getISDN();
        graveyardarea->addCard(card);
        break;
    }
    case 88882:
    {
        Card* card = enemyfieldyardarea->takeCard(index);
        qDebug() << "card isdn from: " << card->getISDN();
        enemygraveyardarea->addCard(card);
        break;
    }
    case 99993:
    {
        QMessageBox msgBox;
        QPushButton* atkButton = msgBox.addButton(tr("all atk"), QMessageBox::ActionRole);
        QPushButton* defButton = msgBox.addButton(tr("all def"), QMessageBox::ActionRole);
        msgBox.exec();

        if (msgBox.clickedButton() == atkButton)
        {
            //你可以让自己场上名字带有“dota”的怪兽全部变为攻击表示
            foreach (Card* card, fieldyardarea->getMyFieldyard())
            {
                card->setStand(true);
            }
        }
        else if (msgBox.clickedButton() == defButton)
        {
            //你可以让自己场上名字带有“dota”的怪兽全部变为防守表示
            foreach (Card* card, fieldyardarea->getMyFieldyard())
            {
                card->setStand(false);
            }
        }
    }
    case 88883:
    {
        Card* card = enemyfieldyardarea->takeCard(index);
        qDebug() << "card isdn from: " << card->getISDN();
        enemygraveyardarea->addCard(card);
        break;
    }
    case 8001:
        //对方from位置的卡作为祭品
        break;
    case 8002:
        //结束对方战斗流程
        break;
    case 8003:
        //对方丢弃from位置的手牌
        break;
    case 8004:
        //对方前场全部转攻击表示
        break;
    case 8005:
        //对方前场全部转防守表示
        break;
    case 8006:
        //对方前场全部上升原本攻击力的一半
        break;
    case 8007:
        //对方前场全部上升原本防御的力一半
        break;
    case 8008:
        //特招from位置手牌到场上
        break;
    /*!
     * \brief 光之守卫
     *
     * ①一回合一次，当有“dota-光之守卫”以外名字带有“dota”的加入手牌时可以发动，
     * 将那只怪兽特殊召唤至场上，若这张卡装备了“dota-阿哈利姆神杖”。
     * 则改此效果为：一回合一次，可以从手牌·卡组特殊召唤一只“dota-光之守卫”以外的名字带有“dota”的怪兽。
     *
     * ②这张卡在手牌存在时，一回合一次，将自己场上一只“dota-光之守卫”以外的名字带有“dota”的怪兽返回手牌。
     *
     * ③一回合一次，选择对方场上的一只怪兽发动，选择的怪兽在进行攻击宣言前必须丢弃一张手牌。
     *
     */
    /*!
     * \brief 恶魔巫师
     *
     * 一回合一次，选择下面的其一的效果发动：
     * 1、选择对方场上一张怪兽发动，选择的怪兽变为里侧守备表示。
     * 2、选择对方场上一张里侧守备表示的怪兽发动，选择的怪兽破坏。
     * 当这张卡装备了“dota-阿哈利姆神杖”时，同时破坏选择怪兽两边的卡。
     *
     */
    /*!
     * \brief 半人猛犸
     *
     * ①一回合一次，令自己场上一只名字带有“dota”的怪兽，
     * 在你下回合开始前攻击力上升400点，且在攻击之后，
     * 可以降低600点攻击对对方场上所有怪兽进行一次攻击。
     *
     * ②将这张卡装备的“dota-跳刀”送去墓地，
     * 将对方场上的怪兽全部转为正面攻击表示，攻击力变为0，
     * 且在对方回合结束前不能进行攻击不能更变形式。
     *
     */
    /*!
     * \brief 地穴刺客
     *
     * ①一回合一次，丢弃一张手牌，指定对方场上一只表侧表示的怪兽发动，
     * 对方受到怪兽等级的一半（向下取整且最少为1）*400点伤害，并丢弃一张手牌。
     *
     * ②这张卡有一次不会被破坏，并破坏的种类发动以下效果。
     * 战斗破坏：给与攻击怪兽控制者等同于攻击怪兽攻击的伤害。
     * 效果破坏：破坏那张卡，若那张卡需要指定的怪兽在场上才能发动，则指定的怪兽一起破坏。
     *
     * ③一回合一次，丢弃一张手卡发动，这张卡在下次攻击的伤害计算阶段结束前攻击力上升600，且不会成为攻击和卡的效果对象。
     *
     */
    /*!
     * \brief 大魔导师
     *
     * 这张卡在自己场上表侧表示存在时，自己场上名字带有“dota”的怪兽一回合一次不会被卡的效果破坏。
     * 一回合一次，选择一个效果发动：
     * 1、对方怪兽你的下回合开始前攻击力下降500；
     * 2、你获得对方的一个怪兽效果直到回合结束。当你装备“dota-阿哈利姆神杖”时，
     * 你的1、2效果可以同时发动且你的2效果1回合可以使用2次（覆盖上次获得的效果且该回合不能再次获得同一效果）
     *
     */
    /*!
     * \brief 巨牙海民
     *
     * 这张卡在进行攻击时，可以从手牌表侧攻击特殊召唤一只名字带有“dota”的怪兽，
     * 伤害计算阶段，这张卡的攻击力上升特殊召唤的怪兽攻击力的一半，
     * 这回合，由这个效果特殊召唤而来的怪兽不需要装备“dota-跳刀”便可以发动装备“dota-跳刀”的效果。
     * 一回合一次，这张卡攻击时，若目标怪兽的防御力低于你的攻击力，此次战斗对对方造成的战斗伤害翻倍。
     *
     */
    /*!
     * \brief 不朽尸王
     *
     * 一回合一次，3回合内使对方场上的一只怪兽守备力下降300点，这张卡的守备力上升300点。
     * 一回合一次，你可以弃一张手牌，在自己场上特殊召唤与对方场上怪兽数量相同的“僵尸衍生物”
     * （1星，不死族暗属性，攻500·守0，不能作为祭品、同调及超量素材，
     * 攻击对方守备力低于800的怪兽时，伤害计算阶段攻击力上升500，受到的战斗伤害为0。
     * 每进行一次攻击，伤害计算后被攻击怪兽攻·守下降200至你下回合结束。）
     *
     */
    /*!
     * \brief 复仇之魂
     *
     * ①当自己场上有“dota-复仇之魂”外的名字带有“dota”怪兽时发动。
     * 你选择一名对方4星以下的怪兽，这张卡和该怪兽的控制权交换直到回合结束，
     * 交换控制权的怪兽不能作为祭品，也不能进行攻击。
     * 若这张卡装备了“dota-阿哈利姆神杖”则可以指定对方任何怪兽且不需要自己场上存在名字带有“dota”的怪兽，
     * 当这张卡的控制权转移给对方时，你可以令这张卡装备的“dota-阿哈利姆神杖”回到手牌。
     *
     * ②只要这张卡在场上，名字带有“dota”的怪兽攻击力上升300。
     *
     * ③一回合一次，你可以指定一名怪兽使其防御力降低800。
     *
     */
    /*!
     * \brief 奥林匹斯之王
     *
     * ①当自己场上存在“dota-奥林匹斯之王”以外名字带有“dota”的怪兽时，这张卡可以从手牌特殊召唤。
     *
     * ②一回合一次，丢弃一张手牌发动，破坏对方场上一只怪兽。
     *
     * ③一回合一次，丢弃2张手牌发动，对方场上怪兽守备力降低1000点，因此效果使怪兽守备力降低至0的场合，破坏此怪兽。
     * 若这张卡装备了“dota-阿哈利姆神杖”时，改为降低2000点守备力。
     *
     */
    default:
        break;
    }
}
