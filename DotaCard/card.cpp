#include "card.h"
#include "area.h"
#include "net.h"
#include "rule.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QImage>
#include <QColor>
#include <QPixmap>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QImage>

Card::Card()
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    //setPixmap(QPixmap(":/png/png/NULL.jpg")); //卡牌会在Deck的AddCard时setPixmap
    area = No_Area;
    face = false;
    stand = true;
    type = No_Type;
    setTransformationMode(Qt::SmoothTransformation);
    isRotate = false;
    isBack = true;
    //    oneTurnHandEffect = false;
    //    oneTurnOneAttack = false;
    //    oneTurnOneEffect = false;
    buff_602 = false;
    buff_604 = false;

    currentFingerFlag = NoFlag;

    cursorNoFlag = QCursor(QPixmap(":/png/png/3.cur"), 31, 15);
    cursorEffect = QCursor(QPixmap(":/png/png/4.cur"), 15, 19);
    cursorSpecialSummon = QCursor(QPixmap(":/png/png/5.cur"), 8, 26);
    cursorNormalSummon = QCursor(QPixmap(":/png/png/1.cur"), 13, 19);
    cursorSetCard = QCursor(QPixmap(":/png/png/2.cur"), 13, 19);
    cursorFlipSummon = QCursor(QPixmap(":/png/png/1.cur"), 13, 19); //暂时先用1.cur替代
    cursorDefencePosition = QCursor(QPixmap(":/png/png/6.cur"), 14, 21);
    cursorAttackPosition = QCursor(QPixmap(":/png/png/8.cur"), 14, 21);
    cursorAttack = QCursor(QPixmap(":/png/png/7.cur"), 14, 19);
}

void Card::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    if(area==No_Area)
    {
        return;
    }

    QImage frameImage;

    if(area == Hand_Area)
    {
        frameImage = QImage(":/png/png/chooseBlue.png");
    }
    else if(area == EnemyHand_Area)
    {
        frameImage = QImage(":/png/png/chooseRed.png");
    }
    else if(area == Deck_Area
            || area == Fieldyard_Area
            || area == Fieldground_Area
            || area == Graveyard_Area)
    {
        frameImage = QImage(":/png/png/selectBlue.png");
    }
    else if(area == EnemyDeck_Area
            || area == EnemyFieldyard_Area
            || area == EnemyFieldground_Area
            || area == EnemyGraveyard_Area)
    {
        frameImage = QImage(":/png/png/selectRed.png");
    }

    if (option->state & QStyle::State_MouseOver)
    {
        painter->drawPixmap(0, 0, lighterPixmap);
        painter->drawPixmap(-3,-3,QPixmap::fromImage(frameImage));
    }
    else
    {
        painter->drawPixmap(0, 0, originalPixmap);
    }
}

