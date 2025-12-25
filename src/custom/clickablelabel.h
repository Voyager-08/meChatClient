// clickablelabel.h
#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = nullptr);

signals:
    void clicked();  // 自定义点击信号

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // CLICKABLELABEL_H