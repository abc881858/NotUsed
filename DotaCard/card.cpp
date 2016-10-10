#include "card.h"
#include <QPainter>
#include <QMetaObject>
#include <QGraphicsSceneMouseEvent>
#include "net.h"
#include "rule.h"
#include <QMatrix>

Card::Card()
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    pixmap = QString(":/png/png/NULL.jpg");
    area = NoArea;
    type = NoType;
    effectOnYourBattle = false;
}

/////////////////////////// Begin Test All Card Status /////////////////////////////

void Card::testAll()
{
    testEffect() ? setCardFlag(Effect, true) : setCardFlag(Effect, false);
    testSpecialSummon() ? setCardFlag(SpecialSummon, true) : setCardFlag(SpecialSummon, false);
    testNormalSummon() ? setCardFlag(NormalSummon, true) : setCardFlag(NormalSummon, false);
    testSetCard() ? setCardFlag(SetCard, true) : setCardFlag(SetCard, false);
    testFlipSummon() ? setCardFlag(Card::FlipSummon, true) : setCardFlag(Card::FlipSummon, false);
    testDefencePosition() ? setCardFlag(Card::DefencePosition, true) : setCardFlag(Card::DefencePosition, false);
    testAttackPosition() ? setCardFlag(Card::AttackPosition, true) : setCardFlag(Card::AttackPosition, false);
    testAttack() ? setCardFlag(Card::Attack, true) : setCardFlag(Card::Attack, false);
}

bool Card::testEffect()
{
    if (Rule::instance()->getphase() == Rule::yourBP && Rule::instance()->getIsResponsing())
    {
        if (type == EffectMonster)
        {
            if (effectOnYourBattle && face)
            {
                return true;
            }
        }
    }
    return false;
}

bool Card::testSpecialSummon()
{
    if (area != HandArea)
    {
        return false;
    }
    return false;
}

bool Card::testNormalSummon()
{
    if (area != HandArea)
    {
        return false;
    }
    //TODO: 后续增加被其他卡影响，无法普通召唤的判断
    if (Rule::instance()->getOneTurnOneNormalSummon())
    {
        return true;
    }
    return false;
}

bool Card::testSetCard()
{
    if (area != HandArea)
    {
        return false;
    }
    //TODO: 后续增加被其他卡影响，无法覆盖卡牌的判断
    //包括【怪兽】和【魔陷】的覆盖
    if (Rule::instance()->getOneTurnOneNormalSummon())
    {
        return true;
    }
    return false;
}

bool Card::testFlipSummon()
{
    //    if (!face && !stand) //FIXME: 后续可以去掉stand
    //    {
    //        //TODO: 后续增加被其他卡影响，无法翻转召唤的判断
    //        if (Rule::instance()->getOneTurnOneNormalSummon()) {
    //            return true;
    //        }
    //    }
    return false;
}

bool Card::testDefencePosition()
{
//    if (face && stand)
//    {
//        return changePosition;
//    }
    return false;
}

bool Card::testAttackPosition()
{
//    if (face && !stand)
//    {
//        return changePosition;
//    }
    return false;
}

bool Card::testAttack()
{
//    return true;
    return false;
}

/////////////////////////// End Test All Card Status /////////////////////////////

Card::CardFlags Card::getCardFlags() const
{
    return myflags;
}

void Card::setCardFlag(Card::CardFlag flag, bool enabled)
{
    qDebug() << "setCardFlag" << flag;

    if (enabled)
        myflags |= flag;
    else
        myflags &= ~flag;

    qDebug() << "myflags" << myflags;
}

void Card::setCurrentflag(Card::CardFlag flag)
{
    currentflag = flag;

    qDebug() << "setCurrentflag" << currentflag;

    if (currentflag == Effect)
    {
        setCursor(Qt::SizeHorCursor);
    }
    else if (currentflag == SpecialSummon)
    {
        setCursor(Qt::SizeBDiagCursor);
    }
    else if (currentflag == NormalSummon)
    {
        setCursor(Qt::SizeFDiagCursor);
    }
    else if (currentflag == SetCard)
    {
        setCursor(Qt::SizeAllCursor);
    }
    else if (currentflag == FlipSummon)
    {
        setCursor(Qt::SplitVCursor);
    }
    else if (currentflag == DefencePosition)
    {
        setCursor(Qt::SplitHCursor);
    }
    else if (currentflag == AttackPosition)
    {
        setCursor(Qt::PointingHandCursor);
    }
    else if (currentflag == Attack)
    {
        setCursor(Qt::ForbiddenCursor);
    }
}

QRectF Card::boundingRect() const
{
    return QRectF(0, 0, 100, 145);
}

