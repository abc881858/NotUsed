#ifndef RULE_H
#define RULE_H

class Rule
{
public:
    enum Phase
    {
        myDP,
        mySP,
        myM1,
        myBP,
        myM2,
        myEP,
        yourDP,
        yourSP,
        yourM1,
        yourBP,
        yourM2,
        yourEP
    };

    static Rule* instance();

    bool getOneTurnOneNormalSummon() const;
    void setOneTurnOneNormalSummon(bool value);

    void setPhase(Phase phase);
    Phase getphase() const;

private:
    bool oneTurnOneNormalSummon;
    Rule::Phase phase;
};

#endif // RULE_H
