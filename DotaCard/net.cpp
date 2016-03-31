#include "net.h"
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

Net::Net(QObject *parent) : QObject(parent)
{
    client = new QTcpSocket(this);
    client->connectToHost(QHostAddress::LocalHost, 7720);
    connect(client, SIGNAL(connected()), this, SLOT(connected()));
    connect(client, SIGNAL(readyRead()), this, SLOT(readFromServer()));
    //  connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    //  connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
}

void Net::connected()
{
    /*
 *  NOTE: this is a example for testing write QJSON to socket server!
 *  QJsonObject jsonObject;
 *  jsonObject.insert("test",QString("just tell server connected"));
 *  qDebug() << jsonObject;
 *  QJsonDocument jsonDoucment(jsonObject);
 *  QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
 *  client->write(json);
 */
    QList<Card*> myDeck;
    QFile file("test1.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream text_stream(&file);
    while (!text_stream.atEnd()) {
        myDeck << new Card(text_stream.readLine().toInt());
    }
    file.close();
    emit loadDeck(myDeck);

    QJsonObject jsonObject;
    QJsonArray cards;
    foreach (Card* card, myDeck) {
        connect(card,SIGNAL(hover(QString)),this,SLOT(setBigImage(QString)));
        cards << card->getISDN();
    }

    jsonObject.insert("command", 1000);
    jsonObject.insert("cards", cards);

    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    client->write(json);
}

void Net::readFromServer()
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
        emit myStartGame();
        break;
    case 10001:
        emit yourStartGame();
        break;
    case 10002:
        emit myDrawPhase();
        break;
    case 20001:
        emit yourDrawPhase();
        break;
    case 20002:
        emit myStandbyPhase();
        break;
    case 30001:
        emit yourStandbyPhase();
        break;
    case 30002:
        emit myMainPhase1();
        break;
    case 40001:
        emit yourMainPhase1();
        break;
    case 50001:
        emit yourBattlePhase();
        break;
    case 60001:
        emit yourMainPhase2();
        break;
    case 70001:
        emit yourEndPhase();
        break;
    default:
        break;
    }
}

//only send int(command) to server
QByteArray Net::getJsonFromInt(int command)
{
    QJsonObject jsonObject;
    jsonObject.insert("command", command);
    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    return json;
}

void Net::firstSetupEnemyDeck(QJsonArray jsonArray)
{
    QList<Card*> yourDeck;
    foreach (const QJsonValue& value, jsonArray) {
        int ISDN = value.toInt();
        yourDeck << new Card(ISDN);
    }
    emit setupEnemyDeck(yourDeck);
}

void Net::secondSetupEnemyDeck(QJsonArray jsonArray)
{
    firstSetupEnemyDeck(jsonArray);
    client->write(getJsonFromInt(1001));
}

//void Net::myStartGame()
//{
//    QJsonObject jsonObject;
//    QJsonArray cards;
//    for (int i = 0; i < 5; i++) {
//        Card* card = myDeck.takeFirst();
//        roomScene->handarea->addCard(card);
//        cards << card->getISDN();
//    }
//    jsonObject.insert("command", 10001);
//    jsonObject.insert("cards", cards);
//    qDebug() << jsonObject;
//    QJsonDocument jsonDoucment(jsonObject);
//    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
//    client->write(json);
//}

//void Net::yourStartGame()
//{
//    for (int i = 0; i < 5; i++) {
//        Card* card = yourDeck.takeFirst();
//        roomScene->enemyhandarea->addCard(card);
//    }

//    QJsonObject jsonObject;
//    QJsonArray cards;
//    for (int i = 0; i < 5; i++) {
//        Card* card = myDeck.takeFirst();
//        roomScene->handarea->addCard(card);
//        cards << card->getISDN();
//    }
//    jsonObject.insert("command", 10002);
//    jsonObject.insert("cards", cards);
//    qDebug() << jsonObject;
//    QJsonDocument jsonDoucment(jsonObject);
//    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
//    client->write(json);
//}

//void Net::myDrawPhase()
//{
//    for (int i = 0; i < 5; i++) {
//        Card* card = yourDeck.takeFirst();
//        roomScene->enemyhandarea->addCard(card);
//    }
//    setPhase(myDP);

//    Card* card = myDeck.takeFirst();
//    roomScene->handarea->addCard(card);

//    client->write(getJsonFromInt(20001));
//}

//void Net::yourDrawPhase()
//{
//    Card* card = yourDeck.takeFirst();
//    roomScene->enemyhandarea->addCard(card);

//    setPhase(yourDP);
//    client->write(getJsonFromInt(20002));
//}

//void Net::myStandbyPhase()
//{
//    setPhase(mySP);
//    client->write(getJsonFromInt(30001));
//}

//void Net::yourStandbyPhase()
//{
//    setPhase(yourSP);
//    client->write(getJsonFromInt(30002));
//}

//void Net::myMainPhase1()
//{
//    setPhase(myM1);
//    client->write(getJsonFromInt(40001));
//}

//void Net::yourMainPhase1()
//{
//    setPhase(yourM2);
//}

//void Net::yourBattlePhase()
//{
//    setPhase(yourBP);
//}

//void Net::yourMainPhase2()
//{
//    setPhase(yourM2);
//}

//void Net::yourEndPhase()
//{
//    setPhase(yourEP);
//    //TODO: 也许对方结束阶段我要触发什么陷阱卡
//    //暂时玩家2直接进入他的抽卡阶段

//    setPhase(myDP);

//    Card* card = myDeck.takeFirst();
//    roomScene->handarea->addCard(card);

//    client->write(getJsonFromInt(20001));
//}
