
#include "progress.h"

progress::progress(QWidget* parent)
{
    this->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint |Qt::WindowStaysOnTopHint );
    this->setStyleSheet("background-color:red;font-size:60px;");
    //this->move(472,520);
    group=new QParallelAnimationGroup;
    QPropertyAnimation *animation1 = new QPropertyAnimation(this,"geometry");
    animation1->setDuration(20000);
    animation1->setStartValue(QRect(552,580,300,8));
    animation1->setEndValue(QRect(552,580,0,8));
    group->addAnimation(animation1);

}

void progress::anime(){
    group->start();
}

void progress::stop(){
    group->stop();
}

progress::~progress(){}
