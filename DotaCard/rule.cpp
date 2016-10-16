#include "rule.h"
#include "net.h"

Q_GLOBAL_STATIC(Rule, rule)

Rule* Rule::instance()
{
    return rule();
}

void Rule::initialize()
{
    isDoing = false;
    oneTurnOneNormalSummon = false;
    isPicking = false;

    NameEffected.insert(KeeperoftheLightRequirement,"KeeperoftheLight");
    NameEffected.insert(KeeperoftheLightRequiremented,"KeeperoftheLight");
    NameEffected.insert(LionRequirement,"Lion");
}

bool Rule::getOneTurnOneNormalSummon() const
{
    return oneTurnOneNormalSummon;
}

void Rule::setOneTurnOneNormalSummon(bool value)
{
    oneTurnOneNormalSummon = value;
}

void Rule::setPhase(Rule::Phase phase)
{
    this->phase = phase;
    Net::instance()->doSetPhase(phase);
}

void Rule::setPhase(int phase)
{
    this->phase = Rule::Phase(phase);
}

Rule::Phase Rule::getphase() const
{
    return this->phase;
}

bool Rule::getDoing() const
{
    return isDoing;
}

void Rule::setDoing(bool value)
{
    isDoing = value;
}

bool Rule::getPicking() const
{
    return isPicking;
}

void Rule::setPicking(bool value)
{
    qDebug() << "setPicking(bool value)" << value;
    isPicking = value;
}

int Rule::getPickRequirement() const
{
    return pickRequirement;
}

void Rule::setPickRequirement(int value)
{
    qDebug() << "setPickRequirement(int value)" << value;
    pickRequirement = value;
}

//int Rule::getPickedArea() const
//{
//    return pickedArea;
//}

//int Rule::getPickedIndex() const
//{
//    return pickedIndex;
//}

//void Rule::setPickedCard(int area, int index)
//{
//    pickedArea = area;
//    pickedIndex = index;
//}
