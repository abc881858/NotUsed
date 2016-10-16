#include "card.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>
#include <QMessageBox>

#include "rule.h"
#include "area.h"
#include "net.h"

Card::Card()
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    //setPixmap(QPixmap(":/png/png/NULL.jpg")); //卡牌会在Deck的AddCard时setPixmap
    area = No_Area;
    type = No_Type;
    setTransformationMode(Qt::SmoothTransformation);
    isRotate = false;
    isBack = true;
}

/////////////////////////// Begin Test All Card Status /////////////////////////////

Card::CardFlags Card::testAll()
{
    testEffect() ? setCardFlag(Effect, true) : setCardFlag(Effect, false);
    testSpecialSummon() ? setCardFlag(SpecialSummon, true) : setCardFlag(SpecialSummon, false);
    testNormalSummon() ? setCardFlag(NormalSummon, true) : setCardFlag(NormalSummon, false);
    testSetCard() ? setCardFlag(SetCard, true) : setCardFlag(SetCard, false);
    testFlipSummon() ? setCardFlag(Card::FlipSummon, true) : setCardFlag(Card::FlipSummon, false);
    testDefencePosition() ? setCardFlag(Card::DefencePosition, true) : setCardFlag(Card::DefencePosition, false);
    testAttackPosition() ? setCardFlag(Card::AttackPosition, true) : setCardFlag(Card::AttackPosition, false);
    testAttack() ? setCardFlag(Card::Attack, true) : setCardFlag(Card::Attack, false);
    testSelectable() ? setCardFlag(Card::Selectable, true) : setCardFlag(Card::Selectable, false);

    return myflags;
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

    //是否在选择卡牌阶段
    if(Rule::instance()->getPicking())
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
        if (FieldyardArea::instance()->testAddCard() == 0)
        {
            return false;
            //QMessageBox::question(0, QString(tr("Can not summon")), QString(tr("Fieldyard is full!")), QMessageBox::Ok);
        }
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

    //是否在选择卡牌阶段
    if(Rule::instance()->getPicking())
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
        if (FieldyardArea::instance()->testAddCard() == 0)
        {
            return false;
            //QMessageBox::question(0, QString(tr("Can not set")), QString(tr("Fieldyard is full!")), QMessageBox::Ok);
        }
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

    //是否在选择卡牌阶段
    if(Rule::instance()->getPicking())
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
    //是否在选择卡牌阶段
    if(Rule::instance()->getPicking())
    {
        return false;
    }
    //    if (face && stand)
    //    {
    //        return changePosition;
    //    }
    return false;
}

bool Card::testAttackPosition()
{
    //是否在选择卡牌阶段
    if(Rule::instance()->getPicking())
    {
        return false;
    }
    //    if (face && !stand)
    //    {
    //        return changePosition;
    //    }
    return false;
}

bool Card::testAttack()
{
    //是否在选择卡牌阶段
    if(Rule::instance()->getPicking())
    {
        return false;
    }
    //    return true;
    return false;
}

bool Card::testSelectable()
{
    int pickRequirement = Rule::instance()->getPickRequirement();

    qDebug() << "Card::testSelectable() pickRequirement: " << pickRequirement;

    if (pickRequirement == KeeperoftheLightRequirement)
    {
        return (area == EnemyFieldyard_Area && face && isMonstor());
    }
    else if (pickRequirement == KeeperoftheLightRequiremented)
    {
        return (area == Fieldyard_Area && face && isMonstor());
    }
    else if (pickRequirement == LionRequirement)
    {
        return (area == EnemyFieldyard_Area && isMonstor());
    }

    return false;
}

/////////////////////////// End Test All Card Status /////////////////////////////

Card::CardFlags Card::getCardFlags() const
{
    return myflags;
}

void Card::setCardFlag(Card::CardFlag flag, bool enabled)
{
    if (enabled)
    {
        myflags |= flag;
    }
    else
    {
        myflags &= ~flag;
    }
}

