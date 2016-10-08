#ifndef NET_H
#define NET_H

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

class Net : public QObject
{
    Q_OBJECT

public:
    static Net* instance();

    void initialize();
    void write(QJsonObject jsonObject);
    void doAddCard(int ISDN, int area, int index, bool face, bool stand);
    void doTakeCard(int area, int index);

    void sendMessage(int command);
    void doSetPhase(int phase);
private:
    QTcpSocket* client;

public slots:
    void readFromServer();
    void connected();

signals:
    void setupDeck(QList<int>);

    Q_INVOKABLE void request_doAddCard(QJsonObject);
    Q_INVOKABLE void request_doTakeCard(QJsonObject);
    Q_INVOKABLE void request_doSetPhase(QJsonObject);
    Q_INVOKABLE void request_startGame();
    Q_INVOKABLE void request_drawPhase();
    Q_INVOKABLE void request_standbyPhase();
    Q_INVOKABLE void request_main1Phase();
    Q_INVOKABLE void request_battlePhase();
    Q_INVOKABLE void request_main2Phase();
    Q_INVOKABLE void request_endPhase();
};

#endif // NET_H
