#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QtDebug>

void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    Q_UNUSED(context)
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        abort();
    case QtInfoMsg:
        txt = QString("Info: %1").arg(msg);
        break;
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
    MainWindow w;
    w.setWindowTitle("Dota Card");
    w.show();

    return a.exec();
}
