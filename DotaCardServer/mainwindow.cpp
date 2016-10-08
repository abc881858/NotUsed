#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new QTcpServer;
    server->listen(QHostAddress::Any, 7720);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    //  connect(server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(acceptError(QAbstractSocket::SocketError)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newConnection()
{
    qDebug() << "a client socket connected...";
    QTcpSocket* newSock = server->nextPendingConnection();

    if (clients.count() == 0)
    {
        newSock->setObjectName("firstPlayer");
        connect(newSock, SIGNAL(readyRead()), this, SLOT(readFromFirstClient()));
        clients.append(newSock);
        qDebug() << "current clients count: " << clients.count();
    }
    else if (clients.count() == 1)
    {
        newSock->setObjectName("secondPlayer");
        connect(newSock, SIGNAL(readyRead()), this, SLOT(readFromSecondClient()));
        clients.append(newSock);
        qDebug() << "current clients count: " << clients.count();
        qDebug() << "room is full, now we disconnect newConnection slot";
        disconnect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
        qDebug()
            << "the two players are ready, we start the game. First in, first go!";
    }
}

void MainWindow::readFromFirstClient()
{
    QByteArray byteArray = clients[0]->readAll();
    QJsonDocument jsonDoucment = QJsonDocument::fromJson(byteArray);
    QJsonObject jsonObject = jsonDoucment.object();
    qDebug() << jsonObject;

    if (clients.count() < 2)
    {
        qDebug() << "waiting for second player...";
        return;
    }
    if (jsonObject["command"].toInt() == 20000)
    {
        QJsonObject jsonObject;
        jsonObject.insert("request", "startGame");
        QJsonDocument jsonDoucment(jsonObject);
        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
        clients[1]->write(json);
    }
    else if (jsonObject["command"].toInt() == 20001)
    {
        //默认对端无响应，直接进入standbyPhase（即30001阶段）
        QJsonObject jsonObject;
        jsonObject.insert("request", "standbyPhase");
        QJsonDocument jsonDoucment(jsonObject);
        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
        clients[0]->write(json);
    }
    else if (jsonObject["command"].toInt() == 30001)
    {
        //默认对端无响应，直接进入main1Phase（即40001阶段）
        QJsonObject jsonObject;
        jsonObject.insert("request", "main1Phase");
        QJsonDocument jsonDoucment(jsonObject);
        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
        clients[0]->write(json);
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
        QJsonDocument jsonDoucment(jsonObject);
        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
        clients[1]->write(json);
    }
    else
    {
        clients[1]->write(byteArray);
    }
}

void MainWindow::readFromSecondClient()
{
    QByteArray byteArray = clients[1]->readAll();
    QJsonDocument jsonDoucment = QJsonDocument::fromJson(byteArray);
    QJsonObject jsonObject = jsonDoucment.object();
    qDebug() << jsonObject;

    if (jsonObject["command"].toInt() == 1000)
    {
        //  two plays have read deck each other, begin the game!
        //  TODO: may do finger-guessing game Animation later. first player first go Now.
        //  FIXME: will do clients[0]->write(getJsonFromInt(3000));
        QJsonObject jsonObject;
        jsonObject.insert("request", "startGame");
        QJsonDocument jsonDoucment(jsonObject);
        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
        clients[0]->write(json);
    }
    else if (jsonObject["command"].toInt() == 20000)
    {
        QJsonObject jsonObject;
        jsonObject.insert("request", "drawPhase");
        QJsonDocument jsonDoucment(jsonObject);
        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
        clients[0]->write(json);
    }
    else if (jsonObject["command"].toInt() == 20001)
    {
        //默认对端无响应，直接进入standbyPhase（即30000阶段）
        QJsonObject jsonObject;
        jsonObject.insert("request", "standbyPhase");
        QJsonDocument jsonDoucment(jsonObject);
        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
        clients[0]->write(json);
    }
    else if (jsonObject["command"].toInt() == 30001)
    {
        //默认对端无响应，直接进入main1Phase（即40000阶段）
        QJsonObject jsonObject;
        jsonObject.insert("request", "main1Phase");
        QJsonDocument jsonDoucment(jsonObject);
        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
        clients[0]->write(json);
    }
    else if (jsonObject["command"].toInt() == 70001)
    {
        QJsonObject jsonObject;
        jsonObject.insert("request", "drawPhase");
        QJsonDocument jsonDoucment(jsonObject);
        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
        clients[0]->write(json);
    }
    else
    {
        clients[0]->write(byteArray);
    }
}
