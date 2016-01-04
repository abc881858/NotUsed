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
    //    connect(server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(acceptError(QAbstractSocket::SocketError)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newConnection()
{
    qDebug() << "a client socket connected...";
    QTcpSocket* newSock = server->nextPendingConnection();

    if (clients.count() == 0) {
        newSock->setObjectName("firstPlayer");
        connect(newSock, SIGNAL(readyRead()), this, SLOT(readFromFirstClient()));
        clients.append(newSock);
        qDebug() << "current clients count: " << clients.count();
    }
    else if (clients.count() == 1) {
        newSock->setObjectName("secondPlayer");
        connect(newSock, SIGNAL(readyRead()), this, SLOT(readFromSecondClient()));
        clients.append(newSock);
        qDebug() << "current clients count: " << clients.count();
        qDebug() << "room is full, now we disconnect newConnection slot";
        disconnect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
        qDebug() << "the two players are ready, we start the game. First in, first go!";
    }
}

void MainWindow::readFromFirstClient()
{
    QByteArray byteArray = clients[0]->readAll();
    QJsonDocument jsonDoucment = QJsonDocument::fromJson(byteArray);
    QJsonObject jsonObject = jsonDoucment.object();
    qDebug() << jsonObject;

    if (jsonObject["command"].toInt() == 1000) {
        deck1 = jsonObject["cards"].toArray();
    }
    else if (clients.count() == 2) {
        clients[1]->write(byteArray);
    }
    else {
        qDebug() << "waiting for second player...";
    }
}

void MainWindow::readFromSecondClient()
{
    QByteArray byteArray = clients[1]->readAll();
    QJsonDocument jsonDoucment = QJsonDocument::fromJson(byteArray);
    QJsonObject jsonObject = jsonDoucment.object();
    qDebug() << jsonObject;

    if (jsonObject["command"].toInt() == 1000) {
        deck2 = jsonObject["cards"].toArray();

        QJsonObject jsonObject;
        jsonObject.insert("command", 2000);
        jsonObject.insert("deck", deck2);
        QJsonDocument jsonDoucment(jsonObject);
        QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
        clients[0]->write(json);

        QJsonObject jsonObject2;
        jsonObject2.insert("command", 3000);
        jsonObject2.insert("deck", deck1);
        QJsonDocument jsonDoucment2(jsonObject2);
        QByteArray json2 = jsonDoucment2.toJson(QJsonDocument::Compact);
        clients[1]->write(json2);
    }
    else if (jsonObject["command"].toInt() == 1001) {
        //two plays have read deck each other, begin the game!
        startGame();
    }
    else {
        clients[0]->write(byteArray);
    }
}

void MainWindow::startGame()
{
    //  TODO: may do finger-guessing game Animation later. first player first go Now.
    //  FIXME: will do clients[0]->write(getJsonFromInt(3000));
    clients[0]->write(getJsonFromInt(10000));
}

QByteArray MainWindow::getJsonFromInt(int command)
{
    QJsonObject jsonObject;
    jsonObject.insert("command", command);
    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    return json;
}
