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
    void initialize();

    bool getOneTurnOneNormalSummon() const;
    void setOneTurnOneNormalSummon(bool value);

    void setPhase(Phase phase);
    void setPhase(int phase);
    Phase getphase() const;

    bool getDoing() const;
    void setDoing(bool value);

private:
    bool oneTurnOneNormalSummon;
    Rule::Phase phase;
    bool isDoing;
};

#endif // RULE_H
