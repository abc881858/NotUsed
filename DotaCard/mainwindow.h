#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class RoomScene;

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
    RoomScene* roomScene;
    int myLP;
    int yourLP;
};

#endif // MAINWINDOW_H
