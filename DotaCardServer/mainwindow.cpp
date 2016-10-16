#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QWebSocketServer>
#include <QWebSocket>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new QWebSocketServer(QString("Card Server"), QWebSocketServer::NonSecureMode, this);
    server->listen(QHostAddress::Any, 7720);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    //  connect(server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(acceptError(QAbstractSocket::SocketError)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::write(QWebSocket* socket, QJsonObject jsonObject)
{
    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    socket->sendBinaryMessage(json);
}

void MainWindow::newConnection()
{
    qDebug() << "a client socket connected...";
    QWebSocket* newSock = server->nextPendingConnection();

    if (clients.count() == 0)
    {
        newSock->setObjectName("firstPlayer");
        connect(newSock, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(readFromFirstClient(QByteArray)));
        clients.append(newSock);
        qDebug() << "current clients count: " << clients.count();
    }
    else if (clients.count() == 1)
    {
        newSock->setObjectName("secondPlayer");
        connect(newSock, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(readFromSecondClient(QByteArray)));
        clients.append(newSock);
        qDebug() << "current clients count: " << clients.count();
        qDebug() << "room is full, now we disconnect newConnection slot";
        disconnect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
        qDebug() << "the two players are ready, we start the game. First in, first go!";
    }
}

void MainWindow::readFromFirstClient(QByteArray byteArray)
{
    QJsonDocument jsonDoucment = QJsonDocument::fromJson(byteArray);
    QJsonObject jsonObject = jsonDoucment.object();
    qDebug() << "readFromFirstClient: " << jsonObject;

    if (clients.count() == 1)
    {
        qDebug() << "waiting for second player...";
        return;
    }

    //toInt默认值是0，所以就算没有command，也会返回0，请求响应code改为666
    if (jsonObject["command"].toInt() == 666)
    {
        QJsonObject jsonObject;
        jsonObject.insert("request", "askForResponse");
        write(clients[1], jsonObject);
    }
    else if (jsonObject["command"].toInt() == 777)
    {
        QJsonObject jsonObject;
        jsonObject.insert("request", "tellForRequest");
        write(clients[1], jsonObject);
    }
    else if (jsonObject["command"].toInt() == 2000)
    {
        QJsonObject jsonObject;
        jsonObject.insert("request", "setupDeck");
        write(clients[1], jsonObject);
    }
    else if (jsonObject["command"].toInt() == 3000)
    {
        QJsonObject jsonObject;
        jsonObject.insert("request", "startGame");
        write(clients[1], jsonObject);
    }
    else if (jsonObject["command"].toInt() == 20001)
    {
        //默认对端无响应，直接进入standbyPhase（即30001阶段）
        QJsonObject jsonObject;
        jsonObject.insert("request", "standbyPhase");
        write(clients[0], jsonObject);
    }
    else if (jsonObject["command"].toInt() == 30001)
    {
        //默认对端无响应，直接进入main1Phase（即40001阶段）
        QJsonObject jsonObject;
        jsonObject.insert("request", "main1Phase");
        write(clients[0], jsonObject);
    }
//    else if (jsonObject["command"].toInt() == 40001)
//    {
//        QJsonObject jsonObject;
//        jsonObject.insert("request", "battlePhase");
//        QJsonDocument jsonDoucment(jsonObject);
//        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
//        clients[1]->write(json);
//    }
//    else if (jsonObject["command"].toInt() == 50001)
//    {
//        QJsonObject jsonObject;
//        jsonObject.insert("request", "main2Phase");
//        QJsonDocument jsonDoucment(jsonObject);
//        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
//        clients[1]->write(json);
//    }
//    else if (jsonObject["command"].toInt() == 60001)
//    {
//        QJsonObject jsonObject;
//        jsonObject.insert("request", "endPhase");
//        QJsonDocument jsonDoucment(jsonObject);
//        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
//        clients[1]->write(json);
//    }
    else if (jsonObject["command"].toInt() == 70001)
    {
        //默认对端无响应，直接进入yourPhase（即对方抽卡阶段）
        QJsonObject jsonObject;
        jsonObject.insert("request", "drawPhase");
        write(clients[1], jsonObject);
    }
    else
    {
        clients[1]->sendBinaryMessage(byteArray);
    }
}

void MainWindow::readFromSecondClient(QByteArray byteArray)
{
    QJsonDocument jsonDoucment = QJsonDocument::fromJson(byteArray);
    QJsonObject jsonObject = jsonDoucment.object();
    qDebug() << "readFromSecondClient: " << jsonObject;

    if (jsonObject["command"].toInt() == 666)
    {
        QJsonObject jsonObject;
        jsonObject.insert("request", "askForResponse");
        write(clients[0], jsonObject);
    }
    else if (jsonObject["command"].toInt() == 777)
    {
        QJsonObject jsonObject;
        jsonObject.insert("request", "tellForRequest");
        write(clients[0], jsonObject);
    }
    else if (jsonObject["command"].toInt() == 1000)
    {
        QJsonObject jsonObject;
        jsonObject.insert("request", "setupDeck");
        write(clients[0], jsonObject);
    }
    else if (jsonObject["command"].toInt() == 2000)
    {
        QJsonObject jsonObject;
        jsonObject.insert("request", "startGame");
        write(clients[0], jsonObject);
    }
    else if (jsonObject["command"].toInt() == 3000)
    {
        QJsonObject jsonObject;
        jsonObject.insert("request", "drawPhase");
        write(clients[0], jsonObject);
    }
    else if (jsonObject["command"].toInt() == 20001)
    {
        //默认对端无响应，直接进入standbyPhase（即30000阶段）
        QJsonObject jsonObject;
        jsonObject.insert("request", "standbyPhase");
        write(clients[1], jsonObject);
    }
    else if (jsonObject["command"].toInt() == 30001)
    {
        //默认对端无响应，直接进入main1Phase（即40000阶段）
        QJsonObject jsonObject;
        jsonObject.insert("request", "main1Phase");
        write(clients[1], jsonObject);
    }
    //    else if (jsonObject["command"].toInt() == 40001)
    //    {
    //        QJsonObject jsonObject;
    //        jsonObject.insert("request", "battlePhase");
    //        QJsonDocument jsonDoucment(jsonObject);
    //        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    //        clients[0]->write(json);
    //    }
    //    else if (jsonObject["command"].toInt() == 50001)
    //    {
    //        QJsonObject jsonObject;
    //        jsonObject.insert("request", "main2Phase");
    //        QJsonDocument jsonDoucment(jsonObject);
    //        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    //        clients[0]->write(json);
    //    }
    //    else if (jsonObject["command"].toInt() == 60001)
    //    {
    //        QJsonObject jsonObject;
    //        jsonObject.insert("request", "endPhase");
    //        QJsonDocument jsonDoucment(jsonObject);
    //        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    //        clients[0]->write(json);
    //    }
    else if (jsonObject["command"].toInt() == 70001)
    {
        QJsonObject jsonObject;
        jsonObject.insert("request", "drawPhase");
        write(clients[0], jsonObject);
    }
    else
    {
        clients[0]->sendBinaryMessage(byteArray);
    }
}
