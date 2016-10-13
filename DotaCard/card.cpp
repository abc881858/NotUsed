#include "card.h"
#include "rule.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>

Card::Card()
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setPixmap(QPixmap(":/png/png/NULL.jpg"));
    area = No_Area;
    type = No_Type;
    setTransformationMode(Qt::SmoothTransformation);
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

bool Card::testSpecialSummon()
{
    if (area != Hand_Area)
    {
        return false;
    }
    return false;
}

bool Card::testNormalSummon()
{
    if (area != Hand_Area)
    {
        return false;
    }

    if (!Rule::instance()->getDoing() || !face)
    {
        return false;
    }

    //一回合只能普通召唤一次
    //TODO: 后续增加被其他卡影响，无法普通召唤的判断
    if (!Rule::instance()->getOneTurnOneNormalSummon())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (phase == Rule::myM1 || phase == Rule::myM2)
    {
        return true;
    }

    return false;
}

bool Card::testSetCard()
{
    if (area != Hand_Area)
    {
        return false;
    }

    if (!Rule::instance()->getDoing() || !face)
    {
        return false;
    }

    if (!Rule::instance()->getOneTurnOneNormalSummon())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (phase == Rule::myM1 || phase == Rule::myM2)
    {
        return true;
    }

    return false;
}

bool Card::testFlipSummon()
{
    if (area != Fieldyard_Area)
    {
        return false;
    }

    //TODO: 后续增加被其他卡影响，无法翻转召唤的判断
    //还要添加一个关键的，刚set的卡牌无法当回合flipSummon
    if (!Rule::instance()->getDoing() || face)
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (phase == Rule::myM1 || phase == Rule::myM2)
    {
        return true;
    }

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
    qDebug() << "setCardFlag: " << flag;

    if (enabled)
    {
        myflags |= flag;
    }
    else
    {
        myflags &= ~flag;
    }

    qDebug() << "myflags: " << myflags;
}

void Card::setCurrentflag(Card::CardFlag flag)
{
    currentflag = flag;

    qDebug() << "setCurrentflag: " << currentflag;

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

void Card::hoverEnterEvent(QGraphicsSceneHoverEvent*)
{
    qDebug() << "hoverEnterEvent: area = " << area;
    switch (area)
    {
    case Hand_Area:
        setY(-35);
        break;
    case EnemyHand_Area:
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

    qDebug() << "hover: " << getName();
    emit hover();
}

void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
{
    if (area == Hand_Area || area == EnemyHand_Area)
    {
        setY(0);
        setCursor(Qt::ArrowCursor);
    }
}

// 当我点击任意一张卡牌时
void Card::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (myflags == CardFlags())
    {
        return;
    }
    // 右键点击的话， 切换到下一种鼠标手势， 包括发动效果、特招、普招等等
    if (event->button() == Qt::RightButton)
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
    else if (event->button() == Qt::LeftButton)
    {
        switch (area)
        {
        case Hand_Area:
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
                //目前没有从手牌发动的特效，已经有了
                //                activeEffectFromHand();
            }
            else if (currentflag == SpecialSummon)
            {
                //目前没有从手牌特殊召唤的卡牌
                //emit specialSummon();
            }
            break;
        case Fieldyard_Area:
            if (currentflag == FlipSummon)
            {
                //目前没有翻转召唤
                //emit flipSummon();
            }
            else if (currentflag == Effect)
            {
                setOneTurnOneEffect(false);
                emit activeEffect();
            }
            break;
        default:
            break;
        }
    }
}

int Card::getIndex() const
{
    return index;
}

void Card::setIndex(int value)
{
    index = value;
}

QString Card::getDescription() const
{
    return description;
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

int Card::getArea() const
{
    return area;
}

void Card::setArea(int value)
{
    area = value;
    int width = pixmap().width();
    int height = pixmap().height();

    switch (area)
    {
    case Deck_Area:
        setTransform(QTransform::fromScale(50.0 / width, 72.0 / height), false);
        break;
    case Hand_Area:
        setTransform(QTransform::fromScale(100.0 / width, 145.0 / height), false);
        break;
    case Fieldyard_Area:
        setTransform(QTransform::fromScale(50.0 / width, 72.0 / height), false);
        break;
    case Fieldground_Area:
        setTransform(QTransform::fromScale(50.0 / width, 72.0 / height), false);
        break;
    case Graveyard_Area:
        setTransform(QTransform::fromScale(50.0 / width, 72.0 / height), false);
        break;
    case EnemyDeck_Area:
        setTransform(QTransform::fromScale(50.0 / width, 72.0 / height), false);
        break;
    case EnemyHand_Area:
        setTransform(QTransform::fromScale(100.0 / width, 145.0 / height), false);
        break;
    case EnemyFieldyard_Area:
        setTransform(QTransform::fromScale(50.0 / width, 72.0 / height), false);
        break;
    case EnemyFieldground_Area:
        setTransform(QTransform::fromScale(50.0 / width, 72.0 / height), false);
        break;
    case EnemyGraveyard_Area:
        setTransform(QTransform::fromScale(50.0 / width, 72.0 / height), false);
        break;
    default:
        break;
    }
}

bool Card::getFace() const
{
    return face;
}

void Card::setFace(bool value)
{
    face = value;
    if (face)
    {
        setPixmap(QPixmap(QString(":/pic/monster/%1.jpg").arg(name)));
    }
    else
    {
        setPixmap(QPixmap(":/png/png/NULL.jpg"));
    }
}

bool Card::getStand() const
{
    return stand;
}

void Card::setStand(bool value)
{
    stand = value;
    if (stand)
    {
        //
    }
    else
    {
        if (face)
        {
            setTransformOriginPoint(290, 415);
        }
        else
        {
            setTransformOriginPoint(100, 145);
        }
        setRotation(-90);
    }
}

bool Card::getOneTurnOneEffect() const
{
    return oneTurnOneEffect;
}

void Card::setOneTurnOneEffect(bool value)
{
    oneTurnOneEffect = value;
}
