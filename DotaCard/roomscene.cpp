#include "roomscene.h"

static const QPointF DeckPos(485, 426);
static const QPointF HandPos(19, 529);
static const QPointF FieldyardPos(94, 317);
static const QPointF FieldgroundPos(94, 424);
//static const QPointF GraveyardPos(485,331);
//static const QPointF RemovePos(479,296);
//static const QPointF FusionPos(12,424);
//static const QPointF EnvironmentPos(12,319);
static const QPointF EnemyDeckPos(14, 105);
static const QPointF EnemyHandPos(17, -71);
static const QPointF EnemyFieldyardPos(94, 213);
static const QPointF EnemyFieldgroundPos(94, 105);
//static const QPointF EnemyGraveyardPos(262,196);
//static const QPointF EnemyRemovePos(253,280);
//static const QPointF EnemyFusionPos(742,105);
//static const QPointF EnemyEnvironmentPos(742,209);

RoomScene::RoomScene(QObject* parent)
    : QGraphicsScene(parent)
{
    setBackgroundBrush(QBrush(QPixmap(":/png/png/b.png")));

    // create DeckArea
    deckarea = new DeckArea;
    addItem(deckarea);
    deckarea->setPos(DeckPos);
    // create HandArea
    handarea = new HandArea;
    addItem(handarea);
    handarea->setPos(HandPos);
    // create FieldyardArea
    fieldyardarea = new FieldyardArea;
    addItem(fieldyardarea);
    fieldyardarea->setPos(FieldyardPos);
    // create FieldgroundArea
    fieldgroundarea = new FieldgroundArea;
    addItem(fieldgroundarea);
    fieldgroundarea->setPos(FieldgroundPos);
    //    // create GraveyardArea
    //    graveyardarea = new GraveyardArea;
    //    addItem(graveyardarea);
    //    graveyardarea->setPos(GraveyardPos);
    //    // create RemoveArea
    //    removearea = new RemoveArea;
    //    addItem(removearea);
    //    removearea->setPos(RemovePos);
    //    // create FusionArea
    //    fusionarea = new FusionArea;
    //    addItem(fusionarea);
    //    fusionarea->setPos(FusionPos);
    //    // create EnvironmentArea
    //    environmentarea = new EnvironmentArea;
    //    addItem(environmentarea);
    //    environmentarea->setPos(EnvironmentPos);

    // create Enemy
    enemydeckarea = new EnemyDeckArea;
    addItem(enemydeckarea);
    enemydeckarea->setPos(EnemyDeckPos);
    enemyhandarea = new EnemyHandArea;
    addItem(enemyhandarea);
    enemyhandarea->setPos(EnemyHandPos);
    enemyfieldyardarea = new EnemyFieldyardArea;
    addItem(enemyfieldyardarea);
    enemyfieldyardarea->setPos(EnemyFieldyardPos);
    enemyfieldgroundarea = new EnemyFieldgroundArea;
    addItem(enemyfieldgroundarea);
    enemyfieldgroundarea->setPos(EnemyFieldgroundPos);
    //    enemygraveyardarea = new EnemyGraveyardArea;
    //    addItem(enemygraveyardarea);
    //    enemygraveyardarea->setPos(EnemyGraveyardPos);
    //    enemyremovearea = new EnemyRemoveArea;
    //    addItem(enemyremovearea);
    //    enemyremovearea->setPos(EnemyRemovePos);
    //    enemyfusionarea = new EnemyFusionArea;
    //    addItem(enemyfusionarea);
    //    enemyfusionarea->setPos(EnemyFusionPos);
    //    enemyenvironmentarea = new EnemyEnvironmentArea;
    //    addItem(enemyenvironmentarea);
    //    enemyenvironmentarea->setPos(EnemyEnvironmentPos);
}
