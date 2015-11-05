#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new QTcpServer;
    server->listen(QHostAddress::Any, 7720);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
//    connect(server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(acceptError(QAbstractSocket::SocketError)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newConnection()
{
    qDebug() << "a client socket connected...";
    QTcpSocket *newSock = server->nextPendingConnection();

    if(clients.count()==0)
    {
        newSock->setObjectName("firstPlayer");
        connect(newSock, SIGNAL(readyRead()), this, SLOT(readFromFirstClient()));
        clients.append(newSock);
        qDebug() << "current clients count: " << clients.count();
    }
    else if(clients.count()==1)
    {
        newSock->setObjectName("secondPlayer");
        connect(newSock, SIGNAL(readyRead()), this, SLOT(readFromSecondClient()));
        clients.append(newSock);
        qDebug() << "current clients count: " << clients.count();
        qDebug() << "room is full, now we disconnect newConnection slot";
        disconnect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
        qDebug() << "the two players are ready, we start the game. First in, first go!";

        //TODO: may do finger-guessing game Animation later. first player first go Now.
        startGame();
    }
}

void MainWindow::readFromFirstClient()
{
    QByteArray json = clients[0]->readAll();
    qDebug() << QJsonDocument::fromJson(json).object();
    if(clients.count()==2)
    {
        clients[1]->write(json);
    }
    else
    {
        qDebug() << "waiting for second player...";
    }
}

void MainWindow::readFromSecondClient()
{
    QByteArray json = clients[1]->readAll();
    qDebug() << QJsonDocument::fromJson(json).object();
    clients[0]->write(json);
}

/*
 * command 定义：
 *
 * 10001 双方猜拳结束，比赛正式开始，假定 firstPlayer 先攻，先攻的先抽卡，初始5张。
 *
*/

void MainWindow::startGame()
{
    QJsonObject jsonObject;
    jsonObject.insert("command", 10001);
    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    clients[0]->write(json);
}
