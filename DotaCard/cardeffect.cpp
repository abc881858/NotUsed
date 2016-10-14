#include "roomscene.h"

#include <QDebug>
#include <QMessageBox>

#include "rule.h"
#include "net.h"
#include "card.h"
#include "area.h"

CentaurWarrunner::CentaurWarrunner()
{
    ISDN = 601;
    name = "dota-CentaurWarrunner";
    setPixmap(QPixmap(":/pic/monster/dota-CentaurWarrunner.jpg"));
    type = EffectMonster;
    ATK = 1350;
    DEF = 1800;
    level = 3;
    attribute = Earth;
    description = tr("CentaurWarrunner");
}

bool CentaurWarrunner::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace() || !getOneTurnOneEffect())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();

    if (getArea() == Fieldyard_Area)
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

void RoomScene::CentaurWarrunnerEffect()
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::question(0, QString(tr("active CentaurWarrunner's effect")), QString(tr("Select all atk or all def?")), QMessageBox::Yes | QMessageBox::No);
    bool all = (ret == QMessageBox::Yes);
    for (Card* card : FieldyardArea::instance()->getMyFieldyard())
    {
        if (card->getFace())
        {
            card->setStand(all);
        }
    }
    Net::instance()->doCentaurWarrunnerEffect(all);
}

void Net::doCentaurWarrunnerEffect(bool all)
{
    QJsonObject parameter;
    parameter.insert("all", all);
    QJsonObject object;
    object.insert("request", "doCentaurWarrunnerEffect");
    object.insert("parameter", parameter);
    write(object);
}

void RoomScene::response_doCentaurWarrunnerEffect(QJsonObject jsonObject)
{
    bool all = jsonObject["all"].toBool();
    for (Card* card : EnemyFieldyardArea::instance()->getYourFieldyard())
    {
        if (card->getFace())
        {
            card->setStand(all);
        }
    }
}

//光之守卫
KeeperoftheLight::KeeperoftheLight()
{
    ISDN = 602;
    name = "dota-KeeperoftheLight";
    setPixmap(QPixmap(":/pic/monster/dota-KeeperoftheLight.jpg"));
    type = EffectMonster;
    ATK = 800;
    DEF = 300;
    level = 3;
    description = tr("KeeperoftheLight");
}

bool KeeperoftheLight::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace() || !getOneTurnOneEffect())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Hand_Area)
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            if(FieldyardArea::instance()->getMyFieldyard().count() > 0)
            {
                return true;
            }
        }
    }

    if (getArea() == Fieldyard_Area)
    {
        {
            if (phase == Rule::myM1 || phase == Rule::myM2)
            {
                if(EnemyFieldyardArea::instance()->getYourFieldyard().count() > 0)
                {
                    return true;
                }
                else
                {
                    //卡牌上面弹出tip: 不满足发动条件
                }
            }
        }
    }

    return false;
}

void RoomScene::KeeperoftheLightEffect()
{
   QMessageBox::question(0, QString(tr("active KeeperoftheLight's effect")), QString(tr("Select enemyfieldyard card.")), QMessageBox::Ok);
}

void RoomScene::response_doKeeperoftheLightEffect(QJsonObject jsonObject)
{
}

//恶魔巫师
Lion::Lion()
{
    ISDN = 603;
    name = "dota-Lion";
    setPixmap(QPixmap(":/pic/monster/dota-Lion.jpg"));
    type = EffectMonster;
    ATK = 1300;
    DEF = 700;
    level = 4;
    description = tr("Lion");
}

bool Lion::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace() || !getOneTurnOneEffect())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Fieldyard_Area)
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

void RoomScene::LionEffect()
{
    QMessageBox::question(0, QString(tr("active Lion's effect")), QString(tr("Select enemyfieldyard card.")), QMessageBox::Ok);
}

void RoomScene::response_doLionEffect(QJsonObject jsonObject)
{
}

//半人猛犸
Magnus::Magnus()
{
    ISDN = 604;
    name = "dota-Magnus";
    setPixmap(QPixmap(":/pic/monster/dota-Magnus.jpg"));
    type = EffectMonster;
    ATK = 1700;
    DEF = 700;
    level = 4;
    description = tr("Magnus");
}