void Card::setCurrentflag(Card::CardFlag flag)
{
    currentflag = flag;

    qDebug() << "setCurrentflag: " << currentflag;

    QCursor cursorEffect = QCursor(QPixmap(":/png/png/4.cur"), 15, 19);
    QCursor cursorSpecialSummon = QCursor(QPixmap(":/png/png/5.cur"), 8, 26);
    QCursor cursorNormalSummon = QCursor(QPixmap(":/png/png/1.cur"), 13, 19);
    QCursor cursorSetCard = QCursor(QPixmap(":/png/png/2.cur"), 13, 19);
    QCursor cursorFlipSummon = QCursor(QPixmap(":/png/png/1.cur"), 13, 19); //暂时先用1.cur替代
    QCursor cursorDefencePosition = QCursor(QPixmap(":/png/png/6.cur"), 14, 21);
    QCursor cursorAttackPosition = QCursor(QPixmap(":/png/png/8.cur"), 14, 21);
    QCursor cursorAttack = QCursor(QPixmap(":/png/png/7.cur"), 14, 19);

    if (currentflag == Selectable)
    {
        //高亮卡牌
    }
    else if (currentflag == Effect)
    {
        setCursor(cursorEffect);
    }
    else if (currentflag == SpecialSummon)
    {
        setCursor(cursorSpecialSummon);
    }
    else if (currentflag == NormalSummon)
    {
        setCursor(cursorNormalSummon);
    }
    else if (currentflag == SetCard)
    {
        setCursor(cursorSetCard);
    }
    else if (currentflag == FlipSummon)
    {
        setCursor(cursorFlipSummon);
    }
    else if (currentflag == DefencePosition)
    {
        setCursor(cursorDefencePosition);
    }
    else if (currentflag == AttackPosition)
    {
        setCursor(cursorAttackPosition);
    }
    else if (currentflag == Attack)
    {
        setCursor(cursorAttack);
    }
}

void Card::hoverEnterEvent(QGraphicsSceneHoverEvent*)
{
    qDebug() << "hoverEnterEvent: area = " << area;
    emit hover();
    if (area == Hand_Area)
    {
        setY(-35);
    }
    else if (area == EnemyHand_Area)
    {
        setY(35);
    }

    if(testAll() == CardFlags())
    {
        qDebug() << "testAll() == CardFlags()";
        return;
    }

    //TODO: 4角选择框

    if (myflags.testFlag(Selectable)) //测试是否满足选择条件
    {
        setCurrentflag(Selectable);
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
}

void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
{
    setCursor(QCursor(QPixmap(":/png/png/3.cur"), 31, 15));

    if (area == Hand_Area || area == EnemyHand_Area)
    {
        setY(0);
    }
}

// 当我点击任意一张卡牌时
void Card::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if(myflags == CardFlags())
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
        if (currentflag == Selectable)
        {
            activePicked();
        }
        else if (currentflag == Effect)
        {
            if (area == Hand_Area)
            {
                //从手牌发动的特效
                activeHandEffect();
            }
            else if (area == Fieldyard_Area)
            {
                setOneTurnOneEffect(false);
                activeEffect();
            }
        }
        else if (currentflag == NormalSummon)
        {
            Rule::instance()->setOneTurnOneNormalSummon(false);
            emit normalSummon();
        }
        else if (currentflag == SetCard)
        {
            Rule::instance()->setOneTurnOneNormalSummon(false);
            emit setCard();
        }
        else if (currentflag == SpecialSummon)
        {
            //目前没有从手牌特殊召唤的卡牌
            //emit specialSummon();
        }
        else if (currentflag == FlipSummon)
        {
            //目前没有翻转召唤
            //emit flipSummon();
        }
    }
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        setCursor(QCursor(QPixmap(":/png/png/3.cur"), 31, 15));
    }
}

bool Card::getOneTurnHandEffect() const
{
    return oneTurnHandEffect;
}

void Card::setOneTurnHandEffect(bool value)
{
    oneTurnHandEffect = value;
}

int Card::getDebuff() const
{
    return debuff;
}

void Card::setDebuff(int value)
{
    debuff = value;
}

int Card::getType() const
{
    return type;
}

