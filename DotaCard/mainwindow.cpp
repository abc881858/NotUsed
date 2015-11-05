#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new QTcpSocket;
    client->connectToHost(QHostAddress::LocalHost, 7720);
    connect(client, SIGNAL(connected()), this, SLOT(connected()));
    connect(client, SIGNAL(readyRead()), this, SLOT(readFromServer()));
//    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
//    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    myLP = 8000;
    yourLP = 8000;
    ui->myLcdNumber->display(8000);
    ui->yourLcdNumber->display(8000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connected()
{
//  NOTE: this is a example for testing write QJSON to socket server!
/*
    QJsonObject jsonObject;
    jsonObject.insert("test",QString("just tell server connected"));
    qDebug() << jsonObject;
    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    client->write(json);
*/
}

void MainWindow::readFromServer()
{
    QByteArray json = client->readAll();
    QJsonDocument jsonDoucment = QJsonDocument::fromJson(json);
    QJsonObject jsonObject = jsonDoucment.object();
    qDebug() << jsonObject;
    int command = jsonObject["command"].toInt();
    switch (command)
    {
    case 10001:
        myStartGame();
        break;
    case 10002:
        yourStartGame();
        break;
    case 20001:
        myDrawPhase();
        break;
    case 20002:
        yourDrawPhase();
        break;
    case 30001:
        myStandbyPhase();
        break;
    case 30002:
        yourStandbyPhase();
        break;
    case 40001:
        myMainPhase1();
        break;
    case 40002:
        yourMainPhase1();
        break;
    case 50001:
        myBattlePhase();
        break;
    case 50002:
        yourBattlePhase();
        break;
    case 60001:
        myMainPhase2();
        break;
    case 60002:
        yourMainPhase2();
        break;
    case 70001:
        myEndPhase();
        break;
    case 70002:
        yourEndPhase();
        break;
    default:
        break;
    }
}

void MainWindow::myStartGame()
{
    //drawCards(5);
    //do something Animation
    //for 5 from myDeck to myHand
    //返回5个int值，代表卡牌的ISDN, 唯一标识符
    QJsonObject jsonObject;
    QJsonArray cards;
    cards.append(601);
    cards.append(602);
    cards.append(603);
    cards.append(604);
    cards.append(605);
    jsonObject.insert("command", 10002);
    jsonObject.insert("cards", cards);
    qDebug() << jsonObject;
    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    client->write(json);
}

void MainWindow::yourStartGame()
{
    //do something Animation
    //for 5 from yourDeck to yourHand
    QJsonObject jsonObject;
    QJsonArray cards;
    cards.append(701);
    cards.append(702);
    cards.append(703);
    cards.append(704);
    cards.append(705);
    jsonObject.insert("command", 20001);
    jsonObject.insert("cards", cards);
    qDebug() << jsonObject;
    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    client->write(json);
}

void MainWindow::myDrawPhase()
{
    QJsonObject jsonObject;
    jsonObject.insert("command",20002);
    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    client->write(json);
}

void MainWindow::yourDrawPhase()
{
    //
}

void MainWindow::myStandbyPhase()
{
    QJsonObject jsonObject;
    jsonObject.insert("command",30002);
    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    client->write(json);
}

void MainWindow::yourStandbyPhase()
{
    //
}

void MainWindow::myMainPhase1()
{
    //
}

void MainWindow::yourMainPhase1()
{
    //
}

void MainWindow::myBattlePhase()
{
    //
}

void MainWindow::yourBattlePhase()
{
    //
}

void MainWindow::myMainPhase2()
{
    //
}

void MainWindow::yourMainPhase2()
{
    //
}

void MainWindow::myEndPhase()
{
    //
}

void MainWindow::yourEndPhase()
{
    //
}
