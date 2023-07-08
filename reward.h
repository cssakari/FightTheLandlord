#ifndef REWARD_H
#define REWARD_H

#include <QWidget>

namespace Ui {
class reward;
}

class reward : public QWidget
{
    Q_OBJECT

public:
    explicit reward(QWidget *parent = nullptr);
    ~reward();
    void count(int money_won);

private:
    Ui::reward *ui;

signals:
    void newgame();
    void tohome();
private slots:
    void on_Button_again_clicked();
    void on_Button_stop_clicked();
};

#endif // REWARD_H