void Card::setType(int value)
{
    type = value;
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

bool Card::getFace() const
{
    return face;
}

void Card::setFace(bool value)
{
    face = value;
}

int Card::getArea() const
{
    return area;
}

void Card::setArea(int value)
{
    area = value;

    if (area==Deck_Area)
    {
        if(!isBack || !isField)
        {
            setPixmap(QPixmap(":/field/NULL.jpg"));
            isBack = true;
            isField = true;
        }
    }
    else if (area==Hand_Area)
    {
        if(isBack || isField)
        {
            //默认没有face==false的卡牌
            setPixmap(QPixmap(QString(":/hand/%1.jpg").arg(name)));
            isBack = false;
            isField = false;
        }
    }
    else if (area==Fieldyard_Area)
    {
        if (face)
        {
            if(isBack || !isField)
            {
                setPixmap(QPixmap(QString(":/field/%1.jpg").arg(name)));
                isBack = false;
                isField = true;
            }
        }
        else
        {
            if(!isBack || !isField)
            {
                setPixmap(QPixmap(":/field/NULL.jpg"));
                isBack = true;
                isField = true;
            }
        }
    }
    else if (area==Fieldground_Area)
    {
        if (face)
        {
            if(isBack || !isField)
            {
                setPixmap(QPixmap(QString(":/field/%1.jpg").arg(name)));
                isBack = false;
                isField = true;
            }
        }
        else
        {
            if(!isBack || !isField)
            {
                setPixmap(QPixmap(":/field/NULL.jpg"));
                isBack = true;
                isField = true;
            }
        }
    }
    else if (area==Graveyard_Area)
    {
        if (face)
        {
            if(isBack || !isField)
            {
                setPixmap(QPixmap(QString(":/field/%1.jpg").arg(name)));
                isBack = false;
                isField = true;
            }
        }
        else
        {
            if(!isBack || !isField)
            {
                setPixmap(QPixmap(":/field/NULL.jpg"));
                isBack = true;
                isField = true;
            }
        }
    }
    else if (area==EnemyDeck_Area)
    {
        if(!isBack || !isField)
        {
            setPixmap(QPixmap(":/field/NULL.jpg"));
            isBack = true;
            isField = true;
        }
    }
    else if (area==EnemyHand_Area)
    {
        if (face)
        {
            if(isBack || isField)
            {
                setPixmap(QPixmap(QString(":/hand/%1.jpg").arg(name)));
                isBack = false;
                isField = false;
            }
        }
        else
        {
            if(!isBack || isField)
            {
                setPixmap(QPixmap(":/hand/NULL.jpg"));
                isBack = true;
                isField = false;
            }
        }
    }
    else if (area==EnemyFieldyard_Area)
    {
        if (face)
        {
            if(isBack || !isField)
            {
                setPixmap(QPixmap(QString(":/field/%1.jpg").arg(name)));
                isBack = false;
                isField = true;
            }
        }
        else
        {
            if(!isBack || !isField)
            {
                setPixmap(QPixmap(":/field/NULL.jpg"));
                isBack = true;
                isField = true;
            }
        }
    }
    else if (area==EnemyFieldground_Area)
    {
        if (face)
        {
            if(isBack || !isField)
            {
                setPixmap(QPixmap(QString(":/field/%1.jpg").arg(name)));
                isBack = false;
                isField = true;
            }
        }
        else
        {
            if(!isBack || !isField)
            {
                setPixmap(QPixmap(":/field/NULL.jpg"));
                isBack = true;
                isField = true;
            }
        }
    }
    else if (area==EnemyGraveyard_Area)
    {
        if (face)
        {
            if(isBack || !isField)
            {
                setPixmap(QPixmap(QString(":/field/%1.jpg").arg(name)));
                isBack = false;
                isField = true;
            }
        }
        else
        {
            if(!isBack || !isField)
            {
                setPixmap(QPixmap(":/field/NULL.jpg"));
                isBack = true;
                isField = true;
            }
        }
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
        if (isRotate)
        {
            if (area==Deck_Area || area==EnemyDeck_Area)
            {
                setTransformOriginPoint(100, 145);
                setRotation(0);
                isRotate = false;
            }
            else
            {
                setTransformOriginPoint(25, 36);
                setRotation(0);
                isRotate = false;
            }
        }
    }
    else
    {
        if (!isRotate)
        {
            if (area==Deck_Area || area==EnemyDeck_Area)
            {
                setTransformOriginPoint(100, 145);
                setRotation(-90);
                isRotate = true;
            }
            else
            {
                setTransformOriginPoint(25, 36);
                setRotation(-90);
                isRotate = true;
            }
        }
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
