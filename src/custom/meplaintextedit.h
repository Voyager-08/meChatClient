#ifndef MePlainTextEdit_H
#define MePlainTextEdit_H

//自定义输入框

#include <QPlainTextEdit>

class MePlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit MePlainTextEdit(QWidget *parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent *event) override;// 重写按键事件处理函数

signals:
    void enterPressed();// 回车键按下信号
};

#endif // MePlainTextEdit_H