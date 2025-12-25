#include "meplaintextedit.h"
#include <QKeyEvent>

MePlainTextEdit::MePlainTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    
}

void MePlainTextEdit::keyPressEvent(QKeyEvent *event)
{
    // 检查是否按的是回车键（包括主键盘和小键盘）
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        // 如果没有按 Shift 键 → 视为“发送”
        if (!(event->modifiers() & Qt::ShiftModifier)) {
            emit enterPressed(); // 发送信号
            return;              // 阻止基类处理（即不插入换行）
        }
        // 如果按了 Shift+Enter → 允许换行（交给基类处理）
    }

    // 其他按键（包括 Shift+Enter）交给基类处理
    QPlainTextEdit::keyPressEvent(event);
}