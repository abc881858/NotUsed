#include "net.h"
#include <QHostAddress>
#include <QFile>
//#include <functional>

Q_GLOBAL_STATIC(Net, net)

Net* Net::instance()
{
    return net();
}

void Net::initialize()
{
    client = new QTcpSocket(this);
    client->connectToHost(QHostAddress::LocalHost, 7720);
    connect(client, SIGNAL(connected()), this, SLOT(connected()));
    connect(client, SIGNAL(readyRead()), this, SLOT(readFromServer()));
}

void Net::write(QJsonObject jsonObject)
{
    QJsonDocument jsonDoucment(jsonObject);
    QByteArray byteArray = jsonDoucment.toJson(QJsonDocument::Compact); //压缩的json
    client->write(byteArray);
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

void Net::doSetPhase(int phase)
{
    QJsonObject parameter;
    parameter.insert("phase", phase);
    QJsonObject object;
    object.insert("request", "doSetPhase");
    object.insert("parameter", parameter);
    write(object);
}

void Net::connected()
{
    QList<int> myDeck;
    QFile file("test1.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream text_stream(&file);
    while (!text_stream.atEnd())
    {
        int ISDN = text_stream.readLine().toInt();
        myDeck << ISDN;
    }
    file.close();

    emit setupDeck(myDeck);

    QJsonObject jsonObject;
    jsonObject.insert("command", 1000);
    write(jsonObject);
}

void Net::readFromServer()
{
    QByteArray json = client->readAll();
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
        if(parameter.isEmpty())
        {
            QMetaObject::invokeMethod(this, request.toLatin1().data());
        }
        else
        {
            QMetaObject::invokeMethod(this, request.toLatin1().data(), Q_ARG(QJsonObject, parameter));
        }
    }

    //    case 8888:
    //        emit actionCommand(jsonObject["parameter"].toInt(), jsonObject["from"].toInt());
    //        break;
    //    case 0: //说明对方不采取任何行动，我方继续行动
    //        emit getResponse();
}

void Net::sendMessage(int command)
{
    QJsonObject jsonObject;
    jsonObject.insert("command", command);
    write(jsonObject);
}

//void Net::sendMessage(int command, QList<int> list)
//{
//    QJsonObject jsonObject;
//    QJsonArray cards;
//    for (int card; list)
//    {
//        cards << QJsonValue(card);
//    }

//    jsonObject.insert("command", command);
//    jsonObject.insert("cards", cards);
//    write(jsonObject);
//}

//void Net::doNormalSummon(int index)
//{
//    qDebug() << "Net's doNormalSummon: from = " << index;

//    emit actionCommand(99991, index);

//    QJsonObject jsonObject;
//    jsonObject.insert("command", 8888);
//    jsonObject.insert("parameter", 88881);
//    jsonObject.insert("index", index);
//    write(jsonObject);
//}

//void Net::doSetCard(int index)
//{
//    qDebug() << "Net's doSetCard: index = " << index;
//}

//void Net::doEffect(int index)
//{
//    qDebug() << "Net's doEffect: index = " << index;
//}

//void Net::doSpecialSummon(int index)
//{
//    qDebug() << "Net's doSpecialSummon: index = " << index;
//}

//void Net::doTribute(int index)
//{
//    qDebug() << "Net's doTribute: index = " << index;

//    emit actionCommand(99992, index);

//    QJsonObject jsonObject;
//    jsonObject.insert("command", 8888);
//    jsonObject.insert("parameter", 88882);
//    jsonObject.insert("index", index);
//    write(jsonObject);
//}

//void Net::doEndOpponentBattlePhase()
//{
//    //
//}

//void Net::doCentaurWarrunnerEffect2()
//{
//    emit actionCommand(99993, index);

//    QJsonObject jsonObject;
//    jsonObject.insert("command", 8888);
//    jsonObject.insert("parameter", 88883);
//    jsonObject.insert("index", index);
//    write(jsonObject);
//}
