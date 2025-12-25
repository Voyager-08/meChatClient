#include "messagebubble.h"
#include <QPainter>
#include <QStaticText>
#include <QFontMetrics>
#include <QPropertyAnimation>

MessageBubble::MessageBubble(Role role, const QString& text, const QDateTime& time, QWidget* parent)
    : QWidget(parent), m_role(role), m_text(text), m_time(time) {
    //  设置控件的大小策略 水平方向使用Preferred策略，表示控件希望获得其所需的空间，但可以被压缩 垂直方向使用Minimum策略，表示控件有最小高度限制，但可以扩展
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum); 
}

void MessageBubble::startAnimation() {
    m_animation = new QPropertyAnimation(this, "opacity");
    m_animation->setDuration(300);  // 动画持续时间300毫秒
    m_animation->setStartValue(0.0);  // 起始透明度
    m_animation->setEndValue(1.0);    // 结束透明度
    m_animation->setEasingCurve(QEasingCurve::OutCubic);  // 使用缓动曲线使动画更自然
    m_animation->start(QAbstractAnimation::DeleteWhenStopped);  // 动画结束后自动删除
}

QSize MessageBubble::sizeHint() const {
    QFontMetrics fm(font()); //  获取当前字体度量对象，用于测量文本尺寸
    int textWidth = fm.horizontalAdvance(m_text) + 20; //  计算文本宽度：文本水平 advancement + 20像素的边距
    int textHeight = fm.height() * (m_text.count('\n') + 1) + 10; //  计算文本高度：文本高度 * (文本中换行符数量 + 1) + 10像素的边距
    return QSize(qMin(textWidth, 300), textHeight + 30);//  返回计算后的尺寸，宽度不超过300像素，高度增加30像素
}

void MessageBubble::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 设置透明度
    QColor bgColor = (m_role == Self) ? QColor(66, 185, 131) : QColor(230, 230, 230);
    bgColor.setAlphaF(m_opacity);  // 应用透明度
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);

    // 气泡圆角矩形
    int margin = (m_role == Self) ? width() - 320 : 20;
    QRect bubbleRect(margin, 5, 300, height() - 10);//  气泡矩形位置和大小
    painter.drawRoundedRect(bubbleRect, 10, 10);

    // 文字颜色
    QColor textColor = (m_role == Self) ? Qt::white : Qt::black;
    textColor.setAlphaF(m_opacity);  // 应用透明度到文字
    painter.setPen(textColor);
    painter.setFont(font());

    // 绘制文本（自动换行）
    QRect textRect = bubbleRect.adjusted(10, 5, -10, -5);
    painter.drawText(textRect, Qt::TextWordWrap, m_text);

    // 时间（小字，右下）
    QString timeStr = m_time.toString("HH:mm");
    painter.setFont(QFont(font().family(), 8));
    QColor timeColor(150, 150, 150);
    timeColor.setAlphaF(m_opacity);  // 应用透明度到时间文字
    painter.setPen(timeColor);
    painter.drawText(bubbleRect.adjusted(0, bubbleRect.height() - 15, 0, 0),
                     Qt::AlignRight | Qt::AlignVCenter, timeStr);
}