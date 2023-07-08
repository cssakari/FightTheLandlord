
#include "card1.h"


card1::card1(int n,int _color ,int _point,bool _emerged)
{
    N=n;
    color=_color;
    point=_point;
    emerged=_emerged;
    picked=0;
    isend=0;
    used=0;
    this->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint |Qt::WindowStaysOnTopHint );
    if(emerged) this->setStyleSheet(cardpath[color][point]);
    else this->setStyleSheet("border-image:url(:/p/cards/back.png);");
};

card1::~card1(){}

void card1::reshow(){
    if(used) this->hide();
    if(picked){
        if(!isend){
            this->resize(18,66);
            this->setStyleSheet(cardpath_h_shift[color][point]);
        }
        else{
            this->resize(48,66);
            this->setStyleSheet(cardpath_h[color][point]);
        }
    }
    else{
        if(!isend){
            this->resize(18,66);
            this->setStyleSheet(cardpath_shift[color][point]);
        }
        else{
            this->resize(48,66);
            this->setStyleSheet(cardpath[color][point]);
        }
    }
}

void card1::mouseReleaseEvent(QMouseEvent *event){
    if(belongto) event->ignore();
    else{
        picked=1-picked;
        reshow();
    }
}



