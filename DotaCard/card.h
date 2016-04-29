#ifndef CARD_H
#define CARD_H

#include <QGraphicsObject>
#include <QHash>
#include <QMouseEvent>
#include <QCursor>

class Card : public QGraphicsObject {
    Q_OBJECT
public:
    explicit Card();

    enum CardFlag {
        Effect = 0x1, //可以发动怪兽效果
        FlipSummon = 0x2, //可以翻转召唤
        DefencePosition = 0x4, //可以防守表示
        AttackPosition = 0x8, //可以攻击表示
        NormalSummon = 0x10,
        SetCard = 0x20,
        SpecialSummon = 0x40,
        Attack = 0x80,
        Chain = 0x100
    };
    Q_DECLARE_FLAGS(CardFlags, CardFlag)

    enum {
        NoArea,
        DeckArea,
        HandArea,
        EnemyDeckArea,
        EnemyHandArea,
        FieldyardArea
    };

    CardFlags getCardFlags() const;
    void setCardFlag(CardFlag flag, bool enabled = true);
    void setCardFlags(CardFlags flags);

    int getISDN() const;
    void setISDN(int value);

    QString getName() const;
    void setName(const QString& value);

    QString getImage() const;
    void setImage(const QString& value);

    int getArea() const;
    void setArea(int value);

    QString getDescription() const;
    void setDescription(const QString& value);

    bool getFace() const;
    void setFace(bool value);

    bool getStand() const;
    void setStand(bool value);

    bool getInActive() const;
    void setInActive(bool value);

    QCursor nextCurrent();

    bool testFlipSummon();
    bool testDefencePosition();
    bool testAttackPosition();
    bool testEffect();

    bool getChangePosition() const;
    void setChangePosition(bool value);

protected:
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void hoverEnterEvent(QGraphicsSceneHoverEvent*);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

private:
    QPixmap pixmap;
    int ISDN;
    QString name;
    QString image;
    int area;
    QString description;
    bool face;
    bool stand;
    bool changePosition; //每回合可以变更一次攻防表示

    bool inActive;
    CardFlags myflags; //右键可以显示的全部cursor
    CardFlags currentflag; //当前如果鼠标移上去该显示的cursor

signals:
    void hover(QString);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Card::CardFlags)

///////////////////////////////////////////////

/*!
 * \brief 半人马酋长
 *
 * ①将这张卡作为祭品发动，强制结束对方的战斗阶段。
 * 若这张卡装备了“dota-跳刀”，
 * 则可以改为丢弃一张手牌发动
 *
 * ②你的每回合一次，
 * 你可以让自己场上名字带有“dota”的怪兽全部变为攻击表示或防守表示，
 * 若这张卡装备了“dota-阿哈利姆神杖”时，
 * 同时令自己场上名字带有“dota”的怪兽的攻击力（或防御力）上升自己原本攻击力（或防御力）的一半。
 *
 */
class CentaurWarrunner : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE CentaurWarrunner();
};

/*!
 * \brief 光之守卫
 *
 * ①一回合一次，当有“dota-光之守卫”以外名字带有“dota”的加入手牌时可以发动，
 * 将那只怪兽特殊召唤至场上，若这张卡装备了“dota-阿哈利姆神杖”。
 * 则改此效果为：一回合一次，可以从手牌·卡组特殊召唤一只“dota-光之守卫”以外的名字带有“dota”的怪兽。
 *
 * ②这张卡在手牌存在时，一回合一次，将自己场上一只“dota-光之守卫”以外的名字带有“dota”的怪兽返回手牌。
 *
 * ③一回合一次，选择对方场上的一只怪兽发动，选择的怪兽在进行攻击宣言前必须丢弃一张手牌。
 *
 */
class KeeperoftheLight : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE KeeperoftheLight();
};

/*!
 * \brief 恶魔巫师
 *
 * 一回合一次，选择下面的其一的效果发动：
 * 1、选择对方场上一张怪兽发动，选择的怪兽变为里侧守备表示。
 * 2、选择对方场上一张里侧守备表示的怪兽发动，选择的怪兽破坏。
 * 当这张卡装备了“dota-阿哈利姆神杖”时，同时破坏选择怪兽两边的卡。
 *
 */
class Lion : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE Lion();
};

/*!
 * \brief 半人猛犸
 *
 * ①一回合一次，令自己场上一只名字带有“dota”的怪兽，
 * 在你下回合开始前攻击力上升400点，且在攻击之后，
 * 可以降低600点攻击对对方场上所有怪兽进行一次攻击。
 *
 * ②将这张卡装备的“dota-跳刀”送去墓地，
 * 将对方场上的怪兽全部转为正面攻击表示，攻击力变为0，
 * 且在对方回合结束前不能进行攻击不能更变形式。
 *
 */