bool Card::testSpecialSummon()
{
    if (area != Hand_Area)
    {
        return false;
    }

    //是否在选择卡牌阶段
    if (Rule::instance()->getPickRequirement())
    {
        return false;
    }

    if (!Rule::instance()->getDoing() || !face)
    {
        return false;
    }

    //TODO: 后续增加被其他卡影响，无法特殊召唤的判断，现在在手牌就可以特招，无次数限制

    Rule::Phase phase = Rule::instance()->getphase();
    if (phase != Rule::myM1 && phase != Rule::myM2)
    {
        return false;
    }

    if (FieldyardArea::instance()->testAddCard() == -1)
    {
        return false;
        //QMessageBox::question(0, QString(tr("Can not summon")), QString(tr("Fieldyard is full!")), QMessageBox::Ok);
    }

    if (ISDN == 610)
    {
        for (Card* card : FieldyardArea::instance()->getMyFieldyard())
        {
            if (card->getName().startsWith("dota"))
            {
                return true;
            }
        }
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
    if (Rule::instance()->getPickRequirement())
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
        if (FieldyardArea::instance()->testAddCard() == -1)
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
    if (Rule::instance()->getPickRequirement())
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
        if (FieldyardArea::instance()->testAddCard() == -1)
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
    if (Rule::instance()->getPickRequirement())
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
    if (Rule::instance()->getPickRequirement())
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
    if (Rule::instance()->getPickRequirement())
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
    if (area != Fieldyard_Area)
    {
        return false;
    }

    if (!Rule::instance()->getDoing() || !face)
    {
        return false;
    }

    if (Rule::instance()->getphase() != Rule::myBP)
    {
        return false;
    }

    //是否在处理某张卡的效果，处于选择target卡牌阶段
    if (Rule::instance()->getPickRequirement() == NoRequirement)
    {
        //保证1回合只能攻击一次，宝剑只出现一次
        if (oneTurnOneAttack) //这个欠考虑，如果战斗流程发动陷阱卡墓地苏生
        {
            return true;
        }
    }
    return false;
}

void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
{
    //高亮卡牌
    setCursor(cursorNoFlag);
//    setPixmap(originalPixmap);
    if (area == Hand_Area || area == EnemyHand_Area)
    {
        setY(0);
    }
    emit hoverLeave();
}

void Card::hoverEnterEvent(QGraphicsSceneHoverEvent*)
{
    if (area == Hand_Area)
    {
        setY(-35);
    }
    else if (area == EnemyHand_Area)
    {
        setY(35);
    }
    emit hoverEnter();

    if (Rule::instance()->getPickRequirement() != NoRequirement)
    {
        return;
    }

    allFingerFlags.clear();
    Rule::Phase phase = Rule::instance()->getphase();
    if (phase == Rule::myM1 || phase == Rule::myM2)
    {
        if (area == Hand_Area)
        {
            if (testEffect())
            {
                allFingerFlags << Effect;
            }
            if (testSpecialSummon())
            {
                allFingerFlags << SpecialSummon;
            }
            if (testNormalSummon())
            {
                allFingerFlags << NormalSummon;
            }
            if (testSetCard())
            {
                allFingerFlags << SetCard;
            }
        }
        else if (area == Fieldyard_Area)
        {
            if (face)
            {
                if (stand)
                {
                    if (testEffect())
                    {
                        allFingerFlags << Effect;
                    }
                    if (testDefencePosition())
                    {
                        allFingerFlags << DefencePosition;
                    }
                }
                else
                {
                    if (testEffect())
                    {
                        allFingerFlags << Effect;
                    }
                    if (testAttackPosition())
                    {
                        allFingerFlags << AttackPosition;
                    }
                }
            }
            else
            {
                if (testFlipSummon())
                {
                    allFingerFlags << FlipSummon;
                }
            }
        }
        else if (area == Fieldground_Area)
        {
            //
        }
    }
    else if (phase == Rule::myBP)
    {
        if (area == Fieldyard_Area)
        {
            if (testAttack())
            {
                allFingerFlags << Attack;
            }
        }
        else if (area == Fieldground_Area)
        {
            //
        }
    }

    if (allFingerFlags.isEmpty())
    {
        currentFingerFlag = NoFlag;
    }
    else
    {
//        setPixmap(lighterPixmap);
        currentFingerFlag = allFingerFlags.first();
        showCurrentFingerFlag();
    }
}

void Card::showCurrentFingerFlag()
{
    if (currentFingerFlag == Effect)
    {
        setCursor(cursorEffect);
    }
    else if (currentFingerFlag == SpecialSummon)
    {
        setCursor(cursorSpecialSummon);
    }
    else if (currentFingerFlag == NormalSummon)
    {
        setCursor(cursorNormalSummon);
    }
    else if (currentFingerFlag == SetCard)
    {
        setCursor(cursorSetCard);
    }
    else if (currentFingerFlag == FlipSummon)
    {
        setCursor(cursorFlipSummon);
    }
    else if (currentFingerFlag == DefencePosition)
    {
        setCursor(cursorDefencePosition);
    }
    else if (currentFingerFlag == AttackPosition)
    {
        setCursor(cursorAttackPosition);
    }
    else if (currentFingerFlag == Attack)
    {
        setCursor(cursorAttack);
    }
}

// 当我点击任意一张卡牌时
void Card::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::RightButton) // 右键点击的话， 切换到下一种鼠标手势， 包括发动效果、特招、普招等等
    {
        if (allFingerFlags.isEmpty())//allFingerFlags 肯定非空
        {
            return;
        }
        int currentIndex = allFingerFlags.indexOf(currentFingerFlag);
        currentFingerFlag = currentFingerFlag == allFingerFlags.last() ? allFingerFlags.first() : allFingerFlags.at(currentIndex + 1);
        showCurrentFingerFlag();
    }
    else if (event->button() == Qt::LeftButton)
    {
        if (Rule::instance()->getPickRequirement() != NoRequirement)
        {
            qDebug() << "emit pickTarget()";
            emit pickTarget();
            return;
        }
        else
        {
            if (allFingerFlags.isEmpty())
            {
                return;
            }
        }

        if (currentFingerFlag == Effect)
        {
            qDebug() << "currentflag == Effect";
            emit activeEffect();
        }
        else if (currentFingerFlag == NormalSummon)
        {
            Rule::instance()->setOneTurnOneNormalSummon(false);
            emit normalSummon();
        }
        else if (currentFingerFlag == SetCard)
        {
            Rule::instance()->setOneTurnOneNormalSummon(false);
            emit setCard();
        }
        else if (currentFingerFlag == SpecialSummon)
        {
            //目前没有从手牌特殊召唤的卡牌
            emit normalSummon();
        }
        else if (currentFingerFlag == FlipSummon)
        {
            //目前没有翻转召唤
            //emit flipSummon();
        }
        else if (currentFingerFlag == DefencePosition)
        {
            //
        }
        else if (currentFingerFlag == AttackPosition)
        {
            //
        }
        else if (currentFingerFlag == Attack)
        {
            qDebug() << "currentflag == Attack";
            emit pressSword();
        }

        currentFingerFlag = NoFlag;
        setCursor(cursorNoFlag);
    }
}

