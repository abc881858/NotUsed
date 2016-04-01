#ifndef NET_H
#define NET_H

#include <QTcpSocket>
#include "card.h"

class Net : public QObject
{
    Q_OBJECT

public:
    Net(QObject *parent);
    QByteArray getJsonFromInt(int command);
    void firstSetupEnemyDeck(QJsonArray jsonArray);
    void secondSetupEnemyDeck(QJsonArray jsonArray);

    void sendMessage(int command, QList<int> list);
    void sendMessage(int command);
private:
    QTcpSocket* client;

public slots:
    void readFromServer();
    void connected();

signals:
    void loadDeck(QList<Card*> myDeck);
    void setupEnemyDeck(QList<Card*> yourDeck);
    void myStartGame();
    void yourStartGame();
    void myDrawPhase();
    void yourDrawPhase();
    void myStandbyPhase();
    void yourStandbyPhase();
    void myMainPhase1();
    void yourMainPhase1();
    void yourBattlePhase();
    void yourMainPhase2();
    void yourEndPhase();
};

#endif // NET_H
