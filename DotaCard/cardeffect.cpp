#include "area.h"
#include "card.h"
#include "net.h"
#include "roomscene.h"
#include "rule.h"
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>

CentaurWarrunner::CentaurWarrunner()
{
    ISDN = 601;
    name = "dota-CentaurWarrunner";
    type = EffectMonster;
    ATK = 1350;
    DEF = 1800;
    level = 3;
    attribute = Earth;
    description = tr("CentaurWarrunner");

    currentATK = ATK;
    currentDEF = DEF;
}

bool CentaurWarrunner::testEffect()
{
    qDebug() << "testEffect: ";
    qDebug() << "getDoing: " << Rule::instance()->getDoing();
    qDebug() << "getFace: " << getFace();
    qDebug() << "getphase: " << Rule::instance()->getphase();
    qDebug() << "getOneTurnOneEffect: " << getOneTurnOneEffect();

    if (!Rule::instance()->getDoing() || !getFace())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();

    if (getArea() == Fieldyard_Area && getOneTurnOneEffect())
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

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
void CentaurWarrunner::activeEffect()
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

    QJsonObject parameter;
    parameter.insert("pickRequirement", CentaurWarrunnerEffect);
    parameter.insert("all", all);
    QJsonObject object;
    object.insert("request", "Effect");
    object.insert("parameter", parameter);
    Net::instance()->write(object);
}

//光之守卫
KeeperoftheLight::KeeperoftheLight()
{
    ISDN = 602;
    name = "dota-KeeperoftheLight";
    type = EffectMonster;
    ATK = 800;
    DEF = 300;
    level = 3;
    description = tr("KeeperoftheLight");

    currentATK = ATK;
    currentDEF = DEF;
}

bool KeeperoftheLight::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Hand_Area && getOneTurnHandEffect())
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            if (FieldyardArea::instance()->getMyFieldyard().count() > 0)
            {
                return true;
            }
        }
    }

    if (getArea() == Fieldyard_Area && getOneTurnOneEffect())
    {
        {
            if (phase == Rule::myM1 || phase == Rule::myM2)
            {
                if (EnemyFieldyardArea::instance()->getYourFieldyard().count() > 0)
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

/*!
 * \brief 光之守卫
 *
 * ①一回合一次，当有“dota-光之守卫”以外名字带有“dota”的加入手牌时可以发动，
 * 将那只怪兽特殊召唤至场上，若这张卡装备了“dota-阿哈利姆神杖”。
 * 则改此效果为：一回合一次，可以从手牌·卡组特殊召唤一只“dota-光之守卫”以外的名字带有“dota”的怪兽。
 *
 * ②这张卡在手牌存在时，一回合一次，将自己场上表侧表示的一只“dota-光之守卫”以外的名字带有“dota”的怪兽返回手牌。
 *
 * ③一回合一次，选择对方场上的一只表侧表示怪兽发动，选择的怪兽在进行攻击宣言前必须丢弃一张手牌。
 *
 */
void KeeperoftheLight::activeEffect()
{
    int area = getArea();
    if (area == Fieldyard_Area) //发动场上的特效
    {
        QMessageBox::question(0, QString(tr("active KeeperoftheLight's effect")), QString(tr("Select enemyfieldyard card.")), QMessageBox::Ok);
        //设置requirement的时候，把picking设为true，代表正在【选择】【需要】的卡
        Rule::instance()->setPickRequirement(KeeperoftheLightRequirement);
    }
    else if (area == Hand_Area) //发动手上的特效
    {
        setOneTurnHandEffect(false);
        QMessageBox::question(0, QString(tr("active KeeperoftheLight's effect")), QString(tr("Select fieldyard card.")), QMessageBox::Ok);
        Rule::instance()->setPickRequirement(KeeperoftheLightRequiremented);
    }
}

//恶魔巫师
Lion::Lion()
{
    ISDN = 603;
    name = "dota-Lion";
    type = EffectMonster;
    ATK = 1300;
    DEF = 700;
    level = 4;
    description = tr("Lion");

    currentATK = ATK;
    currentDEF = DEF;
}

bool Lion::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Fieldyard_Area && getOneTurnOneEffect())
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            if (EnemyFieldyardArea::instance()->getYourFieldyard().count() > 0)
            {
                return true;
            }
        }
    }

    return false;
}

/*!
 * \brief 恶魔巫师
 *
 * 一回合一次，选择下面的其一的效果发动：
 * 1、选择对方场上一张怪兽发动，选择的怪兽变为里侧守备表示。
 * 2、选择对方场上一张里侧表示的怪兽发动，选择的怪兽破坏。
 * 当这张卡装备了“dota-阿哈利姆神杖”时，同时破坏选择怪兽两边的卡。
 *
 */
void Lion::activeEffect()
{
    QMessageBox::question(0, QString(tr("active Lion's effect")), QString(tr("Select enemyfieldyard card.")), QMessageBox::Ok);
    Rule::instance()->setPickRequirement(LionRequirement);
}

//半人猛犸
Magnus::Magnus()
{
    ISDN = 604;
    name = "dota-Magnus";
    type = EffectMonster;
    ATK = 1700;
    DEF = 700;
    level = 4;
    description = tr("Magnus");

    currentATK = ATK;
    currentDEF = DEF;
}

bool Magnus::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Fieldyard_Area && getOneTurnOneEffect())
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

/*!
 * \brief 半人猛犸
 *
 * ①一回合一次，令自己场上表侧表示的一只名字带有“dota”的怪兽，
 * 在你下回合开始前攻击力上升400点，且在攻击之后，
 * 可以降低600点攻击对对方场上所有怪兽进行一次攻击。
 *
 * ②将这张卡装备的“dota-跳刀”送去墓地，
 * 将对方场上的怪兽全部转为正面攻击表示，攻击力变为0，
 * 且在对方回合结束前不能进行攻击不能更变形式。
 *
 */
