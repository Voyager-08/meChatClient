#include "contactmessagebubble.h"
#include <QPainter>
#include <QFontMetrics>
#include <QApplication>

ContactMessageBubble::ContactMessageBubble(SenderType sender,
                                         MessageType type,
                                         const QString& content,
                                         const QDateTime& time,
                                         QWidget* parent)
    : QWidget(parent)
    , m_sender(sender)
    , m_type(type)
    , m_content(content)
    , m_time(time)
    , m_maxWidth(300)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    setMaximumWidth(m_maxWidth + 40);
}

QSize ContactMessageBubble::sizeHint() const
{
    QFontMetrics fm(font());
    
    int textWidth = m_maxWidth;
    int textHeight = 0;
    
    switch (m_type) {
    case Text: {
        QStringList lines = m_content.split('\n');
        textHeight = fm.height() * lines.size();
        break;
    }
    case Image:
        textHeight = 200; // 固定图片高度
        textWidth = m_maxWidth;
        break;
    case File:
        textHeight = fm.height() + 20;
        break;
    }
    
    return QSize(textWidth + 20, textHeight + 30);
}

void ContactMessageBubble::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 根据发送者类型设置背景色
    QColor bgColor;
    switch (m_sender) {
    case Contact:
        bgColor = QColor(230, 230, 230); // 浅灰色，联系人消息
        break;
    case Self:
        bgColor = QColor(66, 185, 131);  // 微信绿，自己发送的消息
        break;
    case System:
        bgColor = QColor(240, 240, 240); // 更浅的灰色，系统消息
        break;
    }

    painter.setBrush(bgColor);
    
    // 根据发送者决定边框颜色
    QColor borderColor;
    switch (m_sender) {
    case Contact:
        borderColor = QColor(210, 210, 210);
        break;
    case Self:
        borderColor = QColor(50, 168, 110);
        break;
    case System:
        borderColor = QColor(220, 220, 220);
        break;
    }
    
    painter.setPen(QPen(borderColor, 1));

    // 计算气泡位置和大小
    int margin = (m_sender == Self) ? width() - m_maxWidth - 20 : 20;
    QRect bubbleRect(margin, 5, m_maxWidth, height() - 10);
    painter.drawRoundedRect(bubbleRect, 10, 10);

    // 根据发送者设置文字颜色
    QColor textColor;
    switch (m_sender) {
    case Contact:
    case System:
        textColor = Qt::black;
        break;
    case Self:
        textColor = Qt::white;
        break;
    }
    painter.setPen(textColor);
    painter.setFont(font());

    // 绘制内容
    QRect contentRect = bubbleRect.adjusted(10, 5, -10, -20);
    switch (m_type) {
    case Text:
        painter.drawText(contentRect, Qt::TextWordWrap, m_content);
        break;
        
    case Image:
        // 图片消息占位符
        painter.drawText(contentRect, Qt::AlignCenter, "[图片]");
        break;
        
    case File:
        // 文件消息占位符
        painter.drawText(contentRect, Qt::AlignCenter, "[文件]");
        break;
    }

    // 绘制时间
    QString timeStr = m_time.toString("HH:mm");
    QFont smallFont = font();
    smallFont.setPointSize(8);
    painter.setFont(smallFont);
    painter.setPen(QColor(150, 150, 150));
    
    QRect timeRect = bubbleRect.adjusted(0, bubbleRect.height() - 18, 0, 0);
    painter.drawText(timeRect, Qt::AlignRight | Qt::AlignVCenter, timeStr);
}