void Card::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if (area == DeckArea || area == EnemyDeckArea)
    {
        return;
    }

    if (area != EnemyHandArea && face)
    {
        pixmap = QPixmap(image);
    }
    else
    {
        pixmap = QPixmap(":/png/png/NULL.jpg");
    }

    if (area == HandArea || area == EnemyHandArea)
    {
        pixmap = pixmap.scaled(100, 145, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    else
    {
        pixmap = pixmap.scaled(50, 72, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    if (!stand)
    {
        QMatrix matrix;
        matrix.rotate(-90);
        pixmap = pixmap.transformed(matrix);
    }

    painter->drawPixmap(0, 0, pixmap);
}

void Card::hoverEnterEvent(QGraphicsSceneHoverEvent*)
{
    switch (area)
    {
    case HandArea:
        setY(-35);
        break;
    case EnemyHandArea:
        setY(35);
        break;
    default:
        break;
    }

    testAll();

    if (myflags == CardFlags())
    {
        setCursor(Qt::ArrowCursor);
    }
    else if (myflags.testFlag(Effect))
    {
        setCurrentflag(Effect);
    }
    else if (myflags.testFlag(SpecialSummon))
    {
        setCurrentflag(SpecialSummon);
    }
    else if (myflags.testFlag(NormalSummon))
    {
        setCurrentflag(NormalSummon);
    }
    else if (myflags.testFlag(SetCard))
    {
        setCurrentflag(SetCard);
    }

    emit hover();
}

void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
{
    if (area == HandArea || area == EnemyHandArea)
    {
        setY(0);
        setCursor(Qt::ArrowCursor);
    }
}

void Card::nextCursor()
{
    if (currentflag == Effect)
    {
        if (myflags.testFlag(SpecialSummon))
        {
            setCurrentflag(SpecialSummon);
        }
        else if (myflags.testFlag(NormalSummon))
        {
            setCurrentflag(NormalSummon);
        }
        else if (myflags.testFlag(SetCard))
        {
            setCurrentflag(SetCard);
        }
    }
    else if (currentflag == SpecialSummon)
    {
        if (myflags.testFlag(NormalSummon))
        {
            setCurrentflag(NormalSummon);
        }
        else if (myflags.testFlag(SetCard))
        {
            setCurrentflag(SetCard);
        }
        else if (myflags.testFlag(Effect))
        {
            setCurrentflag(Effect);
        }
    }
    else if (currentflag == NormalSummon)
    {
        if (myflags.testFlag(SetCard))
        {
            setCurrentflag(SetCard);
        }
        else if (myflags.testFlag(Effect))
        {
            setCurrentflag(Effect);
        }
        else if (myflags.testFlag(SpecialSummon))
        {
            setCurrentflag(SpecialSummon);
        }
    }
    else if (currentflag == SetCard)
    {
        if (myflags.testFlag(Effect))
        {
            setCurrentflag(Effect);
        }
        else if (myflags.testFlag(SpecialSummon))
        {
            setCurrentflag(SpecialSummon);
        }
        else if (myflags.testFlag(NormalSummon))
        {
            setCurrentflag(NormalSummon);
        }
    }
}

// 当我点击任意一张卡牌时
void Card::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    // 右键点击的话， 切换到下一种鼠标手势， 包括发动效果、特招、普招等等
    if (event->button() == Qt::RightButton)
    {
        nextCursor();
    }
    else if (event->button() == Qt::LeftButton)
    {
        switch (area)
        {
        case HandArea:
            if (currentflag == NormalSummon)
            {
                Rule::instance()->setOneTurnOneNormalSummon(false);
                emit normalSummon();
            }
            else if (currentflag == SetCard)
            {
                Rule::instance()->setOneTurnOneNormalSummon(false);
                emit setCard();
            }
            else if (currentflag == Effect)
            {
                //目前没有从手牌发动的特效
//                emit activeEffect();
            }
            else if (currentflag == SpecialSummon)
            {
                //目前没有从手牌特殊召唤的卡牌
//                emit specialSummon();
            }
            break;
        case FieldyardArea:
            if (currentflag == FlipSummon)
            {
                //目前没有翻转召唤
//                Rule::instance()->setOneTurnOneNormalSummon(false);
//                emit flipSummon();
            }
            else if (currentflag == Effect)
            {
                emit activeEffect();
            }
            break;
        default:
            break;
        }
    }
}

void Card::cardEffect(int)
{
}

int Card::getIndex() const
{
    return index;
}

void Card::setIndex(int value)
{
    index = value;
}

bool Card::getChangePosition() const
{
    return changePosition;
}

void Card::setChangePosition(bool value)
{
    changePosition = value;
}

int Card::getISDN() const
{
    return ISDN;
}

QString Card::getName() const
{
    return name;
}

void Card::setName(const QString& value)
{
    name = value;
}

QString Card::getImage() const
{
    return image;
}

void Card::setImage(const QString& value)
{
    image = value;
}

int Card::getArea() const
{
    return area;
}

void Card::setArea(int value)
{
    area = value;
}

QString Card::getDescription() const
{
    return description;
}

void Card::setDescription(const QString& value)
{
    description = value;
}

bool Card::getFace() const
{
    return face;
}

void Card::setFace(bool value)
{
    face = value;
}

bool Card::getStand() const
{
    return stand;
}

void Card::setStand(bool value)
{
    stand = value;
}

bool Card::getInActive() const
{
    return inActive;
}

void Card::setInActive(bool value)
{
    inActive = value;
}

///////////////////////////////////////////////////////////

CentaurWarrunner::CentaurWarrunner() //半人马酋长
{
    ISDN = 601;
    name = "dota-CentaurWarrunner";
    image = ":/pic/monster/dota-CentaurWarrunner.jpg";
    type = EffectMonster;
    ATK = 1350;
    DEF = 1800;
    level = 3;
    attribute = Earth;

    effectOnYourBattle = true;
}

void CentaurWarrunner::cardEffect(int i)
{
    /// ①将这张卡作为祭品发动，强制结束对方的战斗阶段
    /// 若这张卡装备了“dota-跳刀”TODO: 跳刀未开发，暂时先不判断是否装备跳刀
    /// 则可以改为丢弃一张手牌发动

    if (i == 1)
    {
        if (Rule::instance()->getphase() == Rule::yourBP && Rule::instance()->getIsResponsing() && getFace())
        {
            Net::instance()->doTakeCard(getArea(), index);
            Net::instance()->doAddCard(ISDN, 5, index, true, true);
            Net::instance()->doEndOpponentBattlePhase();
        }
    }

    /// ②你的每回合一次，
    /// 你可以让自己场上名字带有“dota”的怪兽全部变为攻击表示或防守表示，
    /// 若这张卡装备了“dota-阿哈利姆神杖”时 TODO: 阿哈利姆神杖未开发，暂时先不判断是否装备阿哈利姆神杖
    /// 同时令自己场上名字带有“dota”的怪兽的攻击力（或防御力）上升自己原本攻击力（或防御力）的一半。

    if (i == 2)
    {
        //        Net::instance()->doCentaurWarrunnerEffect2();
    }
}

KeeperoftheLight::KeeperoftheLight() //光之守卫
{
    ISDN = 602;
    name = "dota-KeeperoftheLight";
    image = ":/pic/monster/dota-KeeperoftheLight.jpg";
    type = EffectMonster;
    ATK = 800;
    DEF = 300;
    level = 3;
}

Lion::Lion() //恶魔巫师
{
    ISDN = 603;
    name = "dota-Lion";
    image = ":/pic/monster/dota-Lion.jpg";
    type = EffectMonster;
    ATK = 1300;
    DEF = 700;
    level = 4;
}

Magnus::Magnus() //半人猛犸
{
    ISDN = 604;
    name = "dota-Magnus";
    image = ":/pic/monster/dota-Magnus.jpg";
    type = EffectMonster;
    ATK = 1700;
    DEF = 700;
    level = 4;
}

NyxAssassin::NyxAssassin() //地穴刺客
{
    ISDN = 605;
    name = "dota-NyxAssassin";
    image = ":/pic/monster/dota-NyxAssassin.jpg";
    type = EffectMonster;
    ATK = 1500;
    DEF = 800;
    level = 4;
}

Rubick::Rubick() //大魔导师
{
    ISDN = 606;
    name = "dota-Rubick";
    image = ":/pic/monster/dota-Rubick.jpg";
    type = EffectMonster;
    ATK = 900;
    DEF = 300;
    level = 4;
}

Tusk::Tusk() //巨牙海民
{
    ISDN = 607;
    name = "dota-Tusk";
    image = ":/pic/monster/dota-Tusk.jpg";
    type = EffectMonster;
    ATK = 1800;
    DEF = 1000;
    level = 4;
}

Undying::Undying() //不朽尸王
{
    ISDN = 608;
    name = "dota-Undying";
    image = ":/pic/monster/dota-Undying.jpg";
    type = EffectMonster;
    ATK = 1100;
    DEF = 1300;
    level = 3;
}

VengefulSpirit::VengefulSpirit() //复仇之魂
{
    ISDN = 609;
    name = "dota-VengefulSpirit";
    image = ":/pic/monster/dota-VengefulSpirit.jpg";
    type = EffectMonster;
    ATK = 1200;
    DEF = 500;
    level = 3;
}

Zeus::Zeus() //奥林匹斯之王
{
    ISDN = 610;
    name = "dota-Zeus";
    image = ":/pic/monster/dota-Zeus.jpg";
    type = EffectMonster;
    ATK = 500;
    DEF = 350;
    level = 2;
}
