#include "rule.h"
#include <QObject>
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

    if(!isDoing)
    {
        Net::instance()->sendMessage(666);
    }
}
