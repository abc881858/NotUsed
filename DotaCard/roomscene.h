#ifndef ROOMSCENE_H
#define ROOMSCENE_H

#include <QGraphicsScene>
#include "area.h"
#include <QMenu>
#include <QAction>
#include <QJsonObject>

class RoomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit RoomScene(QObject* parent = 0);
    Card* enemyTakedCard;

private:
    DeckArea* deckarea;
    HandArea* handarea;
    FieldyardArea* fieldyardarea;
    FieldgroundArea* fieldgroundarea;
    GraveyardArea* graveyardarea;
    EnemyDeckArea* enemydeckarea;
    EnemyHandArea* enemyhandarea;
    EnemyFieldyardArea* enemyfieldyardarea;
    EnemyFieldgroundArea* enemyfieldgroundarea;
    EnemyGraveyardArea* enemygraveyardarea;
    QMenu* myContextMenu;
    QAction* goBP;
    QAction* goM2;
    QAction* goEP;

    //protected:
    //    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

signals:
    void hover(QString, QString);

public slots:
    void actionBP(bool);
    void actionM2(bool);
    void actionEP(bool);
    void response_doAddCard(QJsonObject jsonObject);
    void response_doTakeCard(QJsonObject jsonObject);
    void response_doSetPhase(QJsonObject jsonObject);
    void response_setupDeck();
    void response_startGame();
    void response_drawPhase();
    void response_standbyPhase();
    void response_main1Phase();
    void response_doEndOpponentBattlePhase();
    void response_askForResponse();

    void CentaurWarrunnerEffect();
    void KeeperoftheLightEffect();
    void LionEffect();
    void MagnusEffect();
    void NyxAssassinEffect();
    void RubickEffect();
    void TuskEffect();
    void UndyingEffect();
    void VengefulSpiritEffect();
    void ZeusEffect();

    void response_doCentaurWarrunnerEffect(QJsonObject);
    void response_doKeeperoftheLightEffect(QJsonObject);
    void response_doLionEffect(QJsonObject);
    void response_doMagnusEffect(QJsonObject);
    void response_doNyxAssassinEffect(QJsonObject);
    void response_doRubickEffect(QJsonObject);
    void response_doTuskEffect(QJsonObject);
    void response_doUndyingEffect(QJsonObject);
    void response_doVengefulSpiritEffect(QJsonObject);
    void response_doZeusEffect(QJsonObject);
};

#endif // ROOMSCENE_H
