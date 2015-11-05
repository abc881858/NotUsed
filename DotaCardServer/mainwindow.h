#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTcpServer *server;
    QList<QTcpSocket*> clients;

    void startGame();
public slots:
    void newConnection();
    void readFromFirstClient();
    void readFromSecondClient();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
