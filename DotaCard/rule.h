#ifndef RULE_H
#define RULE_H

#include "card.h"

class Rule {
public:
    static Rule* instance();

    bool getOneTurnOneNormalSummon() const;
    void setOneTurnOneNormalSummon(bool value);

private:
    bool oneTurnOneNormalSummon;
};

#endif // RULE_H
