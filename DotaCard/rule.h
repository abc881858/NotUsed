#ifndef RULE_H
#define RULE_H

class Rule
{
public:
    enum Phase
    {
        myDP = 0,
        mySP,
        myM1,
        myBP,
        myM2,
        myEP,
        yourDP = 6,
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
    void setPhase(int phase);
    Phase getphase() const;

    bool getIsWaiting() const;
    void setIsWaiting(bool value);

    bool getIsResponsing() const;
    void setIsResponsing(bool value);

private:
    bool oneTurnOneNormalSummon;
    Rule::Phase phase;
    bool isWaiting;
    bool isResponsing;
};

#endif // RULE_H
