#ifndef NET_H
#define NET_H

#include <QJsonObject>
#include <QWebSocket>

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
    void doSetDoing(bool doing);
    void doAddMyLP(int value);
    void doAddYourLP(int value);

private:
    QWebSocket* client;

public slots:
    void readFromServer(QByteArray json);
    void connected();

signals:
    Q_INVOKABLE void request_doAddCard(QJsonObject);
    Q_INVOKABLE void request_doTakeCard(QJsonObject);
    Q_INVOKABLE void request_doSetPhase(QJsonObject);
    Q_INVOKABLE void request_doSetDoing(QJsonObject);
    Q_INVOKABLE void request_doAddMyLP(QJsonObject);
    Q_INVOKABLE void request_doAddYourLP(QJsonObject);
    Q_INVOKABLE void request_setupDeck();
    Q_INVOKABLE void request_startGame();
    Q_INVOKABLE void request_drawPhase();
    Q_INVOKABLE void request_standbyPhase();
    Q_INVOKABLE void request_main1Phase();
    Q_INVOKABLE void request_askForResponse();
    Q_INVOKABLE void request_tellForRequest();
    Q_INVOKABLE void request_Effect(QJsonObject);

    Q_INVOKABLE void request_attckStep1(QJsonObject); //攻击宣言
    Q_INVOKABLE void request_attckStep2(QJsonObject); //发动攻击怪兽特效
    Q_INVOKABLE void request_attckStep3(QJsonObject); //翻转被攻击怪兽，发动其特效
    Q_INVOKABLE void request_attckStep4(QJsonObject); //伤害计算流程，计算攻击力防御力
    Q_INVOKABLE void request_attckStep5(QJsonObject); //战败方进入墓地后，发动特效
};

#endif // NET_H
