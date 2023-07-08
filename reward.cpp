#include "reward.h"
#include "ui_reward.h"

reward::reward(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reward)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint |Qt::WindowStaysOnTopHint );
}

reward::~reward()
{
    delete ui;
}



void reward::on_Button_again_clicked()
{
    emit newgame();
    this->hide();
}

void reward::count(int money_won){
    if(money_won>0){
        QString s="金钱+";
        s=s+QString::number(money_won);
        this->ui->label_result->setStyleSheet("border-image:url(:/files/win.png);");
        this->ui->label_count->setText(s);
    }
    else{
        QString s="金钱-";
        s=s+QString::number(-money_won);
        this->ui->label_result->setStyleSheet("border-image:url(:/files/lose.png);");
        this->ui->label_count->setText(s);
    }
}

void reward::on_Button_stop_clicked()
{
    emit tohome();
    this->hide();
}

