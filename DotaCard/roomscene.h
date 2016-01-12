#ifndef ROOMSCENE_H
#define ROOMSCENE_H

#include <QGraphicsScene>
#include "area.h"

class RoomScene : public QGraphicsScene {
    Q_OBJECT
public:
    RoomScene(QObject* parent = 0);
    DeckArea* deckarea;
    HandArea* handarea;
    FieldyardArea* fieldyardarea;
    FieldgroundArea* fieldgroundarea;
    //    GraveyardArea * graveyardarea;
    //    RemoveArea * removearea;
    //    FusionArea * fusionarea;
    //    EnvironmentArea * environmentarea;
    EnemyDeckArea* enemydeckarea;
    EnemyHandArea* enemyhandarea;
    EnemyFieldyardArea* enemyfieldyardarea;
    EnemyFieldgroundArea* enemyfieldgroundarea;
    //    EnemyGraveyardArea * enemygraveyardarea;
    //    EnemyRemoveArea * enemyremovearea;
    //    EnemyFusionArea * enemyfusionarea;
    //    EnemyEnvironmentArea * enemyenvironmentarea;
};

#endif // ROOMSCENE_H