int Card::getLevel() const
{
    return level;
}

void Card::setLevel(int value)
{
    level = value;
}

int Card::getCurrentDEF() const
{
    return currentDEF;
}

void Card::setCurrentDEF(int value)
{
    currentDEF = value;
}

int Card::getCurrentATK() const
{
    return currentATK;
}

void Card::setCurrentATK(int value)
{
    currentATK = value;
}

int Card::getDEF() const
{
    return DEF;
}

void Card::setDEF(int value)
{
    DEF = value;
}

int Card::getATK() const
{
    return ATK;
}

void Card::setATK(int value)
{
    ATK = value;
}

bool Card::getBuff_602() const
{
    return buff_602;
}

void Card::setBuff_602(bool value)
{
    buff_602 = value;
}

bool Card::getBuff_604() const
{
    return buff_604;
}

void Card::setBuff_604(bool value)
{
    buff_604 = value;
}

bool Card::getOneTurnOneAttack() const
{
    return oneTurnOneAttack;
}

void Card::setOneTurnOneAttack(bool value)
{
    oneTurnOneAttack = value;
}

bool Card::getOneTurnHandEffect() const
{
    return oneTurnHandEffect;
}

void Card::setOneTurnHandEffect(bool value)
{
    oneTurnHandEffect = value;
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

    if (area == Deck_Area)
    {
        if (!isBack || !isField)
        {
            setPixmap(QPixmap(":/field/NULL.jpg"));
            isBack = true;
            isField = true;
        }
    }
    else if (area == Hand_Area)
    {
        if (isBack || isField)
        {
            //默认没有face==false的卡牌
            setPixmap(QPixmap(QString(":/hand/%1.jpg").arg(name)));
            isBack = false;
            isField = false;
        }
    }
    else if (area == Fieldyard_Area)
    {
        if (face)
        {
            if (isBack || !isField)
            {
                setPixmap(QPixmap(QString(":/field/%1.jpg").arg(name)));
                isBack = false;
                isField = true;
            }
        }
        else
        {
            if (!isBack || !isField)
            {
                setPixmap(QPixmap(":/field/NULL.jpg"));
                isBack = true;
                isField = true;
            }
        }
    }
    else if (area == Fieldground_Area)
    {
        if (face)
        {
            if (isBack || !isField)
            {
                setPixmap(QPixmap(QString(":/field/%1.jpg").arg(name)));
                isBack = false;
                isField = true;
            }
        }
        else
        {
            if (!isBack || !isField)
            {
                setPixmap(QPixmap(":/field/NULL.jpg"));
                isBack = true;
                isField = true;
            }
        }
    }
    else if (area == Graveyard_Area)
    {
        if (face)
        {
            if (isBack || !isField)
            {
                setPixmap(QPixmap(QString(":/field/%1.jpg").arg(name)));
                isBack = false;
                isField = true;
            }
        }
        else
        {
            if (!isBack || !isField)
            {
                setPixmap(QPixmap(":/field/NULL.jpg"));
                isBack = true;
                isField = true;
            }
        }
    }
    else if (area == EnemyDeck_Area)
    {
        if (!isBack || !isField)
        {
            setPixmap(QPixmap(":/field/NULL.jpg"));
            isBack = true;
            isField = true;
        }
    }
    else if (area == EnemyHand_Area)
    {
        if (face)
        {
            if (isBack || isField)
            {
                setPixmap(QPixmap(QString(":/hand/%1.jpg").arg(name)));
                isBack = false;
                isField = false;
            }
        }
        else
        {
            if (!isBack || isField)
            {
                setPixmap(QPixmap(":/hand/NULL.jpg"));
                isBack = true;
                isField = false;
            }
        }
    }
    else if (area == EnemyFieldyard_Area)
    {
        if (face)
        {
            if (isBack || !isField)
            {
                setPixmap(QPixmap(QString(":/field/%1.jpg").arg(name)));
                isBack = false;
                isField = true;
            }
        }
        else
        {
            if (!isBack || !isField)
            {
                setPixmap(QPixmap(":/field/NULL.jpg"));
                isBack = true;
                isField = true;
            }
        }
    }
    else if (area == EnemyFieldground_Area)
    {
        if (face)
        {
            if (isBack || !isField)
            {
                setPixmap(QPixmap(QString(":/field/%1.jpg").arg(name)));
                isBack = false;
                isField = true;
            }
        }
        else
        {
            if (!isBack || !isField)
            {
                setPixmap(QPixmap(":/field/NULL.jpg"));
                isBack = true;
                isField = true;
            }
        }
    }
    else if (area == EnemyGraveyard_Area)
    {
        if (face)
        {
            if (isBack || !isField)
            {
                setPixmap(QPixmap(QString(":/field/%1.jpg").arg(name)));
                isBack = false;
                isField = true;
            }
        }
        else
        {
            if (!isBack || !isField)
            {
                setPixmap(QPixmap(":/field/NULL.jpg"));
                isBack = true;
                isField = true;
            }
        }
    }

    originalPixmap = pixmap();
    QImage image = pixmap().toImage();
    for(int i=0;i<image.width();i++)
    {
        for(int j=0;j<image.height();j++)
        {
            image.setPixelColor(i,j,image.pixelColor(i,j).lighter(165));
        }
    }
    lighterPixmap = QPixmap::fromImage(image);
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
            isRotate = false;
            if (area == Hand_Area)
            {
                setTransformOriginPoint(50, 72);
                setRotation(0);
            }
            else if (area == Deck_Area
                || area == Fieldyard_Area
                || area == Fieldground_Area
                || area == Graveyard_Area)
            {
                setTransformOriginPoint(25, 36);
                setRotation(0);
            }
            else if (area == EnemyHand_Area)
            {
                setTransformOriginPoint(50, 72);
                setRotation(180);
            }
            else if (area == EnemyDeck_Area
                || area == EnemyFieldyard_Area
                || area == EnemyFieldground_Area
                || area == EnemyGraveyard_Area) //以后加入除外额外环境区域
            {
                setTransformOriginPoint(25, 36);
                setRotation(180);
            }
        }
    }
    else
    {
        if (!isRotate)
        {
            isRotate = true;
            if (area == Hand_Area)
            {
                setTransformOriginPoint(50, 72);
                setRotation(-90);
            }
            else if (area == Deck_Area
                || area == Fieldyard_Area
                || area == Fieldground_Area
                || area == Graveyard_Area)
            {
                setTransformOriginPoint(25, 36);
                setRotation(-90);
            }
            else if (area == EnemyHand_Area)
            {
                setTransformOriginPoint(50, 72);
                setRotation(-270);
            }
            else if (area == EnemyDeck_Area
                || area == EnemyFieldyard_Area
                || area == EnemyFieldground_Area
                || area == EnemyGraveyard_Area) //以后加入除外额外环境区域
            {
                setTransformOriginPoint(25, 36);
                setRotation(-270);
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
