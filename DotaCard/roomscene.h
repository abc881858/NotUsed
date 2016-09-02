#ifndef ROOMSCENE_H
#define ROOMSCENE_H

#include <QGraphicsScene>
#include "area.h"
#include <QMenu>
#include <QAction>

class RoomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit RoomScene(QObject* parent = 0);
    QList<int> startMyGame();
    QList<int> startYourGame();
    void drawMyPhase();
    void drawYourPhase();
    void endYourPhase();
    void setupDeck(QList<int> list);
    void setupEnemyDeck(QList<int> list);

    void initializeFieldyard();

    void doActionCommand(int parameter, int index);

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

signals:
    void hover(QString);

public slots:
    void actionBP(bool);
    void actionM2(bool);
    void actionEP(bool);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
};

#endif // ROOMSCENE_H
