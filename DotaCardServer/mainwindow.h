#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class QWebSocketServer;
class QWebSocket;

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

    QWebSocketServer *server;
    QList<QWebSocket*> clients;

public slots:
    void newConnection();
    void readFromFirstClient(QByteArray byteArray);
    void readFromSecondClient(QByteArray byteArray);

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
