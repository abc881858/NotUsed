#ifndef NET_H
#define NET_H

#include <QTcpSocket>
#include "card.h"

/*!
 * \brief The Net class
 */

class Net : public QObject {
    Q_OBJECT

public:
    static Net* instance();

    void initialize();
    QByteArray getJsonFromInt(int command);
    void firstSetupEnemyDeck(QJsonArray jsonArray);
    void secondSetupEnemyDeck(QJsonArray jsonArray);

    void sendMessage(int command, QList<int> list);
    void sendMessage(int command);
    
    void doNormalSummon(int from);
private:
    QTcpSocket* client;

public slots:
    void readFromServer();
    void connected();

signals:
    void setupDeck(QList<int> myDeck);
    void setupEnemyDeck(QList<int> yourDeck);
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
