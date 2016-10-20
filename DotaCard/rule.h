#ifndef RULE_H
#define RULE_H

#define NoRequiremente 0
#define AttackedRequirement 100
#define KeeperoftheLightRequirement 602
#define KeeperoftheLightRequiremented 502
#define LionRequirement 603

#include <QHash>

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
//    QHash<int,QString> NameEffected;

    bool getOneTurnOneNormalSummon() const;
    void setOneTurnOneNormalSummon(bool value);

    void setPhase(Phase phase);
    void setPhase(int phase);
    Phase getphase() const;

    bool getDoing() const;
    void setDoing(bool value);

//    bool getPicking() const;
//    void setPicking(bool value);

    int getPickRequirement() const;
    void setPickRequirement(int value);

//    int getPickedArea() const;
//    int getPickedIndex() const;

private:
    bool oneTurnOneNormalSummon;
    Rule::Phase phase;
    bool isDoing;
    bool isPicking;
    int pickRequirement;

//    int pickedArea;
//    int pickedIndex;
};

#endif // RULE_H
