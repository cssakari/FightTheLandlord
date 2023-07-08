
#include "mainwindow.h"
#include "AI.h"
#include "player.h"
#include "randomArray.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "ai.h"
//#include  "AI.h"
#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    money=1000;
    QFile file;
    QString f = QFileDialog::getOpenFileName(this, QString("选择文件"), QString("/"),QString("TEXT(*.txt)"));
    file.setFileName(f);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray t;
        while(!file.atEnd())
        {
            t+=file.readLine();
        }
        money=QString(t).toInt();
    }
    file.close();
    qDebug()<<money;
    startSound = new QSoundEffect();
    startSound->setSource(QUrl::fromLocalFile(":/p/music/background.wav"));
    startSound->setVolume(1);
    startSound->setLoopCount(QSoundEffect::Infinite);
    startSound->play();
    //ui->horizontalSlider->setValue(1);
    setWindowFlags(Qt::Window);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setWindowFlags((windowFlags() & ~Qt::WindowCloseButtonHint));
    //setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小
    ui->setupUi(this);
    QFont font1("STXingkai",100);
    ui->label->setFont(font1);
    QFont font2("SimHei",100);
    ui->label_4->setFont(font2);
    setWindowTitle("FightTheLandlord");
    this->setObjectName("mainScreen");
    setStyleSheet("QMainWindow#mainScreen{border-image:url(:/files/homepage.jpg);}");
    ui->stackedWidget->setCurrentIndex(1);
    ui->Button_Playcard->hide();
    activepage=1;
    num_games=0;
    cardback_num=0;
    color_of_back="purple";
    connect(ui->Button_Play,&QPushButton::clicked,this,[=](){
        ui->Button_Play->hide();
        ui->Button_Playcard->show();
        ui->pushButton->hide();
        ui->radioButton->hide();
        ui->radioButton_2->hide();
        ui->label_3->hide();
        ui->stackedWidget->setCurrentIndex(0);
        activepage=0;
        //ui->Button_Settings_0->setStyleSheet("background-image:url(:/files/settings.png)");
        //ui->Button_Settings_0->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint |Qt::WindowStaysOnTopHint );
        SetBackground(color_of_back);
        NewGame();
    });
    connect(ui->Button_Restart,&QPushButton::clicked,this,[=](){
        for(int i=0;i<54;i++) c[i]->hide();
        NewGame();
    });
    connect(ui->Button_Back,&QPushButton::clicked,this,[=](){
        setStyleSheet("QMainWindow#mainScreen{border-image:url(:/files/homepage.jpg);}");
        //ui->Button_Settings_1->setStyleSheet("border-image:url(:/files/settings.png)");
        ui->stackedWidget->setCurrentIndex(1);
        activepage=1;
        ui->Button_Play->show();
        ui->Button_Playcard->hide();
        for (int i = 0; i < 54; ++i) {
            c[i]->hide();
        }
        pro.stop();
        pro.hide();
    });
    connect(ui->Button_Settings_0,&QPushButton::clicked,this,[=](){
        setStyleSheet("QMainWindow#mainScreen{border-image:url(:/files/homepage.jpg);}");
        ui->stackedWidget->setCurrentIndex(2);
        ui->Button_Play->hide();
        ui->Button_Playcard->hide();
        for (int i = 0; i < 54; ++i) {
            c[i]->hide();
        }
        pro.hide();
    });
    connect(ui->Button_Settings_1,&QPushButton::clicked,this,[=](){
        setStyleSheet("QMainWindow#mainScreen{border-image:url(:/files/homepage.jpg);}");
        ui->stackedWidget->setCurrentIndex(2);
        ui->Button_Play->hide();
        ui->Button_Playcard->hide();
    });
    connect(ui->Button_Worship,&QPushButton::clicked,this,[=](){
        setStyleSheet("QMainWindow#mainScreen{border-image:url(:/files/genshin.png);}");
        ui->stackedWidget->setCurrentIndex(5);
        ui->label_8->setText(QString::number(money));
        ui->label_9->hide();
        ui->label_10->hide();
        ui->label_11->hide();
        ui->label_12->hide();
        ui->label_13->hide();
        ui->label_14->hide();
        ui->label_15->hide();
        ui->label_16->hide();
        ui->label_17->hide();
        ui->label_18->hide();
        ui->label_pay->hide();
        ui->pushButton_paid->hide();
        ui->label_new->hide();
    });
    connect(ui->pushButton_back,&QPushButton::clicked,this,[=](){
        setStyleSheet("QMainWindow#mainScreen{border-image:url(:/files/homepage.jpg);}");
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->pushButton_plus,&QPushButton::clicked,this,[=](){
        ui->label_pay->show();
        ui->pushButton_paid->show();
        ui->pushButton_draw->setEnabled(0);
        ui->pushButton_back->setEnabled(0);
        ui->pushButton_3->setEnabled(0);
    });
    connect(ui->pushButton_paid,&QPushButton::clicked,this,[=](){
        ui->label_pay->hide();
        ui->pushButton_paid->hide();
        ui->pushButton_draw->setEnabled(1);
        ui->pushButton_back->setEnabled(1);
        ui->pushButton_3->setEnabled(1);
    });
    connect(ui->pushButton_draw,&QPushButton::clicked,this,[&](){
        if(money>=1000){
            money-=1000;
            setStyleSheet("QMainWindow#mainScreen{border-image:url(:/files/draw.png);}");
            ui->label_8->hide();
            ui->label_7->hide();
            ui->pushButton_draw->hide();
            ui->pushButton_3->hide();
            ui->pushButton_plus->hide();
            ui->pushButton_back->hide();
            ui->label_9->show();
            ui->label_10->show();
            ui->label_11->show();
            ui->label_12->show();
            ui->label_13->show();
            ui->label_14->show();
            ui->label_15->show();
            ui->label_16->show();
            ui->label_17->show();
            ui->label_18->show();
            ui->label_new->show();
            QTimer::singleShot(4000,[&](){
                setStyleSheet("QMainWindow#mainScreen{border-image:url(:/files/genshin.png);}");
                ui->label_8->setText(QString::number(money));
                ui->label_8->show();
                ui->label_7->show();
                ui->pushButton_draw->show();
                ui->pushButton_3->show();
                ui->pushButton_plus->show();
                ui->pushButton_back->show();
                ui->label_9->hide();
                ui->label_10->hide();
                ui->label_11->hide();
                ui->label_12->hide();
                ui->label_13->hide();
                ui->label_14->hide();
                ui->label_15->hide();
                ui->label_16->hide();
                ui->label_17->hide();
                ui->label_18->hide();
                ui->label_new->hide();
            });
        }
    });
    connect(ui->Button_quitsettings,&QPushButton::clicked,this,[=](){
        if(activepage){
            setStyleSheet("QMainWindow#mainScreen{border-image:url(:/files/homepage.jpg);}");
            ui->stackedWidget->setCurrentIndex(1);
            ui->Button_Play->show();
            ui->Button_Playcard->hide();
        }
        else{
            ui->stackedWidget->setCurrentIndex(0);
            ui->Button_Playcard->show();
            SetBackground(color_of_back);
            for (int i = 0; i < 54; ++i) {
                c[i]->show();
            }
            if(Player[0].active)pro.show();
        }
    });
    connect(ui->Button_setbackground,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(3);
    });
    connect(ui->Button_setsound,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(4);
    });
    connect(ui->pushButton_quitforsettings,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(2);
    });
    connect(ui->pushButton_quitforsettings_2,&QPushButton::clicked,this,[=](){
        ui->stackedWidget->setCurrentIndex(2);
    });
    connect(ui->radioButton_purple,&QPushButton::clicked,this,[=](){
        color_of_back="purple";
    });
    connect(ui->radioButton_blue,&QPushButton::clicked,this,[=](){
        color_of_back="blue";
    });
    connect(ui->radioButton_snow,&QPushButton::clicked,this,[=](){
        color_of_back="snow";
    });
    connect(ui->radioButton_night,&QPushButton::clicked,this,[=](){
        color_of_back="night";
    });
    connect(ui->radioButton_melon,&QPushButton::clicked,this,[=](){
        color_of_back="melon";
    });
}