void Magnus::activeEffect()
{
    QMessageBox::question(0, QString(tr("active Magnus's effect")), QString(tr("Select fieldyard card.")), QMessageBox::Ok);
    Rule::instance()->setPickRequirement(MagnusRequirement);
}

//地穴刺客
NyxAssassin::NyxAssassin()
{
    ISDN = 605;
    name = "dota-NyxAssassin";
    type = EffectMonster;
    ATK = 1500;
    DEF = 800;
    level = 4;
    description = tr("NyxAssassin");

    currentATK = ATK;
    currentDEF = DEF;
}

bool NyxAssassin::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Fieldyard_Area && getOneTurnOneEffect())
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

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
void NyxAssassin::activeEffect()
{
    QMessageBox messageBox;
    QPushButton *effect1 = messageBox.addButton(tr("active effect1"), QMessageBox::ActionRole);
    messageBox.addButton(tr("active effect2"), QMessageBox::ActionRole);
    messageBox.exec();
    if (messageBox.clickedButton() == effect1)
    {
        //discard one card from hand area
        QMessageBox::information(0, QString(tr("active NyxAssassin's effect")), QString(tr("Discard one card from hand area.")));
        Rule::instance()->setPickRequirement(NyxAssassinRequirement1);
    }
    else
    {
        QMessageBox::information(0, QString(tr("active NyxAssassin's effect")), QString(tr("Discard one card from hand area.")));
        Rule::instance()->setPickRequirement(NyxAssassinRequirement2);
    }
}

//大魔导师
Rubick::Rubick()
{
    ISDN = 606;
    name = "dota-Rubick";
    type = EffectMonster;
    ATK = 900;
    DEF = 300;
    level = 4;
    description = tr("Rubick");

    currentATK = ATK;
    currentDEF = DEF;
}

bool Rubick::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Fieldyard_Area && getOneTurnOneEffect())
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

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
void Rubick::activeEffect()
{
//    QMessageBox messageBox;
//    QPushButton *effect1 = messageBox.addButton(tr("active effect1"), QMessageBox::ActionRole);
//    messageBox.addButton(tr("active effect2"), QMessageBox::ActionRole);
//    messageBox.exec();
//    if (messageBox.clickedButton() == effect1)
//    {
//        //discard one card from hand area
//        QMessageBox::information(0, QString(tr("active Rubick's effect")), QString(tr("Discard one card from hand area.")));
//        Rule::instance()->setPickRequirement(NyxAssassinRequirement1);
//    }
//    else
//    {
//        QMessageBox::information(0, QString(tr("active Rubick's effect")), QString(tr("Discard one card from hand area.")));
//        Rule::instance()->setPickRequirement(NyxAssassinRequirement2);
//    }
}

//巨牙海民
Tusk::Tusk()
{
    ISDN = 607;
    name = "dota-Tusk";
    type = EffectMonster;
    ATK = 1800;
    DEF = 1000;
    level = 4;
    description = tr("Tusk");

    currentATK = ATK;
    currentDEF = DEF;
}

bool Tusk::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace())
    {
        return false;
    }
    return false;
}

/*!
 * \brief 巨牙海民
 *
 * 这张卡在进行攻击时，可以从手牌表侧攻击特殊召唤一只名字带有“dota”的怪兽，
 * 伤害计算阶段，这张卡的攻击力上升特殊召唤的怪兽攻击力的一半，
 * 这回合，由这个效果特殊召唤而来的怪兽不需要装备“dota-跳刀”便可以发动装备“dota-跳刀”的效果。
 * 一回合一次，这张卡攻击时，若目标怪兽的防御力低于你的攻击力，此次战斗对对方造成的战斗伤害翻倍。
 *
 */
void Tusk::activeEffect()
{
}

//不朽尸王
Undying::Undying()
{
    ISDN = 608;
    name = "dota-Undying";
    type = EffectMonster;
    ATK = 1100;
    DEF = 1300;
    level = 3;
    description = tr("Undying");

    currentATK = ATK;
    currentDEF = DEF;
}

bool Undying::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Fieldyard_Area && getOneTurnOneEffect())
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

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
void Undying::activeEffect()
{
}

//复仇之魂
VengefulSpirit::VengefulSpirit()
{
    ISDN = 609;
    name = "dota-VengefulSpirit";
    type = EffectMonster;
    ATK = 1200;
    DEF = 500;
    level = 3;
    description = tr("VengefulSpirit");

    currentATK = ATK;
    currentDEF = DEF;
}

bool VengefulSpirit::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Fieldyard_Area && getOneTurnOneEffect())
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

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
void VengefulSpirit::activeEffect()
{
}

//奥林匹斯之王
Zeus::Zeus()
{
    ISDN = 610;
    name = "dota-Zeus";
    type = EffectMonster;
    ATK = 500;
    DEF = 350;
    level = 2;
    description = tr("Zeus");

    currentATK = ATK;
    currentDEF = DEF;
}

bool Zeus::testEffect()
{
    if (!Rule::instance()->getDoing() || !getFace())
    {
        return false;
    }

    Rule::Phase phase = Rule::instance()->getphase();
    if (getArea() == Fieldyard_Area && getOneTurnOneEffect())
    {
        if (phase == Rule::myM1 || phase == Rule::myM2)
        {
            return true;
        }
    }

    return false;
}

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
void Zeus::activeEffect()
{
}
