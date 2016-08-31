#ifndef NET_H
#define NET_H

#include <QTcpSocket>

/*!
 * \brief The Net class
 */

class Net : public QObject
{
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
    void doSetCard(int from);
    void doEffect(int from);
    void doSpecialSummon(int from);
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

    //【commmand 8888】 【from、to】
    // 0 卡组，1-20 手牌，100-104 前场，105-109 后场
    // 200 墓地，300 除外区域，400 融合额外区域
    // 1000 对方卡组，1001-1020 对方手牌，1100-1104 对方前场，1105-1109 对方后场
    // 1200 对方墓地，1300 对方除外区域，1400 对方融合额外区域
    void actionCommand(int,int,int);
};

#endif // NET_H
