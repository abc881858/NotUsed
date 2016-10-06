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

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

signals:
    void hover(QString);

public slots:
    void actionBP(bool);
    void actionM2(bool);
    void actionEP(bool);

    void setupDeck(QList<int> list);
    void doAddCard(QJsonObject jsonObject);
    void doTakeCard(QJsonObject jsonObject);
    void setupEnemyDeck(QList<int> list);
    void startMyGame();
};

#endif // ROOMSCENE_H
