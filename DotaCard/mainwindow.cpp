#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPalette>
#include <QBrush>
#include <QPixmap>

#include "net.h"
#include "rule.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/png/png/back.jpg")));
    setPalette(palette);

    Net::instance()->initialize();

    //    //对端做了什么操作，我方都要显示出来
    //    connect(Net::instance(), SIGNAL(actionCommand(int, int)), this, SLOT(doActionCommand(int, int)));
    //    connect(Net::instance(), SIGNAL(getResponse()), this, SLOT(doGetResponse()));

    myLP = 8000;
    yourLP = 8000;
    ui->myLcdNumber->display(myLP);
    ui->yourLcdNumber->display(yourLP);

    roomScene = new RoomScene(ui->roomWidget);
    ui->roomView->setScene(roomScene);
    ui->roomView->setSceneRect(0, 0, 550, 600);

    connect(roomScene, &RoomScene::hover, [=](QString name)
        {
            //首先一张卡被hover，左上角大图肯定要更新
            //但对方的（除前后场的face-up卡）不该显示，我方卡组也不该显示
            ui->label->setStyleSheet(QString("image: url(:/pic/monster/%1.jpg)").arg(name));
        });
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::battleYourPhase()
//{
//    Rule::instance()->setPhase(Rule::yourBP);
//    //如果前后场都没有卡，就显示对话框了
//    QMessageBox msgBox(this);
//    msgBox.setText("dui fang jin ru zhan dou liu cheng.");
//    msgBox.setInformativeText("qing wen fa dong ka pian te xiao ma?");
//    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
//    int ret = msgBox.exec();
//    if (ret == QMessageBox::Close)
//    {
//        //        Net::instance()->sendMessage(0);
//    }
//    else
//    {
//        Rule::instance()->setIsResponsing(true);
//    }
//}

//void MainWindow::doGetResponse()
//{
//    Rule::instance()->setIsWaiting(false);
//}

//void MainWindow::doActionCommand(int parameter, int index)
//{
//    roomScene->doActionCommand(parameter, index);
//}
