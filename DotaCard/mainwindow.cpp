#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new QTcpSocket(this);
    client->connectToHost(QHostAddress::LocalHost, 7720);
    connect(client, SIGNAL(connected()), this, SLOT(connected()));
    connect(client, SIGNAL(readyRead()), this, SLOT(readFromServer()));
    //  connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    //  connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    myLP = 8000;
    yourLP = 8000;
    ui->myLcdNumber->display(myLP);
    ui->yourLcdNumber->display(yourLP);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPhase(MainWindow::Phase phase)
{
    this->phase = phase;
}

MainWindow::Phase MainWindow::getphase() const
{
    return this->phase;
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

    QFile file("test1.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream text_stream(&file);
    while (!text_stream.atEnd()) {
        myDeck << new Card(0, text_stream.readLine().toInt());
    }
    file.close();

    QJsonObject jsonObject;
    QJsonArray cards;
    foreach (Card* card, myDeck) {
        cards << card->getISDN();
    }

    jsonObject.insert("command", 1000);
    jsonObject.insert("cards", cards);

    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    client->write(json);
}

void MainWindow::readFromServer()
{
    QByteArray json = client->readAll();
    QJsonDocument jsonDoucment = QJsonDocument::fromJson(json);
    QJsonObject jsonObject = jsonDoucment.object();
    qDebug() << jsonObject;
    int command = jsonObject["command"].toInt();
    switch (command) {
    case 2000:
        firstSetupEnemyDeck(jsonObject["deck"].toArray());
        break;
    case 3000:
        secondSetupEnemyDeck(jsonObject["deck"].toArray());
        break;
    case 10000:
        myStartGame();
        break;
    case 10001:
        yourStartGame();
        break;
    case 10002:
        myDrawPhase();
        break;
    case 20001:
        yourDrawPhase();
        break;
    case 20002:
        myStandbyPhase();
        break;
    case 30001:
        yourStandbyPhase();
        break;
    case 30002:
        myMainPhase1();
        break;
    case 40001:
        yourMainPhase1();
        break;
    case 50001:
        yourBattlePhase();
        break;
    case 60001:
        yourMainPhase2();
        break;
    case 70001:
        yourEndPhase();
        break;
    default:
        break;
    }
}

//only send int(command) to server
QByteArray MainWindow::getJsonFromInt(int command)
{
    QJsonObject jsonObject;
    jsonObject.insert("command", command);
    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    return json;
}

void MainWindow::firstSetupEnemyDeck(QJsonArray jsonArray)
{
    foreach (const QJsonValue& value, jsonArray) {
        int ISDN = value.toInt();
        yourDeck << new Card(0, ISDN);
    }
}

void MainWindow::secondSetupEnemyDeck(QJsonArray jsonArray)
{
    foreach (const QJsonValue& value, jsonArray) {
        int ISDN = value.toInt();
        yourDeck << new Card(0, ISDN);
    }
    client->write(getJsonFromInt(1001));
}

void MainWindow::myStartGame()
{
    QJsonObject jsonObject;
    QJsonArray cards;
    for (int i = 0; i < 5; i++) {
        Card* card = myDeck.takeFirst();
        myHand << card;
        cards << card->getISDN();
    }
    jsonObject.insert("command", 10001);
    jsonObject.insert("cards", cards);
    qDebug() << jsonObject;
    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    client->write(json);
}

void MainWindow::yourStartGame()
{
    for (int i = 0; i < 5; i++) {
        Card* card = yourDeck.takeFirst();
        yourHand << card;
    }

    QJsonObject jsonObject;
    QJsonArray cards;
    for (int i = 0; i < 5; i++) {
        Card* card = myDeck.takeFirst();
        myHand << card;
        cards << card->getISDN();
    }
    jsonObject.insert("command", 10002);
    jsonObject.insert("cards", cards);
    qDebug() << jsonObject;
    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    client->write(json);
}

void MainWindow::myDrawPhase()
{
    for (int i = 0; i < 5; i++) {
        Card* card = yourDeck.takeFirst();
        yourHand << card;
    }
    setPhase(myDP);

    Card* card = myDeck.takeFirst();
    myHand << card;

    client->write(getJsonFromInt(20001));
}

void MainWindow::yourDrawPhase()
{
    Card* card = yourDeck.takeFirst();
    yourHand << card;

    setPhase(yourDP);
    client->write(getJsonFromInt(20002));
}

void MainWindow::myStandbyPhase()
{
    setPhase(mySP);
    client->write(getJsonFromInt(30001));
}

void MainWindow::yourStandbyPhase()
{
    setPhase(yourSP);
    client->write(getJsonFromInt(30002));
}

void MainWindow::myMainPhase1()
{
    setPhase(myM1);
    client->write(getJsonFromInt(40001));
}

void MainWindow::yourMainPhase1()
{
    setPhase(yourM2);
}

void MainWindow::yourBattlePhase()
{
    setPhase(yourBP);
}

void MainWindow::yourMainPhase2()
{
    setPhase(yourM2);
}

void MainWindow::yourEndPhase()
{
    setPhase(yourEP);
    //TODO: 也许对方结束阶段我要触发什么陷阱卡
    //暂时玩家2直接进入他的抽卡阶段

    setPhase(myDP);

    Card* card = myDeck.takeFirst();
    myHand << card;

    client->write(getJsonFromInt(20001));
}

void MainWindow::on_buttonBP_clicked()
{
    setPhase(myBP);
    client->write(getJsonFromInt(50001));
}

void MainWindow::on_buttonM2_clicked()
{
    setPhase(myM2);
    client->write(getJsonFromInt(60001));
}

void MainWindow::on_buttonEP_clicked()
{
    setPhase(myEP);
    client->write(getJsonFromInt(70001));
}