bool Magnus::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace() || !getOneTurnOneEffect())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Fieldyard_Area)
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

void RoomScene::MagnusEffect()
{
    QMessageBox::question(0, QString(tr("active Magnus's effect")), QString(tr("Select fieldyard card.")), QMessageBox::Ok);
}

void RoomScene::response_doMagnusEffect(QJsonObject jsonObject)
{
}

//地穴刺客
NyxAssassin::NyxAssassin()
{
    ISDN = 605;
    name = "dota-NyxAssassin";
    setPixmap(QPixmap(":/pic/monster/dota-NyxAssassin.jpg"));
    type = EffectMonster;
    ATK = 1500;
    DEF = 800;
    level = 4;
    description = tr("NyxAssassin");
}

bool NyxAssassin::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace() || !getOneTurnOneEffect())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Fieldyard_Area)
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

void RoomScene::NyxAssassinEffect()
{
}

void RoomScene::response_doNyxAssassinEffect(QJsonObject jsonObject)
{
    //
}

//大魔导师
Rubick::Rubick()
{
    ISDN = 606;
    name = "dota-Rubick";
    setPixmap(QPixmap(":/pic/monster/dota-Rubick.jpg"));
    type = EffectMonster;
    ATK = 900;
    DEF = 300;
    level = 4;
    description = tr("Rubick");
}

bool Rubick::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace() || !getOneTurnOneEffect())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Fieldyard_Area)
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

void RoomScene::RubickEffect()
{
}

void RoomScene::response_doRubickEffect(QJsonObject jsonObject)
{
    //
}

//巨牙海民
Tusk::Tusk()
{
    ISDN = 607;
    name = "dota-Tusk";
    setPixmap(QPixmap(":/pic/monster/dota-Tusk.jpg"));
    type = EffectMonster;
    ATK = 1800;
    DEF = 1000;
    level = 4;
    description = tr("Tusk");
}

bool Tusk::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace() || !getOneTurnOneEffect())
    {
        return false;
    }
    return false;
}

void RoomScene::TuskEffect()
{
}

void RoomScene::response_doTuskEffect(QJsonObject jsonObject)
{
    //
}

//不朽尸王
Undying::Undying()
{
    ISDN = 608;
    name = "dota-Undying";
    setPixmap(QPixmap(":/pic/monster/dota-Undying.jpg"));
    type = EffectMonster;
    ATK = 1100;
    DEF = 1300;
    level = 3;
    description = tr("Undying");
}

bool Undying::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace() || !getOneTurnOneEffect())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Fieldyard_Area)
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

void RoomScene::UndyingEffect()
{
}

void RoomScene::response_doUndyingEffect(QJsonObject jsonObject)
{
}

//复仇之魂
VengefulSpirit::VengefulSpirit()
{
    ISDN = 609;
    name = "dota-VengefulSpirit";
    setPixmap(QPixmap(":/pic/monster/dota-VengefulSpirit.jpg"));
    type = EffectMonster;
    ATK = 1200;
    DEF = 500;
    level = 3;
    description = tr("VengefulSpirit");
}

bool VengefulSpirit::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace() || !getOneTurnOneEffect())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Fieldyard_Area)
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

void RoomScene::VengefulSpiritEffect()
{
}

void RoomScene::response_doVengefulSpiritEffect(QJsonObject jsonObject)
{
}

//奥林匹斯之王
Zeus::Zeus()
{
    ISDN = 610;
    name = "dota-Zeus";
    setPixmap(QPixmap(":/pic/monster/dota-Zeus.jpg"));
    type = EffectMonster;
    ATK = 500;
    DEF = 350;
    level = 2;
    description = tr("Zeus");
}

bool Zeus::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace() || !getOneTurnOneEffect())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Fieldyard_Area)
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

void RoomScene::ZeusEffect()
{
}

void RoomScene::response_doZeusEffect(QJsonObject jsonObject)
{
}