void MainWindow::SetBackground(std::string s){
    this->setObjectName("mainScreen");
    if(s=="purple")
    {
        setStyleSheet("QMainWindow#mainScreen{border-image:url(:/p/backgrounds/background_purple.png);}");
    }
    if(s=="blue")
    {
        setStyleSheet("QMainWindow#mainScreen{border-image:url(:/p/backgrounds/background_blue.png);}");
    }
    if(s=="snow")
    {
        setStyleSheet("QMainWindow#mainScreen{border-image:url(:/p/backgrounds/background_snow.png);}");
    }
    if(s=="night")
    {
        setStyleSheet("QMainWindow#mainScreen{border-image:url(:/p/backgrounds/background_night.png);}");
    }
    if(s=="melon")
    {
        setStyleSheet("QMainWindow#mainScreen{border-image:url(:/p/backgrounds/background_melon.png);}");
    }
}

void MainWindow::NewGame(){
    generate_array();
    memset(last_played_card,-1,sizeof(last_played_card));
    last_played_num=0;
    ui->label_player0->hide();
    ui->label_player1->hide();
    ui->label_player2->hide();
    own1.clear();
    own2.clear();
    last1.clear();
    last2.clear();
    if(num_games){
        for(int i=0;i<54;i++){
            c[i]->setEnabled(1);
            c[i]->hide();
        }
        pro.stop();
    }
    num_games++;
    //player Player[3];
    pro.hide();
    for(int i=0;i<3;i++){
        Player[i].active=0;
        Player[i].cardNumber=17;
        Player[i].landlord=0;
        Player[i].turns=0;
        memset(Player[i].cards,0,sizeof(Player[i].cards));
    }
    decided=0;
    for (int i = 0; i < 54; ++i) {
        int col=(Myarray[i]-1)/13;
        int p=(Myarray[i]-1)%13;
        if(i<17)
        {
            c[i]=new card1(i,col,p,1);
            Player[0].cards[i]=i;
            c[i]->resize(48,66);
            c[i]->move(18*i+this->x()+this->width()/2-168,500+this->y());
            c[i]->show();
            c[i]->belongto=100;
        }
        else if(i<37&&i>19){
            c[i]=new card1(i,col,p,0);
            Player[2].cards[i-20]=i;
            c[i]->resize(66,48);
            c[i]->move(this->x()+this->width()/2-168-5*(i-20),50+18*(i-20)+this->y());
            c[i]->show();
            c[i]->belongto=2;
            own2.push_back(convert_encoding1_to_encoding2(Myarray[i]));
        }
        else if(i>36){
            c[i]=new card1(i,col,p,0);
            Player[1].cards[i-37]=i;
            c[i]->resize(66,48);
            c[i]->move(this->x()+this->width()/2+168+5*(i-37),50+18*(i-37)+this->y());
            c[i]->show();
            c[i]->belongto=1;
            own1.push_back(convert_encoding1_to_encoding2(Myarray[i]));
        }
        else{
            c[i]=new card1(i,col,p,0);
            c[i]->resize(48,66);
            c[i]->move(50*(i-17)+this->x()+this->width()/2-75,250+this->y());
            c[i]->show();
            c[i]->belongto=100;
            publiccard[i-17]=convert_encoding1_to_encoding2(Myarray[i]);
        }
    }
    int player_firstcall=0;
    flag_calllandlord=0;
    int player_call=(player_firstcall)%3;
    if(!player_call){
        ui->pushButton->show();
        ui->radioButton->show();
        ui->radioButton_2->show();
        QTimer::singleShot(10000,[&](){
            if(decided) return;
            decided=1;
            if(ui->radioButton->isChecked())
            {
                flag_calllandlord=1;
                for(int i=17;i<20;i++){
                    c[i]->emerged=1;
                    c[i]->setStyleSheet(cardpath[c[i]->color][c[i]->point]);
                    Player[0].cards[i]=i;
                    c[i]->belongto=0;
                }
                Player[0].cardNumber=20;
                Player[0].landlord=1;
                landlord=0;
                ui->label_player0->show();
                QTimer::singleShot(1500,[&](){
                    sortcards(20);
                });
            }
            ui->pushButton->hide();
            ui->radioButton->hide();
            ui->radioButton_2->hide();
            call[0]=call[1]=1;
            for (int j = 0; j < 1&&!flag_calllandlord; ++j) {
                if(call[j]){
                    flag_calllandlord=1;
                    sortcards(17);
                    for(int i=17;i<20;i++){
                        c[i]->emerged=1;
                        c[i]->setStyleSheet(cardpath[c[i]->color][c[i]->point]);
                        Player[j+1].cards[i]=i;
                        c[i]->belongto=j+1;
                    }
                    Player[j+1].cardNumber=20;
                    Player[j+1].landlord=1;
                    landlord=j+1;
                    if(j) ui->label_player2->show();
                    else ui->label_player1->show();
                    QTimer::singleShot(1500,[&](){
                        for(int i=17;i<20;i++){
                            c[i]->emerged=0;
                            c[i]->setStyleSheet(backpath[cardback_num]);
                            c[i]->resize(66,48);
                            if(j) c[i]->move(this->x()+this->width()/2-168-5*(i),50+18*(i)+this->y());
                            else c[i]->move(this->x()+this->width()/2+168+5*(i),50+18*(i)+this->y());
                        }
                    });
                }
            }
            ingame(num_games);
        });
        connect(ui->pushButton,&QPushButton::clicked,this,[&](){
            //cout<<decided<<endl<<ui->radioButton->isChecked()<<endl<<ui->radioButton_2->isChecked()<<endl;
            if(decided||(!ui->radioButton->isChecked()&&!ui->radioButton_2->isChecked())) return;
            decided=1;
            if(ui->radioButton->isChecked())
            {
                flag_calllandlord=1;
                for(int i=17;i<20;i++){
                    c[i]->emerged=1;
                    c[i]->setStyleSheet(cardpath[c[i]->color][c[i]->point]);
                    Player[0].cards[i]=i;
                    c[i]->belongto=0;
                }
                Player[0].cardNumber=20;
                Player[0].landlord=1;
                landlord=0;
                ui->label_player0->show();
                QTimer::singleShot(1500,[&](){
                    sortcards(20);
                });
            }
            ui->pushButton->hide();
            ui->radioButton->hide();
            ui->radioButton_2->hide();
            call[0]=call[1]=1;
            for (int j = 0; j < 1&&!flag_calllandlord; ++j) {
                if(call[j]){
                    flag_calllandlord=1;
                    sortcards(17);
                    for(int i=17;i<20;i++){
                        c[i]->emerged=1;
                        c[i]->setStyleSheet(cardpath[c[i]->color][c[i]->point]);
                        Player[j+1].cards[i]=i;
                        c[i]->belongto=j+1;
                    }
                    Player[j+1].cardNumber=20;
                    Player[j+1].landlord=1;
                    landlord=j+1;
                    if(j) ui->label_player2->show();
                    else ui->label_player1->show();
                    QTimer::singleShot(1500,[&](){
                        for(int i=17;i<20;i++){
                            c[i]->emerged=0;
                            c[i]->setStyleSheet(backpath[cardback_num]);
                            c[i]->resize(66,48);
                            if(j) c[i]->move(this->x()+this->width()/2-168-5*(i),50+18*(i)+this->y());
                            else c[i]->move(this->x()+this->width()/2+168+5*(i),50+18*(i)+this->y());
                        }
                    });
                }
            }
            ingame(num_games);
        });
    }
}

