#include "rule.h"
#include <QObject>
#include "net.h"

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
    Net::instance()->doSetPhase(phase);
}

void Rule::setPhase(int phase)
{
    this->phase = Rule::Phase(phase);
    Net::instance()->doSetPhase(phase);
}

Rule::Phase Rule::getphase() const
{
    return this->phase;
}

bool Rule::getIsWaiting() const
{
    return isWaiting;
}

void Rule::setIsWaiting(bool value)
{
    isWaiting = value;
}

bool Rule::getIsResponsing() const
{
    return isResponsing;
}

void Rule::setIsResponsing(bool value)
{
    isResponsing = value;
}
