#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPalette>
#include <QBrush>
#include <QPixmap>

#include "roomscene.h"
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
    Rule::instance()->initialize();

    myLP = 8000;
    yourLP = 8000;
    ui->myLcdNumber->display(myLP);
    ui->yourLcdNumber->display(yourLP);

    roomScene = new RoomScene(ui->roomWidget);
    ui->roomView->setScene(roomScene);
    ui->roomView->setSceneRect(0, 0, 550, 600);

    connect(roomScene, &RoomScene::hover, [=](QString name, QString description)
        {
            //首先一张卡被hover，左上角大图肯定要更新
            //但对方的（除前后场的face-up卡）不该显示，我方卡组也不该显示
            ui->label->setStyleSheet(QString("image: url(:/pic/monster/%1.jpg)").arg(name));
            ui->textEdit->setText(description);
        });
    connect(ui->buttonBP, &QPushButton::pressed, [=]()
        {
            Rule::instance()->setPhase(Rule::myBP);
            Rule::instance()->setDoing(false);
        });
    connect(ui->buttonM2, &QPushButton::pressed, [=]()
        {
            Rule::instance()->setPhase(Rule::myM2);
            Rule::instance()->setDoing(false);
        });
    connect(ui->buttonEP, &QPushButton::pressed, [=]()
        {
            Rule::instance()->setPhase(Rule::myEP);
            Rule::instance()->setDoing(false);
        });
}

MainWindow::~MainWindow()
{
    delete ui;
}