void MainWindow::sortcards(int num_card){
    sort(Myarray,Myarray+num_card,in_left);
    for(int k=0;k<num_card-1;k++){
        int col=(Myarray[k]-1)/13;
        int p=(Myarray[k]-1)%13;
        c[k]->color=col;
        c[k]->point=p;
        c[k]->belongto=0;
        c[k]->resize(18,66);
        c[k]->setStyleSheet(cardpath_shift[c[k]->color][c[k]->point]);
        c[k]->move(18*k+this->x()+this->width()/2-168,500+this->y());
    }
    int k=num_card-1;
    int col=(Myarray[k]-1)/13;
    int p=(Myarray[k]-1)%13;
    c[k]->color=col;
    c[k]->point=p;
    c[k]->belongto=0;
    c[k]->isend=1;
    c[k]->resize(48,66);
    c[k]->setStyleSheet(cardpath[c[k]->color][c[k]->point]);
    c[k]->move(18*k+this->x()+this->width()/2-168,500+this->y());
}
bool MainWindow::endgame(){
    return Player[0].cardNumber==0||Player[1].cardNumber==0||Player[2].cardNumber==0;
}

void MainWindow::ingame(int game){
    if(endgame()||activepage)return;
    //
    if(!Player[0].active){
        Player[0].active=1;
        QString s=QString::number(Player[0].turns);

        //ui->label_3->setText(s);
        //ui->label_3->show();
        if(ui->stackedWidget->currentIndex()==0)pro.show();
        pro.anime();
    }
        QTimer::singleShot(20000,[&](){
            emit end_of_turn(0,Player[0].turns,game);
        });

    connect(this,&MainWindow::end_of_turn,this,&MainWindow::end_of_play);
    connect(&r,&reward::newgame,this,&MainWindow::NewGame);
    connect(&r,&reward::tohome,this,[&](){
        setStyleSheet("QMainWindow#mainScreen{border-image:url(:/files/homepage.jpg);}");
        ui->stackedWidget->setCurrentIndex(1);
        activepage=1;
        ui->Button_Play->show();
        ui->Button_Playcard->hide();
        for (int i = 0; i < 54; ++i) {
            c[i]->hide();
        }
        pro.hide();
    });
}




MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::play1(){
    int *p=play_decide1();
    last_played_num=0;
    for(int j=0;j<(Player[0].landlord?20:17);j++){
        c[j]->reshow();
    }
    for (int i = 0; i < length; ++i) {
        last2.push_back(*(p+i));
        last_played_card[last_played_num++]=*(p+i);
        int point_to_play=convert_encoding2_to_encoding1(*(p+i));
        for(int j=0;j<(Player[1].landlord?20:17);j++){
            if(c[Player[1].cards[j]]->color*13+c[Player[1].cards[j]]->point==point_to_play){
                c[Player[1].cards[j]]->used=1;
                c[Player[1].cards[j]]->resize(48,66);
                c[Player[1].cards[j]]->move(18*(i-17)+this->x()+this->width()/2+260,250+this->y());
                c[Player[1].cards[j]]->setStyleSheet(cardpath[c[Player[1].cards[j]]->color][c[Player[1].cards[j]]->point]);
                c[Player[1].cards[j]]->show();
                c[Player[1].cards[j]]->setEnabled(0);
            }
        }
    }
    Player[1].cardNumber-=length;
}

void MainWindow::play2(){
    int *p=play_decide2();
    if(length){
        last_played_num=0;
    }
    for(int j=0;j<(Player[1].landlord?20:17);j++){
        if(c[Player[1].cards[j]]->used) c[Player[1].cards[j]]->hide();
    }
    for (int i = 0; i < length; ++i) {
        last_played_card[last_played_num++]=*(p+i);
        int point_to_play=convert_encoding2_to_encoding1(*(p+i));
        for(int j=0;j<(Player[2].landlord?20:17);j++){
            if(c[Player[2].cards[j]]->color*13+c[Player[2].cards[j]]->point==point_to_play){
                c[Player[2].cards[j]]->used=1;
                c[Player[2].cards[j]]->resize(48,66);
                c[Player[2].cards[j]]->move(18*(i-17)+this->x()+this->width()/2+260,250+this->y());
                c[Player[2].cards[j]]->setStyleSheet(cardpath[c[Player[2].cards[j]]->color][c[Player[2].cards[j]]->point]);
                c[Player[2].cards[j]]->show();
                c[Player[2].cards[j]]->setEnabled(0);
            }
        }
    }
    Player[2].cardNumber-=length;
}

