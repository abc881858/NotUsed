#ifndef NET_H
#define NET_H

#include <QWebSocket>
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
    QWebSocket* client;

public slots:
    void readFromServer(QByteArray json);
    void connected();

signals:
    Q_INVOKABLE void request_doAddCard(QJsonObject);
    Q_INVOKABLE void request_doTakeCard(QJsonObject);
    Q_INVOKABLE void request_doSetPhase(QJsonObject);
    Q_INVOKABLE void request_setupDeck();
    Q_INVOKABLE void request_startGame();
    Q_INVOKABLE void request_drawPhase();
    Q_INVOKABLE void request_standbyPhase();
    Q_INVOKABLE void request_main1Phase();
    Q_INVOKABLE void request_doEndOpponentBattlePhase();
    Q_INVOKABLE void request_askForResponse();
    Q_INVOKABLE void request_tellForRequest();
    Q_INVOKABLE void request_CentaurWarrunnerEffect(QJsonObject);
    Q_INVOKABLE void request_KeeperoftheLightEffect(QJsonObject);
    Q_INVOKABLE void request_LionEffect(QJsonObject);
    Q_INVOKABLE void request_MagnusEffect(QJsonObject);
    Q_INVOKABLE void request_NyxAssassinEffect(QJsonObject);
    Q_INVOKABLE void request_RubickEffect(QJsonObject);
    Q_INVOKABLE void request_TuskEffect(QJsonObject);
    Q_INVOKABLE void request_UndyingEffect(QJsonObject);
    Q_INVOKABLE void request_VengefulSpiritEffect(QJsonObject);
    Q_INVOKABLE void request_ZeusEffect(QJsonObject);
};

#endif // NET_H
