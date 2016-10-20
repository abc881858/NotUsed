#ifndef ROOMSCENE_H
#define ROOMSCENE_H

#include <QAction>
#include <QGraphicsScene>
#include <QJsonObject>
#include <QMenu>

#include "area.h"
#include "graphicspixmapobject.h"

class RoomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit RoomScene(QObject* parent = 0);
    Card* enemyTakedCard;
    GraphicsPixmapObject sword[10];
    GraphicsPixmapObject* duifangxingdong;
//    GraphicsPixmapObject currentSword;
    int currentMove;

private:
    //    QMenu* myContextMenu;
    //    QAction* goBP;
    //    QAction* goM2;
    //    QAction* goEP;

signals:
    void hover(QString, QString);

public slots:
    //    void actionBP(bool);
    //    void actionM2(bool);
    //    void actionEP(bool);
    void response_doAddCard(QJsonObject jsonObject);
    void response_doTakeCard(QJsonObject jsonObject);
    void response_doSetPhase(QJsonObject jsonObject);
    void response_doSetDoing(QJsonObject jsonObject);
    void response_setupDeck();
    void response_startGame();
    void response_drawPhase();
    void response_standbyPhase();
    void response_main1Phase();
    void response_doEndOpponentBattlePhase();
    void response_askForResponse();
    void response_tellForRequest();

    void response_CentaurWarrunnerEffect(QJsonObject);

    void doPickTarget();

    void response_Effect(QJsonObject object);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif // ROOMSCENE_H
