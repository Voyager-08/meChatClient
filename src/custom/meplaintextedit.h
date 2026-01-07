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
    void focusInEvent(QFocusEvent *event) override; // 重写焦点进入事件
    void focusOutEvent(QFocusEvent *event) override; // 重写焦点离开事件

signals:
    void enterPressed();// 回车键按下信号
    void focusIn(); // 焦点进入信号
    void focusOut(); // 焦点离开信号
};

#endif // MePlainTextEdit_H