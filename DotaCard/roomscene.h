#ifndef ROOMSCENE_H
#define ROOMSCENE_H

#include <QGraphicsScene>
#include "area.h"

class RoomScene : public QGraphicsScene {
    Q_OBJECT
public:
    RoomScene(QObject* parent = 0);
    QList<int> startMyGame();
    QList<int> startYourGame();
    void drawMyPhase();
    void drawYourPhase();
    void endYourPhase();
    void setupDeck(QList<int> list);
    void setupEnemyDeck(QList<int> list);

private:
    DeckArea* deckarea;
    HandArea* handarea;
    FieldyardArea* fieldyardarea;
    FieldgroundArea* fieldgroundarea;
    EnemyDeckArea* enemydeckarea;
    EnemyHandArea* enemyhandarea;
    EnemyFieldyardArea* enemyfieldyardarea;
    EnemyFieldgroundArea* enemyfieldgroundarea;

signals:
    void hoverCard(QString);
};

#endif // ROOMSCENE_H
