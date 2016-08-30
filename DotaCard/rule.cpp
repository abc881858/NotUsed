#include "rule.h"
#include <QObject>

Q_GLOBAL_STATIC(Rule, rule)

Rule* Rule::instance()
{
    return rule();
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
}

Rule::Phase Rule::getphase() const
{
    return this->phase;
}
