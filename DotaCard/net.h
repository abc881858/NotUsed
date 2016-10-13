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
    void doCentaurWarrunnerEffect(bool all);

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
    Q_INVOKABLE void request_doCentaurWarrunnerEffect(QJsonObject);
    Q_INVOKABLE void request_doKeeperoftheLightEffect(QJsonObject);
    Q_INVOKABLE void request_doLionEffect(QJsonObject);
    Q_INVOKABLE void request_doMagnusEffect(QJsonObject);
    Q_INVOKABLE void request_doNyxAssassinEffect(QJsonObject);
    Q_INVOKABLE void request_doRubickEffect(QJsonObject);
    Q_INVOKABLE void request_doTuskEffect(QJsonObject);
    Q_INVOKABLE void request_doUndyingEffect(QJsonObject);
    Q_INVOKABLE void request_doVengefulSpiritEffect(QJsonObject);
    Q_INVOKABLE void request_doZeusEffect(QJsonObject);
};

#endif // NET_H
