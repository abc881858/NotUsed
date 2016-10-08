#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
    QTcpServer* server;
    QList<QTcpSocket*> clients;

public slots:
    void newConnection();
    void readFromFirstClient();
    void readFromSecondClient();

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