void MainWindow::end_of_play(int player, int turn, int game){
    if(Player[player].active&&Player[player].turns==turn&&game==num_games&&!endgame()){
        int card_picked[20]{};
        int num_card_picked=0;
        last1.clear();
        for(int i=0;i<(Player[1].landlord?20:17);i++){
            if(c[Player[1].cards[i]]->used) c[Player[1].cards[i]]->hide();
        }
        for(int i=0;i<(Player[player].landlord?20:17);i++){
            if(c[i]->picked&&!c[i]->used){
                card_picked[num_card_picked++]=convert_encoding1_to_encoding2(c[i]->color*13+c[i]->point);
                last1.push_back(card_picked[num_card_picked-1]);
            }
        }
        int*p=&card_picked[0];
        for(int i=0;i<num_card_picked;i++){
            cout<<*(p+i)<<endl;
        }
        cout<<checkValid(p,p+num_card_picked,&last_played_card[0],&last_played_card[0]+last_played_num)<<endl;
        if(checkValid(p,p+num_card_picked,&last_played_card[0],&last_played_card[0]+last_played_num))
        {
            pro.stop();
            pro.hide();
            //ui->label_3->hide();
            Player[player].turns++;
            Player[player].active=0;
            int card_remain=0;
            int last_card=0;
            for(int j=0;j<(Player[player].landlord?20:17);j++){
                if(!c[j]->picked&&!c[j]->used){
                    last_card=j;
                    c[j]->move(18*card_remain+this->x()+this->width()/2-168,500+this->y());
                    card_remain++;
                }
                else if(!c[j]->used){
                        c[j]->resize(48,66);
                        c[j]->move(18*(j-card_remain-17)+this->x()+this->width()/2+260,250+this->y());
                        c[j]->setStyleSheet(cardpath[c[j]->color][c[j]->point]);
                        c[j]->show();
                        c[j]->setEnabled(0);
                    }
                    else{
                        c[j]->hide();
                    }
            }
            c[last_card]->isend=1;
            Player[player].cardNumber=card_remain;
            for(int j=0;j<(Player[player].landlord?20:17);j++){
                    if(c[j]->picked&&!c[j]->used) {
                        c[j]->used=1;
                        continue;
                    }
                c[j]->reshow();
            }
            if(endgame()){
                money++;
                r.count(1);
                r.show();
                return;
            }
            ingame(game);
            /*else {
                last2.clear();
                play1();
                if(!endgame()){
                    play2();
                        if(!endgame())ingame(game);
                        else {
                            money--;
                            r.count(-1);
                            r.show();
                            return;
                        }
                }
                else{
                        money--;
                        r.count(-1);
                        r.show();
                        return;
                }
            }*/
        }
    }
}

