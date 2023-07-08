
#ifndef CARD1_H
#define CARD1_H

#include <QPushButton>
#include <QAbstractItemModel>
#include <QMouseEvent>

const QString cardpath[5][14]={{"border-image:url(:/p/cards/sa.png);","border-image:url(:/p/cards/s2.png);","border-image:url(:/p/cards/s3.png);","border-image:url(:/p/cards/s4.png);","border-image:url(:/p/cards/s5.png);","border-image:url(:/p/cards/s6.png);","border-image:url(:/p/cards/s7.png);","border-image:url(:/p/cards/s8.png);","border-image:url(:/p/cards/s9.png);","border-image:url(:/p/cards/s10.png);","border-image:url(:/p/cards/sj.png);","border-image:url(:/p/cards/sq.png);","border-image:url(:/p/cards/sk.png);"},
    {"border-image:url(:/p/cards/ca.png);","border-image:url(:/p/cards/c2.png);","border-image:url(:/p/cards/c3.png);","border-image:url(:/p/cards/c4.png);","border-image:url(:/p/cards/c5.png);","border-image:url(:/p/cards/c6.png);","border-image:url(:/p/cards/c7.png);","border-image:url(:/p/cards/c8.png);","border-image:url(:/p/cards/c9.png);","border-image:url(:/p/cards/c10.png);","border-image:url(:/p/cards/cj.png);","border-image:url(:/p/cards/cq.png);","border-image:url(:/p/cards/ck.png);"},
    {"border-image:url(:/p/cards/ha.png);","border-image:url(:/p/cards/h2.png);","border-image:url(:/p/cards/h3.png);","border-image:url(:/p/cards/h4.png);","border-image:url(:/p/cards/h5.png);","border-image:url(:/p/cards/h6.png);","border-image:url(:/p/cards/h7.png);","border-image:url(:/p/cards/h8.png);","border-image:url(:/p/cards/h9.png);","border-image:url(:/p/cards/h10.png);","border-image:url(:/p/cards/hj.png);","border-image:url(:/p/cards/hq.png);","border-image:url(:/p/cards/hk.png);"},
    {"border-image:url(:/p/cards/da.png);","border-image:url(:/p/cards/d2.png);","border-image:url(:/p/cards/d3.png);","border-image:url(:/p/cards/d4.png);","border-image:url(:/p/cards/d5.png);","border-image:url(:/p/cards/d6.png);","border-image:url(:/p/cards/d7.png);","border-image:url(:/p/cards/d8.png);","border-image:url(:/p/cards/d9.png);","border-image:url(:/p/cards/d10.png);","border-image:url(:/p/cards/dj.png);","border-image:url(:/p/cards/dq.png);","border-image:url(:/p/cards/dk.png);"},
    {"border-image:url(:/p/cards/bj.png);","border-image:url(:/p/cards/rj.png);"}
};
const QString cardpath_shift[5][14]={{"border-image:url(:/p/cards/sa.png) 0 44 0 0;","border-image:url(:/p/cards/s2.png) 0 44 0 0;","border-image:url(:/p/cards/s3.png) 0 44 0 0;","border-image:url(:/p/cards/s4.png) 0 44 0 0;","border-image:url(:/p/cards/s5.png) 0 44 0 0;","border-image:url(:/p/cards/s6.png) 0 44 0 0;","border-image:url(:/p/cards/s7.png) 0 44 0 0;","border-image:url(:/p/cards/s8.png) 0 44 0 0;","border-image:url(:/p/cards/s9.png) 0 44 0 0;","border-image:url(:/p/cards/s10.png) 0 44 0 0;","border-image:url(:/p/cards/sj.png) 0 44 0 0;","border-image:url(:/p/cards/sq.png) 0 44 0 0;","border-image:url(:/p/cards/sk.png) 0 44 0 0;"},
    {"border-image:url(:/p/cards/ca.png) 0 44 0 0;","border-image:url(:/p/cards/c2.png) 0 44 0 0;","border-image:url(:/p/cards/c3.png) 0 44 0 0;","border-image:url(:/p/cards/c4.png) 0 44 0 0;","border-image:url(:/p/cards/c5.png) 0 44 0 0;","border-image:url(:/p/cards/c6.png) 0 44 0 0;","border-image:url(:/p/cards/c7.png) 0 44 0 0;","border-image:url(:/p/cards/c8.png) 0 44 0 0;","border-image:url(:/p/cards/c9.png) 0 44 0 0;","border-image:url(:/p/cards/c10.png) 0 44 0 0;","border-image:url(:/p/cards/cj.png) 0 44 0 0;","border-image:url(:/p/cards/cq.png) 0 44 0 0;","border-image:url(:/p/cards/ck.png) 0 44 0 0;"},
    {"border-image:url(:/p/cards/ha.png) 0 44 0 0;","border-image:url(:/p/cards/h2.png) 0 44 0 0;","border-image:url(:/p/cards/h3.png) 0 44 0 0;","border-image:url(:/p/cards/h4.png) 0 44 0 0;","border-image:url(:/p/cards/h5.png) 0 44 0 0;","border-image:url(:/p/cards/h6.png) 0 44 0 0;","border-image:url(:/p/cards/h7.png) 0 44 0 0;","border-image:url(:/p/cards/h8.png) 0 44 0 0;","border-image:url(:/p/cards/h9.png) 0 44 0 0;","border-image:url(:/p/cards/h10.png) 0 44 0 0;","border-image:url(:/p/cards/hj.png) 0 44 0 0;","border-image:url(:/p/cards/hq.png) 0 44 0 0;","border-image:url(:/p/cards/hk.png) 0 44 0 0;"},
    {"border-image:url(:/p/cards/da.png) 0 44 0 0;","border-image:url(:/p/cards/d2.png) 0 44 0 0;","border-image:url(:/p/cards/d3.png) 0 44 0 0;","border-image:url(:/p/cards/d4.png) 0 44 0 0;","border-image:url(:/p/cards/d5.png) 0 44 0 0;","border-image:url(:/p/cards/d6.png) 0 44 0 0;","border-image:url(:/p/cards/d7.png) 0 44 0 0;","border-image:url(:/p/cards/d8.png) 0 44 0 0;","border-image:url(:/p/cards/d9.png) 0 44 0 0;","border-image:url(:/p/cards/d10.png) 0 44 0 0;","border-image:url(:/p/cards/dj.png) 0 44 0 0;","border-image:url(:/p/cards/dq.png) 0 44 0 0;","border-image:url(:/p/cards/dk.png) 0 44 0 0;"},
    {"border-image:url(:/p/cards/bj.png) 0 44 0 0;","border-image:url(:/p/cards/rj.png) 0 44 0 0;"}
};
const QString cardpath_h[5][14]={{"border-image:url(:/p/cards/sah.png);","border-image:url(:/p/cards/s2h.png);","border-image:url(:/p/cards/s3h.png);","border-image:url(:/p/cards/s4h.png);","border-image:url(:/p/cards/s5h.png);","border-image:url(:/p/cards/s6h.png);","border-image:url(:/p/cards/s7h.png);","border-image:url(:/p/cards/s8h.png);","border-image:url(:/p/cards/s9h.png);","border-image:url(:/p/cards/s10h.png);","border-image:url(:/p/cards/sjh.png);","border-image:url(:/p/cards/sqh.png);","border-image:url(:/p/cards/skh.png);"},
    {"border-image:url(:/p/cards/cah.png);","border-image:url(:/p/cards/c2h.png);","border-image:url(:/p/cards/c3h.png);","border-image:url(:/p/cards/c4h.png);","border-image:url(:/p/cards/c5h.png);","border-image:url(:/p/cards/c6h.png);","border-image:url(:/p/cards/c7h.png);","border-image:url(:/p/cards/c8h.png);","border-image:url(:/p/cards/c9h.png);","border-image:url(:/p/cards/c10h.png);","border-image:url(:/p/cards/cjh.png);","border-image:url(:/p/cards/cqh.png);","border-image:url(:/p/cards/ckh.png);"},
    {"border-image:url(:/p/cards/hah.png);","border-image:url(:/p/cards/h2h.png);","border-image:url(:/p/cards/h3h.png);","border-image:url(:/p/cards/h4h.png);","border-image:url(:/p/cards/h5h.png);","border-image:url(:/p/cards/h6h.png);","border-image:url(:/p/cards/h7h.png);","border-image:url(:/p/cards/h8h.png);","border-image:url(:/p/cards/h9h.png);","border-image:url(:/p/cards/h10h.png);","border-image:url(:/p/cards/hjh.png);","border-image:url(:/p/cards/hqh.png);","border-image:url(:/p/cards/hkh.png);"},
    {"border-image:url(:/p/cards/dah.png);","border-image:url(:/p/cards/d2h.png);","border-image:url(:/p/cards/d3h.png);","border-image:url(:/p/cards/d4h.png);","border-image:url(:/p/cards/d5h.png);","border-image:url(:/p/cards/d6h.png);","border-image:url(:/p/cards/d7h.png);","border-image:url(:/p/cards/d8h.png);","border-image:url(:/p/cards/d9h.png);","border-image:url(:/p/cards/d10h.png);","border-image:url(:/p/cards/djh.png);","border-image:url(:/p/cards/dqh.png);","border-image:url(:/p/cards/dkh.png);"},
    {"border-image:url(:/p/cards/bjh.png);","border-image:url(:/p/cards/rjh.png);"}
};
const QString cardpath_h_shift[5][14]={{"border-image:url(:/p/cards/sah.png) 0 44 0 0;","border-image:url(:/p/cards/s2h.png) 0 44 0 0;","border-image:url(:/p/cards/s3h.png) 0 44 0 0;","border-image:url(:/p/cards/s4h.png) 0 44 0 0;","border-image:url(:/p/cards/s5h.png) 0 44 0 0;","border-image:url(:/p/cards/s6h.png) 0 44 0 0;","border-image:url(:/p/cards/s7h.png) 0 44 0 0;","border-image:url(:/p/cards/s8h.png) 0 44 0 0;","border-image:url(:/p/cards/s9h.png) 0 44 0 0;","border-image:url(:/p/cards/s10h.png) 0 44 0 0;","border-image:url(:/p/cards/sjh.png) 0 44 0 0;","border-image:url(:/p/cards/sqh.png) 0 44 0 0;","border-image:url(:/p/cards/skh.png) 0 44 0 0;"},
    {"border-image:url(:/p/cards/cah.png) 0 44 0 0;","border-image:url(:/p/cards/c2h.png) 0 44 0 0;","border-image:url(:/p/cards/c3h.png) 0 44 0 0;","border-image:url(:/p/cards/c4h.png) 0 44 0 0;","border-image:url(:/p/cards/c5h.png) 0 44 0 0;","border-image:url(:/p/cards/c6h.png) 0 44 0 0;","border-image:url(:/p/cards/c7h.png) 0 44 0 0;","border-image:url(:/p/cards/c8h.png) 0 44 0 0;","border-image:url(:/p/cards/c9h.png) 0 44 0 0;","border-image:url(:/p/cards/c10h.png) 0 44 0 0;","border-image:url(:/p/cards/cjh.png) 0 44 0 0;","border-image:url(:/p/cards/cqh.png) 0 44 0 0;","border-image:url(:/p/cards/ckh.png) 0 44 0 0;"},
    {"border-image:url(:/p/cards/hah.png) 0 44 0 0;","border-image:url(:/p/cards/h2h.png) 0 44 0 0;","border-image:url(:/p/cards/h3h.png) 0 44 0 0;","border-image:url(:/p/cards/h4h.png) 0 44 0 0;","border-image:url(:/p/cards/h5h.png) 0 44 0 0;","border-image:url(:/p/cards/h6h.png) 0 44 0 0;","border-image:url(:/p/cards/h7h.png) 0 44 0 0;","border-image:url(:/p/cards/h8h.png) 0 44 0 0;","border-image:url(:/p/cards/h9h.png) 0 44 0 0;","border-image:url(:/p/cards/h10h.png) 0 44 0 0;","border-image:url(:/p/cards/hjh.png) 0 44 0 0;","border-image:url(:/p/cards/hqh.png) 0 44 0 0;","border-image:url(:/p/cards/hkh.png) 0 44 0 0;"},
    {"border-image:url(:/p/cards/dah.png) 0 44 0 0;","border-image:url(:/p/cards/d2h.png) 0 44 0 0;","border-image:url(:/p/cards/d3h.png) 0 44 0 0;","border-image:url(:/p/cards/d4h.png) 0 44 0 0;","border-image:url(:/p/cards/d5h.png) 0 44 0 0;","border-image:url(:/p/cards/d6h.png) 0 44 0 0;","border-image:url(:/p/cards/d7h.png) 0 44 0 0;","border-image:url(:/p/cards/d8h.png) 0 44 0 0;","border-image:url(:/p/cards/d9h.png) 0 44 0 0;","border-image:url(:/p/cards/d10h.png) 0 44 0 0;","border-image:url(:/p/cards/djh.png) 0 44 0 0;","border-image:url(:/p/cards/dqh.png) 0 44 0 0;","border-image:url(:/p/cards/dkh.png) 0 44 0 0;"},
    {"border-image:url(:/p/cards/bjh.png) 0 44 0 0;","border-image:url(:/p/cards/rjh.png) 0 44 0 0;"}
};
const QString backpath[10]={"border-image:url(:/p/cards/back.png);","border-image:url(:/p/cards/back2.png);"};
class card1 : public QPushButton
{
    Q_OBJECT

public:
    //explicit card1(QWidget *parent = nullptr);

    card1(int n,int _color,int _point,bool _emerged);
    ~card1();
    int N;
    int point;
    int color;
    int emerged;
    bool picked;
    int belongto;
    bool isend;
    bool used;
    void reshow();
    void mouseReleaseEvent(QMouseEvent *event);

private:


signals:
    void on_click();

private:
};

#endif // CARD1_H
