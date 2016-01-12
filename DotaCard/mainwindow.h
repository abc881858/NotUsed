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
#include "roomscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    Q_ENUMS(Phase)

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    QTcpSocket* client;

    enum Phase { myDP,
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
        yourEP };
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
    void yourBattlePhase();
    void yourMainPhase2();
    void yourEndPhase();

    QByteArray getJsonFromInt(int command);
    void firstSetupEnemyDeck(QJsonArray jsonArray);
    void secondSetupEnemyDeck(QJsonArray jsonArray);

private:
    Ui::MainWindow* ui;
    Phase phase;

    QList<Card *> myDeck, yourDeck, myHand, yourHand, myFieldyard, yourFieldyard, myFieldground, yourFieldground;

    int myLP;
    int yourLP;

    RoomScene* roomScene;

public slots:
    void readFromServer();
    void connected();

private slots:
    void on_buttonBP_clicked();
    void on_buttonM2_clicked();
    void on_buttonEP_clicked();
    void setBigImage(QString s);
};

#endif // MAINWINDOW_H
