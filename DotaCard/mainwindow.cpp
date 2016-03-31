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
    this->setPalette(palette);

    net = new Net(this);

    connect(net,SIGNAL(myStartGame()),this,SLOT(startMyGame()));
    connect(net,SIGNAL(yourStartGame()),this,SLOT(startYourGame()));
    connect(net,SIGNAL(myDrawPhase()),this,SLOT(drawMyPhase()));
    connect(net,SIGNAL(yourDrawPhase()),this,SLOT(drawYourPhase()));
    connect(net,SIGNAL(myStandbyPhase()),this,SLOT(standbyMyPhase()));
    connect(net,SIGNAL(yourStandbyPhase()),this,SLOT(standbyYourPhase()));
    connect(net,SIGNAL(myMainPhase1()),this,SLOT(mainMyPhase1()));
    connect(net,SIGNAL(yourMainPhase1()),this,SLOT(mainYourPhase1()));
    connect(net,SIGNAL(yourBattlePhase()),this,SLOT(battleYourPhase()));
    connect(net,SIGNAL(yourMainPhase2()),this,SLOT(mainYourPhase2()));
    connect(net,SIGNAL(yourEndPhase()),this,SLOT(endYourPhase()));

    myLP = 8000;
    yourLP = 8000;
    ui->myLcdNumber->display(myLP);
    ui->yourLcdNumber->display(yourLP);

    roomScene = new RoomScene(ui->roomWidget);
    ui->roomView->setScene(roomScene);
    ui->roomView->setSceneRect(0, 0, 550, 600);

//    this->myDeck = roomScene->deckarea->myDeck;
//    this->myHand = roomScene->handarea->myHand;
//    this->myFieldyard = roomScene->fieldyardarea->myFieldyard;
//    this->myFieldground = roomScene->fieldgroundarea->myFieldground;
//    this->yourDeck = roomScene->enemydeckarea->yourDeck;
//    this->yourHand = roomScene->enemyhandarea->yourHand;
//    this->yourFieldyard = roomScene->enemyfieldyardarea->yourFieldyard;
//    this->yourFieldground = roomScene->enemyfieldgroundarea->yourFieldground;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_buttonBP_clicked()
//{
//    setPhase(myBP);
//    client->write(getJsonFromInt(50001));
//}

//void MainWindow::on_buttonM2_clicked()
//{
//    setPhase(myM2);
//    client->write(getJsonFromInt(60001));
//}

//void MainWindow::on_buttonEP_clicked()
//{
//    setPhase(myEP);
//    client->write(getJsonFromInt(70001));
//}

void MainWindow::setPhase(MainWindow::Phase phase)
{
    this->phase = phase;
}

MainWindow::Phase MainWindow::getphase() const
{
    return this->phase;
}

void MainWindow::setBigImage(QString s)
{
    ui->label->setStyleSheet(QString("image: url(%1)").arg(s));
}

void MainWindow::startMyGame()
{

}

void MainWindow::startYourGame()
{

}

void MainWindow::drawMyPhase()
{

}

void MainWindow::drawYourPhase()
{

}

void MainWindow::standbyMyPhase()
{

}

void MainWindow::standbyYourPhase()
{

}

void MainWindow::mainMyPhase1()
{

}

void MainWindow::mainYourPhase1()
{

}

void MainWindow::battleYourPhase()
{

}

void MainWindow::mainYourPhase2()
{

}

void MainWindow::endYourPhase()
{

}
