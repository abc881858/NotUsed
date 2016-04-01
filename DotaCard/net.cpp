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
    sendMessage(1001);
}

void Net::sendMessage(int command)
{
    client->write(getJsonFromInt(command));
}

void Net::sendMessage(int command, QList<int> list)
{
    QJsonObject jsonObject;
    QJsonArray cards;
    foreach (int card, list) {
        cards << QJsonValue(card);
    }

    jsonObject.insert("command", command);
    jsonObject.insert("cards", cards);

    QJsonDocument jsonDoucment(jsonObject);
    QByteArray json = jsonDoucment.toJson(QJsonDocument::Compact);
    client->write(json);
}
