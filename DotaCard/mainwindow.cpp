#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPalette>
#include <QBrush>
#include <QPixmap>

#include "net.h"
#include "rule.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/png/png/back.jpg")));
    setPalette(palette);

    Net::instance()->initialize();

    connect(Net::instance(), SIGNAL(setupDeck(QList<int>)), this, SLOT(setupDeck(QList<int>)));
    connect(Net::instance(), SIGNAL(setupEnemyDeck(QList<int>)), this, SLOT(setupEnemyDeck(QList<int>)));

    connect(Net::instance(), SIGNAL(myStartGame()), this, SLOT(startMyGame()));
    connect(Net::instance(), SIGNAL(yourStartGame()), this, SLOT(startYourGame()));
    connect(Net::instance(), SIGNAL(myDrawPhase()), this, SLOT(drawMyPhase()));
    connect(Net::instance(), SIGNAL(yourDrawPhase()), this, SLOT(drawYourPhase()));
    connect(Net::instance(), SIGNAL(myStandbyPhase()), this, SLOT(standbyMyPhase()));
    connect(Net::instance(), SIGNAL(yourStandbyPhase()), this, SLOT(standbyYourPhase()));
    connect(Net::instance(), SIGNAL(myMainPhase1()), this, SLOT(mainMyPhase1()));
    connect(Net::instance(), SIGNAL(yourMainPhase1()), this, SLOT(mainYourPhase1()));
    connect(Net::instance(), SIGNAL(yourBattlePhase()), this, SLOT(battleYourPhase()));
    connect(Net::instance(), SIGNAL(yourMainPhase2()), this, SLOT(mainYourPhase2()));
    connect(Net::instance(), SIGNAL(yourEndPhase()), this, SLOT(endYourPhase()));

    //对端做了什么操作，我方都要显示出来
    connect(Net::instance(), SIGNAL(actionCommand(int, int, int)), this, SLOT(doActionCommand(int, int, int)));

    myLP = 8000;
    yourLP = 8000;
    ui->myLcdNumber->display(myLP);
    ui->yourLcdNumber->display(yourLP);

    roomScene = new RoomScene(ui->roomWidget);
    ui->roomView->setScene(roomScene);
    ui->roomView->setSceneRect(0, 0, 550, 600);

    connect(roomScene, SIGNAL(hover(QString)), this, SLOT(hover(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupDeck(QList<int> list)
{
    roomScene->setupDeck(list);
}

void MainWindow::setupEnemyDeck(QList<int> list)
{
    roomScene->setupEnemyDeck(list);
}

void MainWindow::hover(QString name)
{
    //首先一张卡被hover，左上角大图肯定要更新
    //但对方的（除前后场的face-up卡）不该显示，我方卡组也不该显示
    ui->label->setStyleSheet(QString("image: url(:/pic/monster/%1.jpg)").arg(name));
}

void MainWindow::startMyGame()
{
    QList<int> list = roomScene->startMyGame();
    Net::instance()->sendMessage(10001, list);
}

void MainWindow::startYourGame()
{
    //client2 start his game
    QList<int> list = roomScene->startYourGame();
    Net::instance()->sendMessage(10002, list);
}

void MainWindow::drawMyPhase()
{
    Rule::instance()->setPhase(Rule::myDP);
    roomScene->drawMyPhase();
    Net::instance()->sendMessage(20001);
}

void MainWindow::drawYourPhase()
{
    Rule::instance()->setPhase(Rule::yourDP);
    roomScene->drawYourPhase();
    Net::instance()->sendMessage(20002);
}

void MainWindow::standbyMyPhase()
{
    Rule::instance()->setPhase(Rule::mySP);
    Net::instance()->sendMessage(30001);
}

void MainWindow::standbyYourPhase()
{
    Rule::instance()->setPhase(Rule::yourSP);
    Net::instance()->sendMessage(30002);
}

void MainWindow::mainMyPhase1()
{
    Rule::instance()->setPhase(Rule::myM1);
    Rule::instance()->setOneTurnOneNormalSummon(true);
    roomScene->initializeFieldyard();
    Net::instance()->sendMessage(40001);
}

void MainWindow::mainYourPhase1()
{
    Rule::instance()->setPhase(Rule::yourM2);
}

void MainWindow::battleYourPhase()
{
    Rule::instance()->setPhase(Rule::yourBP);
}

void MainWindow::mainYourPhase2()
{
    Rule::instance()->setPhase(Rule::yourM2);
}

void MainWindow::endYourPhase()
{
    Rule::instance()->setPhase(Rule::yourEP);
    //TODO: 也许对方结束阶段我要触发什么陷阱卡
    //暂时玩家2直接进入他的抽卡阶段

    Rule::instance()->setPhase(Rule::myDP);
    roomScene->endYourPhase();
    Net::instance()->sendMessage(20001);
}

void MainWindow::doActionCommand(int parameter, int from, int to)
{
    roomScene->doActionCommand(parameter, from, to);
}