class Magnus : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE Magnus();
};

/*!
 * \brief 地穴刺客
 *
 * ①一回合一次，丢弃一张手牌，指定对方场上一只表侧表示的怪兽发动，
 * 对方受到怪兽等级的一半（向下取整且最少为1）*400点伤害，并丢弃一张手牌。
 *
 * ②这张卡有一次不会被破坏，并破坏的种类发动以下效果。
 * 战斗破坏：给与攻击怪兽控制者等同于攻击怪兽攻击的伤害。
 * 效果破坏：破坏那张卡，若那张卡需要指定的怪兽在场上才能发动，则指定的怪兽一起破坏。
 *
 * ③一回合一次，丢弃一张手卡发动，这张卡在下次攻击的伤害计算阶段结束前攻击力上升600，且不会成为攻击和卡的效果对象。
 *
 */
class NyxAssassin : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE NyxAssassin();
};

/*!
 * \brief 大魔导师
 *
 * 这张卡在自己场上表侧表示存在时，自己场上名字带有“dota”的怪兽一回合一次不会被卡的效果破坏。
 * 一回合一次，选择一个效果发动：
 * 1、对方怪兽你的下回合开始前攻击力下降500；
 * 2、你获得对方的一个怪兽效果直到回合结束。当你装备“dota-阿哈利姆神杖”时，
 * 你的1、2效果可以同时发动且你的2效果1回合可以使用2次（覆盖上次获得的效果且该回合不能再次获得同一效果）
 *
 */
class Rubick : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE Rubick();
};

/*!
 * \brief 巨牙海民
 *
 * 这张卡在进行攻击时，可以从手牌表侧攻击特殊召唤一只名字带有“dota”的怪兽，
 * 伤害计算阶段，这张卡的攻击力上升特殊召唤的怪兽攻击力的一半，
 * 这回合，由这个效果特殊召唤而来的怪兽不需要装备“dota-跳刀”便可以发动装备“dota-跳刀”的效果。
 * 一回合一次，这张卡攻击时，若目标怪兽的防御力低于你的攻击力，此次战斗对对方造成的战斗伤害翻倍。
 *
 */
class Tusk : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE Tusk();
};

/*!
 * \brief 不朽尸王
 *
 * 一回合一次，3回合内使对方场上的一只怪兽守备力下降300点，这张卡的守备力上升300点。
 * 一回合一次，你可以弃一张手牌，在自己场上特殊召唤与对方场上怪兽数量相同的“僵尸衍生物”
 * （1星，不死族暗属性，攻500·守0，不能作为祭品、同调及超量素材，
 * 攻击对方守备力低于800的怪兽时，伤害计算阶段攻击力上升500，受到的战斗伤害为0。
 * 每进行一次攻击，伤害计算后被攻击怪兽攻·守下降200至你下回合结束。）
 *
 */
class Undying : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE Undying();
};

/*!
 * \brief 复仇之魂
 *
 * ①当自己场上有“dota-复仇之魂”外的名字带有“dota”怪兽时发动。
 * 你选择一名对方4星以下的怪兽，这张卡和该怪兽的控制权交换直到回合结束，
 * 交换控制权的怪兽不能作为祭品，也不能进行攻击。
 * 若这张卡装备了“dota-阿哈利姆神杖”则可以指定对方任何怪兽且不需要自己场上存在名字带有“dota”的怪兽，
 * 当这张卡的控制权转移给对方时，你可以令这张卡装备的“dota-阿哈利姆神杖”回到手牌。
 *
 * ②只要这张卡在场上，名字带有“dota”的怪兽攻击力上升300。
 *
 * ③一回合一次，你可以指定一名怪兽使其防御力降低800。
 *
 */
class VengefulSpirit : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE VengefulSpirit();
};

/*!
 * \brief 奥林匹斯之王
 *
 * ①当自己场上存在“dota-奥林匹斯之王”以外名字带有“dota”的怪兽时，这张卡可以从手牌特殊召唤。
 *
 * ②一回合一次，丢弃一张手牌发动，破坏对方场上一只怪兽。
 *
 * ③一回合一次，丢弃2张手牌发动，对方场上怪兽守备力降低1000点，因此效果使怪兽守备力降低至0的场合，破坏此怪兽。
 * 若这张卡装备了“dota-阿哈利姆神杖”时，改为降低2000点守备力。
 *
 */
class Zeus : public Card {
    Q_OBJECT
public:
    Q_INVOKABLE Zeus();
};

#endif // CARD_H
