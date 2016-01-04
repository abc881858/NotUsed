#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
    QByteArray getJsonFromInt(int command);
public slots:
    void newConnection();
    void readFromFirstClient();
    void readFromSecondClient();
private:
    Ui::MainWindow *ui;
    QJsonArray deck1,deck2;
};

#endif // MAINWINDOW_H
