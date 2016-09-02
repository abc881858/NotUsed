#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "roomscene.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    int myLP;
    int yourLP;

    RoomScene* roomScene;

private slots:
    void setupDeck(QList<int> list);
    void setupEnemyDeck(QList<int> list);
    void startMyGame();
    void startYourGame();
    void drawMyPhase();
    void drawYourPhase();
    void standbyMyPhase();
    void standbyYourPhase();
    void mainMyPhase1();
    void mainYourPhase1();
    void battleYourPhase();
    void mainYourPhase2();
    void endYourPhase();

public slots:
    void hover(QString);
    void doActionCommand(int para, int from, int to);
    void doGetResponse();
};

#endif // MAINWINDOW_H
