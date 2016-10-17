#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QBrush>
#include <QPalette>
#include <QPixmap>
#include <QCursor>

#include "net.h"
#include "roomscene.h"
#include "rule.h"
#include "card.h"

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

    QCursor cursorCommmon = QCursor(QPixmap(":/png/png/3.cur"), 31, 15);
    setCursor(cursorCommmon);

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
            ui->label->setStyleSheet(QString("image: url(:/image/%1.jpg)").arg(name));
            ui->textEdit->setText(description);
        });
    connect(ui->buttonBP, &QPushButton::pressed, [=]()
        {
            if (Rule::instance()->getPickRequirement())
            {
                return;
            }
            if (Rule::instance()->getphase() == Rule::myM1)
            {
                Rule::instance()->setPhase(Rule::myBP);
                Rule::instance()->setDoing(false);
                Net::instance()->sendMessage(666); //询问对方是否连锁
            }
        });
    connect(ui->buttonM2, &QPushButton::pressed, [=]()
        {
            if (Rule::instance()->getPickRequirement())
            {
                return;
            }
            if (Rule::instance()->getphase() == Rule::myBP)
            {
                Rule::instance()->setPhase(Rule::myM2);
                Rule::instance()->setDoing(false);
                Net::instance()->sendMessage(666); //询问对方是否连锁
            }
        });
    connect(ui->buttonEP, &QPushButton::pressed, [=]()
        {
            if (Rule::instance()->getPickRequirement())
            {
                return;
            }
            Rule::Phase phase = Rule::instance()->getphase();
            if (phase == Rule::myM1 || phase == Rule::myBP || phase == Rule::myM2)
            {
                Rule::instance()->setPhase(Rule::myEP);
                Rule::instance()->setDoing(false);
                Net::instance()->sendMessage(666); //TODO: //询问对方是否连锁，处理结束阶段的连锁
            }
        });
}

MainWindow::~MainWindow()
{
    delete ui;
}
