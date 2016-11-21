#ifndef ROOMSCENE_H
#define ROOMSCENE_H

#include "area.h"
#include "graphicspixmapobject.h"
#include <QAction>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QJsonObject>
#include <QMenu>

class RoomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit RoomScene(QObject* parent = 0);
    Card* enemyTakedCard;
    GraphicsPixmapObject sword[10];
    QGraphicsTextItem word[10];
    GraphicsPixmapObject* duifangxingdong;

private:
    Card* battleSourceCard;
    Card* battleDestinationCard;

signals:
    void hover(QString, QString);
    void addYourLP(int);
    void addMyLP(int);

public slots:
    void response_doAddCard(QJsonObject jsonObject);
    void response_doTakeCard(QJsonObject jsonObject);
    void response_doSetPhase(QJsonObject jsonObject);
    void response_doSetDoing(QJsonObject jsonObject);
    void response_doAddMyLP(QJsonObject jsonObject);
    void response_doAddYourLP(QJsonObject jsonObject);
    void response_setupDeck();
    void response_startGame();
    void response_drawPhase();
    void response_standbyPhase();
    void response_main1Phase();
    void response_askForResponse();
    void response_tellForRequest();
    void doPickTarget();
    void response_Effect(QJsonObject object);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
};

#endif // ROOMSCENE_H
