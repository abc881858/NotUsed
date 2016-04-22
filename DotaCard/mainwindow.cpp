#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPalette>
#include <QBrush>
#include <QPixmap>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/png/png/back.jpg")));
    setPalette(palette);

    net = new Net(this);

    connect(net, SIGNAL(setupDeck(QList<int>)), this, SLOT(setupDeck(QList<int>)));
    connect(net, SIGNAL(setupEnemyDeck(QList<int>)), this, SLOT(setupEnemyDeck(QList<int>)));

    connect(net, SIGNAL(myStartGame()), this, SLOT(startMyGame()));
    connect(net, SIGNAL(yourStartGame()), this, SLOT(startYourGame()));
    connect(net, SIGNAL(myDrawPhase()), this, SLOT(drawMyPhase()));
    connect(net, SIGNAL(yourDrawPhase()), this, SLOT(drawYourPhase()));
    connect(net, SIGNAL(myStandbyPhase()), this, SLOT(standbyMyPhase()));
    connect(net, SIGNAL(yourStandbyPhase()), this, SLOT(standbyYourPhase()));
    connect(net, SIGNAL(myMainPhase1()), this, SLOT(mainMyPhase1()));
    connect(net, SIGNAL(yourMainPhase1()), this, SLOT(mainYourPhase1()));
    connect(net, SIGNAL(yourBattlePhase()), this, SLOT(battleYourPhase()));
    connect(net, SIGNAL(yourMainPhase2()), this, SLOT(mainYourPhase2()));
    connect(net, SIGNAL(yourEndPhase()), this, SLOT(endYourPhase()));

    myLP = 8000;
    yourLP = 8000;
    ui->myLcdNumber->display(myLP);
    ui->yourLcdNumber->display(yourLP);

    roomScene = new RoomScene(ui->roomWidget);
    ui->roomView->setScene(roomScene);
    ui->roomView->setSceneRect(0, 0, 550, 600);

    connect(roomScene, SIGNAL(hoverCard(QString)), this, SLOT(hoverCard(QString)));
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

void MainWindow::setPhase(MainWindow::Phase phase)
{
    this->phase = phase;
}

MainWindow::Phase MainWindow::getphase() const
{
    return this->phase;
}

void MainWindow::hoverCard(QString name)
{
    //首先一张卡被hover，左上角大图肯定要更新
    //但对方的（除前后场的face-up卡）不该显示，我方卡组也不该显示
    ui->label->setStyleSheet(QString("image: url(:/pic/monster/%1.jpg)").arg(name));
}

void MainWindow::startMyGame()
{
    QList<int> list = roomScene->startMyGame();
    net->sendMessage(10001, list);
}

void MainWindow::startYourGame()
{
    //client2 start his game
    QList<int> list = roomScene->startYourGame();
    net->sendMessage(10002, list);
}

void MainWindow::drawMyPhase()
{
    setPhase(myDP);
    roomScene->drawMyPhase();
    net->sendMessage(20001);
}

void MainWindow::drawYourPhase()
{
    setPhase(yourDP);
    roomScene->drawYourPhase();
    net->sendMessage(20002);
}

void MainWindow::standbyMyPhase()
{
    setPhase(mySP);
    net->sendMessage(30001);
}

void MainWindow::standbyYourPhase()
{
    setPhase(yourSP);
    net->sendMessage(30002);
}

void MainWindow::mainMyPhase1()
{
    setPhase(myM1);
    net->sendMessage(40001);
}

void MainWindow::mainYourPhase1()
{
    setPhase(yourM2);
}

void MainWindow::battleYourPhase()
{
    setPhase(yourBP);
}

void MainWindow::mainYourPhase2()
{
    setPhase(yourM2);
}

void MainWindow::endYourPhase()
{
    setPhase(yourEP);
    //TODO: 也许对方结束阶段我要触发什么陷阱卡
    //暂时玩家2直接进入他的抽卡阶段

    setPhase(myDP);
    roomScene->endYourPhase();
    net->sendMessage(20001);
}
