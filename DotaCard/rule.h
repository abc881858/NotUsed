#ifndef RULE_H
#define RULE_H

class Rule
{
public:
    enum Phase
    {
        myDP = 0,
        mySP = 1,
        myM1 = 2,
        myBP = 3,
        myM2 = 4,
        myEP = 5,
        yourDP = 6,
        yourSP = 7,
        yourM1 = 8,
        yourBP = 9,
        yourM2 = 10,
        yourEP = 11
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
