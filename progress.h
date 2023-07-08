
#ifndef PROGRESS_H
#define PROGRESS_H


#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QPropertyAnimation>
#include <qparallelanimationgroup.h>


class progress:public QLabel
{
    Q_OBJECT
public:
    explicit progress(QWidget* parent=nullptr);
    ~progress();
    void anime();
    void stop();
    QParallelAnimationGroup *group;
};

#endif // PROGRESS_H
