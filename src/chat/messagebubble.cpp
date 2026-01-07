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
    QFont font;
    font.setFamily("KaiTi");  // 设置字体为正楷
    QFontMetrics fm(font); //  获取当前字体度量对象，用于测量文本尺寸
    int textWidth = fm.horizontalAdvance(m_text) + 20; //  计算文本宽度：文本水平 advancement + 20像素的边距
    int textHeight = fm.height() * (m_text.count('\n') + 1) + 10; //  计算文本高度：文本高度 * (文本中换行符数量 + 1) + 10像素的边距
    return QSize(qMin(textWidth, 300), textHeight + 30);//  返回计算后的尺寸，宽度不超过300像素，高度增加30像素
}

void MessageBubble::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 设置透明度
    QColor bgColor = (m_role == Self) ? QColor(66, 185, 131) : QColor(250, 250, 255);
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
    
    // 设置正楷字体
    QFont font;
    font.setFamily("KaiTi");
    font.setPointSize(12);
    painter.setFont(font);

    // 绘制文本（自动换行）
    QRect textRect = bubbleRect.adjusted(10, 5, -10, -5);
    painter.drawText(textRect, Qt::TextWordWrap, m_text);

    // 时间（小字，右下）
    QString timeStr = m_time.toString("HH:mm:ss");
    QFont timeFont;
    timeFont.setFamily("KaiTi");  // 时间也使用正楷字体
    timeFont.setPointSize(8);     // 设置字体大小为8
    //设置颜色
    painter.setPen(QColor(0x8B, 0x8B, 0x8B));// 深灰色
    painter.setFont(timeFont);
    QColor timeColor(Qt::black);
    painter.setPen(timeColor);
    painter.drawText(bubbleRect.adjusted(0, bubbleRect.height() - 15, -5, -1),// 设置位置x为气泡底部，y为气泡高度减15像素
                     Qt::AlignRight | Qt::AlignVCenter, timeStr);
}