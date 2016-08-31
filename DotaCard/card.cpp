#include "card.h"
#include <QPainter>
#include <QMetaObject>
#include <QGraphicsSceneMouseEvent>
#include "net.h"
#include "rule.h"

Card::Card()
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    pixmap = QString(":/png/png/NULL.jpg");
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
    testAttackPosition() ? setCardFlag(Card::AttackPosition, true) : setCardFlag(Card::DefencePosition, false);
    testAttack() ? setCardFlag(Card::Attack, true) : setCardFlag(Card::Attack, false);
}

bool Card::testEffect()
{
    return false;
}

bool Card::testSpecialSummon()
{
    if(area!=HandArea)
    {
        return false;
    }
    return false;
}

bool Card::testNormalSummon()
{
    if(area!=HandArea)
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
    if(area!=HandArea)
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
    if (face && stand)
    {
        return changePosition;
    }
    return false;
}

bool Card::testAttackPosition()
{
    if (face && !stand)
    {
        return changePosition;
    }
    return false;
}

bool Card::testAttack()
{
    return true;
}

/////////////////////////// End Test All Card Status /////////////////////////////

Card::CardFlags Card::getCardFlags() const
{
    return myflags;
}

void Card::setCardFlag(Card::CardFlag flag, bool enabled)
{
    if (enabled)
        setCardFlags(myflags | flag);
    else
        setCardFlags(myflags & ~flag);
}

void Card::setCardFlags(CardFlags flags)
{
    if (myflags == flags)
        return;

    myflags = flags;
}

void Card::setCurrentflag(Card::CardFlag flag)
{
    currentflag = flag;

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
    if (area != EnemyHandArea)
    {
        pixmap = QPixmap(image);
    }
    if (area == HandArea || area == EnemyHandArea)
    {
        pixmap = pixmap.scaled(100, 145, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    else
    {
        pixmap = pixmap.scaled(50, 72, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
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

void Card::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
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
                Net::instance()->doNormalSummon(index);
            }
            else if (currentflag == SetCard)
            {
                Rule::instance()->setOneTurnOneNormalSummon(false);
                Net::instance()->doSetCard(index);
            }
            else if (currentflag == Effect)
            {
                Net::instance()->doEffect(index);
            }
            else if (currentflag == SpecialSummon)
            {
                Net::instance()->doSpecialSummon(index);
            }
            break;
        case FieldyardArea:
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

void Card::setISDN(int value)
{
    ISDN = value;
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
    setISDN(601);
    setName("dota-CentaurWarrunner");
    setImage(":/pic/monster/dota-CentaurWarrunner.jpg");
}

KeeperoftheLight::KeeperoftheLight() //光之守卫
{
    setISDN(602);
    setName("dota-KeeperoftheLight");
    setImage(":/pic/monster/dota-KeeperoftheLight.jpg");
}

Lion::Lion() //恶魔巫师
{
    setISDN(603);
    setName("dota-Lion");
    setImage(":/pic/monster/dota-Lion.jpg");
}

Magnus::Magnus() //半人猛犸
{
    setISDN(604);
    setName("dota-Magnus");
    setImage(":/pic/monster/dota-Magnus.jpg");
}

NyxAssassin::NyxAssassin() //地穴刺客
{
    setISDN(605);
    setName("dota-NyxAssassin");
    setImage(":/pic/monster/dota-NyxAssassin.jpg");
}

Rubick::Rubick() //大魔导师
{
    setISDN(606);
    setName("dota-Rubick");
    setImage(":/pic/monster/dota-Rubick.jpg");
}

Tusk::Tusk() //巨牙海民
{
    setISDN(607);
    setName("dota-Tusk");
    setImage(":/pic/monster/dota-Tusk.jpg");
}

Undying::Undying() //不朽尸王
{
    setISDN(608);
    setName("dota-Undying");
    setImage(":/pic/monster/dota-Undying.jpg");
}

VengefulSpirit::VengefulSpirit() //复仇之魂
{
    setISDN(609);
    setName("dota-VengefulSpirit");
    setImage(":/pic/monster/dota-VengefulSpirit.jpg");
}

Zeus::Zeus() //奥林匹斯之王
{
    setISDN(610);
    setName("dota-Zeus");
    setImage(":/pic/monster/dota-Zeus.jpg");
}
