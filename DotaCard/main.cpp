#include "engine.h"
#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QTranslator>
#include <QtDebug>

void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    Q_UNUSED(context)
    QString txt;
    switch (type)
    {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        // txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtInfoMsg:
        txt = QString("InfoMsg: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        abort();
    }

    QFile outFile("debuglog.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char* argv[])
{
    qInstallMessageHandler(myMessageOutput);

    QApplication a(argc, argv);
    QTranslator translator;
    if (translator.load("dc_zh_cn"))
    {
        a.installTranslator(&translator);
    }

    Engine::instance()->loadAllCards();

    MainWindow* mainwindow = new MainWindow;
    mainwindow->setWindowTitle("Dota Card");
    mainwindow->show();

    return a.exec();
}
