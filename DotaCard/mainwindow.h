#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
//#include "deck.h"
//#include "hand.h"
//#include "field.h"
//#include "graveyard.h"
#include <QList>
#include "card.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTcpSocket *client;

    enum Phase { myDP,mySP,myM1,myBP,myM2,myEP,yourDP,yourSP,yourM1,yourBP,yourM2,yourEP };
    Q_ENUM(Phase)
    void setPhase(Phase phase);
    Phase getphase() const;

    void myStartGame();
    void yourStartGame();

    void myDrawPhase();
    void yourDrawPhase();

    void myStandbyPhase();
    void yourStandbyPhase();

    void myMainPhase1();
    void yourMainPhase1();

    void myBattlePhase();
    void yourBattlePhase();

    void myMainPhase2();
    void yourMainPhase2();

    void myEndPhase();
    void yourEndPhase();

private:
    Ui::MainWindow *ui;
    Phase phase;

//    Field description  场地介绍
//    Deck  卡组
//    Hand  手牌
//    Field  场地
//    Graveyard  墓地
//    Fusion deck  融合卡组
//    Remove from play  除外
    QList<Card> myDeck,yourDeck,myHand,yourHand,myField,yourField,myGraveyard,yourGraveyard;

//    Life point (LP)  生命值
    int myLP;
    int yourLP;

//    Turn  回合
//    Draw phase  抽卡阶段
//    Standby phase  准备阶段
//    Main phase 1  主要阶段1
//    Battle phase  战斗阶段
//    Main phase 2  主要阶段2
//    End phase  结束阶段

public slots:
    void readFromServer();
    void connected();
private slots:
    void on_buttonBP_clicked();
    void on_buttonM2_clicked();
    void on_buttonEP_clicked();
};

#endif // MAINWINDOW_H
