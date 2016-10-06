#ifndef NET_H
#define NET_H

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

/*!
 * \brief The Net class
 */

class Net : public QObject
{
    Q_OBJECT

public:
    static Net* instance();

    void initialize();
    void write(QJsonObject jsonObject);
    void doAddCard(int ISDN, int area, int index, bool face, bool stand);
    void doTakeCard(int area, int index);

private:
    QTcpSocket* client;

public slots:
    void readFromServer();
    void connected();

signals:
    void setupDeck(QList<int>);
    void setupEnemyDeck(QList<int>);
    void startMyGame();
    Q_INVOKABLE void request_doAddCard(QJsonObject);
    Q_INVOKABLE void request_doTakeCard(QJsonObject);
};

#endif // NET_H
