
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "card1.h"
#include "progress.h"
#include "qsoundeffect.h"
#include "reward.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void SetBackground(std::string s);
    void NewGame();
    void RestartGame();
    void addcards(int player);
    void sortcards(int num_card);

    bool endgame();
    void ingame(int game);
    ~MainWindow();

    void play1();
    void play2();


    int activepage;
    int num_games;
    std::string color_of_back;
    int cardback_num;
    card1 *c[54];
    int last_played_card[20];
    int last_played_num;
    progress pro;
    reward r;
    int decided;
    int flag_calllandlord;
    int starttime_of_call;
    int call[2];
    int money;
    QSoundEffect * startSound;

private:
    Ui::MainWindow *ui;

protected:
    void changeEvent(QEvent * event);

signals:
    void end_of_turn(int player,int turn,int game);
private slots:
    void on_Button_Playcard_clicked();
    void on_Button_Quit_clicked();

    void on_horizontalSlider_sliderMoved(int position);

public slots:
    void end_of_play(int player,int turn,int game);
};

#endif // MAINWINDOW_H