void MainWindow::on_Button_Playcard_clicked()
{
    emit end_of_turn(0,Player[0].turns,num_games);
}

void MainWindow::changeEvent(QEvent *event)
{
    if(QEvent::WindowStateChange == event->type()){
        QWindowStateChangeEvent * stateEvent = dynamic_cast<QWindowStateChangeEvent*>(event);
        if(Q_NULLPTR != stateEvent){
            if(Qt::WindowMinimized == stateEvent->oldState()){
                if(num_games)
                {
                    for(int i=0;i<54;i++){
                        c[i]->show();
                    }
                }
            }
            else if(Qt::WindowNoState == stateEvent->oldState()){
                if(num_games)
                {
                    for(int i=0;i<54;i++){
                        c[i]->hide();
                    }
                }
            }
        }
    }
}

void MainWindow::on_Button_Quit_clicked()
{

    /*QFile file;
    QString f = QFileDialog::getOpenFileName(this, QString("选择文件"), QString("/"),QString("TEXT(*.txt)"));
    file.setFileName(f);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray t;
        while(!file.atEnd())
        {
            t+=file.readLine();

        }
        ui->textEdit->setText(QString(t));
    }
    file.close();*/
    money=1000;
    QString s=QString::number(money);
    QFile file;
    file.setFileName(QFileDialog::getSaveFileName(this,QString("保存路径"),QString("/"),QString("TEXT(*.txt)")));
    //file.setFileName("save.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Truncate);
    file.write(s.toUtf8());
    file.close();
    this->close();
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    startSound->setVolume(position);
}
