#include "net.h"
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QUrl>

Q_GLOBAL_STATIC(Net, net)

Net* Net::instance()
{
    return net();
}

void Net::initialize()
{
    client = new QWebSocket;
    client->open(QUrl("ws://localhost:7720"));
    //    client->open(QUrl("ws://139.196.72.104:7720"));
    connect(client, SIGNAL(connected()), this, SLOT(connected()));
    connect(client, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(readFromServer(QByteArray)));
}

void Net::write(QJsonObject jsonObject)
{
    QJsonDocument jsonDoucment(jsonObject);
    QByteArray byteArray = jsonDoucment.toJson(QJsonDocument::Compact); //压缩的json
    client->sendBinaryMessage(byteArray);
    qDebug() << "write: " << jsonObject;
}

void Net::doAddCard(int ISDN, int area, int index, bool face, bool stand) //添加
{
    QJsonObject parameter;
    parameter.insert("ISDN", ISDN);
    parameter.insert("area", area);
    parameter.insert("index", index);
    parameter.insert("face", face);
    parameter.insert("stand", stand);
    QJsonObject object;
    object.insert("request", "doAddCard");
    object.insert("parameter", parameter);
    write(object);
}

void Net::doTakeCard(int area, int index) //删除
{
    QJsonObject parameter;
    parameter.insert("area", area);
    parameter.insert("index", index);
    QJsonObject object;
    object.insert("request", "doTakeCard");
    object.insert("parameter", parameter);
    write(object);
}

void Net::doSetPhase(int phase) //告诉对方我的阶段
{
    QJsonObject parameter;
    parameter.insert("phase", phase);
    QJsonObject object;
    object.insert("request", "doSetPhase");
    object.insert("parameter", parameter);
    write(object);
}

void Net::doSetDoing(bool doing)
{
    QJsonObject parameter;
    parameter.insert("doing", doing);
    QJsonObject object;
    object.insert("request", "doSetDoing");
    object.insert("parameter", parameter);
    write(object);
}

void Net::connected()
{
    QJsonObject jsonObject;
    jsonObject.insert("command", 1000);
    write(jsonObject);
}

void Net::readFromServer(QByteArray json)
{
    QJsonDocument jsonDoucment = QJsonDocument::fromJson(json);
    QJsonObject object = jsonDoucment.object();
    qDebug() << "Net's readFromServer" << object;

    QString request = object["request"].toString();
    if (request.isEmpty())
    {
        return;
    }
    else
    {
        request.prepend("request_");
        QJsonObject parameter = object["parameter"].toObject();
        if (parameter.isEmpty())
        {
            QMetaObject::invokeMethod(this, request.toLatin1().data());
        }
        else
        {
            QMetaObject::invokeMethod(this, request.toLatin1().data(), Q_ARG(QJsonObject, parameter));
        }
    }
}

void Net::sendMessage(int command)
{
    QJsonObject jsonObject;
    jsonObject.insert("command", command);
    write(jsonObject);
}
