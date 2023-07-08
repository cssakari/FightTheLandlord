
#include "mainwindow.h"
#include<QApplication>
#include<QWidget>
#include<QDebug>


#include <QSoundEffect>
#include <QUrl>
#include <QMediaPlayer>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    QSoundEffect * startSound = new QSoundEffect();
    startSound->setSource(QUrl::fromLocalFile(":/p/music/background.wav"));
    startSound->setVolume(1);
    startSound->setLoopCount(QSoundEffect::Infinite);
    startSound->play();


    qDebug()<<startSound->status();

    return a.exec();
}
