#include "messagedelegate.h"
#include <QPainter>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QFontDatabase>
#include <QLinearGradient>
#include <QPainterPath>
#include <QStyleOption>
#include "messagemodel.h"

// 专业级消息代理类
MessageDelegate::MessageDelegate(QObject *parent)
    : QStyledItemDelegate(parent), m_hoveredRow(-1), m_checkedRow(-1)
{
    qDebug() << "MessageDelegate 专业版初始化成功";
}

QPixmap MessageDelegate::getDefaultAvatar(int size) const
{
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter p(&pixmap);
    p.setRenderHint(QPainter::Antialiasing);
    
    // 专业级默认头像设计
    QPen pen(Qt::lightGray, 1.5);
    p.setPen(pen);
    p.setBrush(Qt::white);
    p.drawEllipse(0, 0, size, size);
    
    // 添加简洁的图标
    p.setPen(Qt::gray);
    p.setFont(QFont("Segoe UI", 10, QFont::Bold));
    p.drawText(pixmap.rect(), Qt::AlignCenter, "U");
    
    p.end();
    return pixmap;
}

void MessageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    painter->save();
    
    // 1. 绘制背景 - 专业级设计
    if (option.state & QStyle::State_Selected ) {
        // 选中状态：使用主题高亮色
        painter->fillRect(option.rect, option.palette.highlight());
    } else if (m_hoveredRow == index.row()) {
        // 悬停状态：柔和的浅蓝色背景
        painter->fillRect(option.rect, QColor(235, 245, 255));
    } else {
        // 普通状态：干净的白色背景
        painter->fillRect(option.rect, option.palette.base());
    }

    // 2. 获取数据
    QString note = index.data(static_cast<int>(MessageRoles::NoteRole)).toString();
    QString avatarPath = index.data(static_cast<int>(MessageRoles::AvatarPathRole)).toString();
    QString message = index.data(static_cast<int>(MessageRoles::MessageRole)).toString();
    QDateTime time = index.data(static_cast<int>(MessageRoles::TimeRole)).toDateTime();
    QString senderID = index.data(static_cast<int>(MessageRoles::SenderIDRole)).toString();

    // 3. 布局参数
    const int avatarSize = 50; //  头像的大小
    const int spacing = 3; //  元素之间的间距
    const int timeFontSize = 9; //  时间文字的字体大小
    const int messageFontSize = 9; //  消息内容的字体大小
    const int noteFontSize = 10; //  用户名称的字体大小
    
    // 计算文本区域
    QRect textRect = option.rect.adjusted(avatarSize + spacing+10, 15, 0, -10);//左，上
    
    // 4. 加载头像（带缓存和专业处理）
    QPixmap pixmap;
    if (!avatarPath.isEmpty()) {
        if (m_pixmapCache.contains(avatarPath)) {
            pixmap = *m_pixmapCache.object(avatarPath);
        } else if (QFile::exists(avatarPath)) {
            pixmap = QPixmap(avatarPath).scaled(avatarSize, avatarSize,
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation);
            m_pixmapCache.insert(avatarPath, new QPixmap(pixmap));
        }
    }
    if (pixmap.isNull()) pixmap = getDefaultAvatar(avatarSize);

    // 5. 绘制头像
    QRect avatarRect(option.rect.left() + spacing, 
                     option.rect.top() + (option.rect.height() - avatarSize) / 2,
                     avatarSize, avatarSize);
    
    // 创建圆角矩形头像（只创建一次）
    QPixmap avatarPixmap(50, 50);
    avatarPixmap.fill(Qt::transparent);
    QPainter avatarPainter(&avatarPixmap);
    avatarPainter.setRenderHint(QPainter::Antialiasing);
    
    QPainterPath path;
    path.addRoundedRect(0, 0, 50, 50, 8, 8);//创建圆角矩形
    
    avatarPainter.setClipPath(path);
    avatarPainter.drawPixmap(0, 0, pixmap.scaled(50, 50, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    // 绘制头像
    painter->drawPixmap(avatarRect, avatarPixmap);

    // 6. 绘制昵称 - 专业排版
    QFont noteFont = QApplication::font();
    noteFont.setPointSize(noteFontSize);
    noteFont.setBold(true);
    noteFont.setFamily("KaiTi"); // 专业系统字体
    painter->setFont(noteFont);
    
    // 专业级颜色：深灰色（#333333）
    painter->setPen(QColor(51, 51, 51));
    painter->drawText(textRect, Qt::AlignTop | Qt::AlignLeft, note);
    
    // 7. 绘制消息内容 - 专业级文本处理
    QFont messageFont = QApplication::font();
    messageFont.setPointSize(messageFontSize);
    messageFont.setFamily("KaiTi");
    painter->setFont(messageFont);
    
    // 专业级消息文本颜色：深灰色（#555555）
    painter->setPen(QColor(85, 85, 85));
    
    // 专业级消息截断（右端省略）
    QString elidedMessage = painter->fontMetrics().elidedText(
        message, Qt::ElideRight, textRect.width());
    
    // 计算消息内容位置
    QRect messageRect = textRect;
    messageRect.setTop(messageRect.top() + 25);
    messageRect.setHeight(40); // 限制高度
    
    // 绘制消息内容（带适当行距）
    painter->drawText(messageRect, Qt::AlignTop | Qt::AlignLeft, elidedMessage);

    // 8. 绘制时间 - 专业级时间显示
    QFont timeFont = QApplication::font();
    timeFont.setPointSize(timeFontSize);
    timeFont.setFamily("Segoe UI");
    timeFont.setItalic(true);
    painter->setFont(timeFont);
    
    // 专业级时间颜色：深灰色（#777777）
    painter->setPen(QColor(119, 119, 119));
    
    // 格式化时间（如"15:30"）
    QString timeStr = time.time().toString("hh:mm");
    QRect timeRect = textRect;
    timeRect.setTop(timeRect.top() + 70);
    timeRect.setLeft(timeRect.right() - 50);
    
    // 专业级时间右对齐
    painter->drawText(timeRect, Qt::AlignRight | Qt::AlignTop, timeStr);

    // 9. 绘制发送者ID - 专业级辅助信息
    if (!senderID.isEmpty()) {
        QFont idFont = timeFont;
        idFont.setPointSize(timeFontSize - 1);
        painter->setFont(idFont);
        painter->setPen(QColor(150, 150, 150));
        
        QRect idRect = textRect;
        idRect.setTop(idRect.top() + 90);
        idRect.setLeft(textRect.left());
        painter->drawText(idRect, Qt::AlignLeft | Qt::AlignTop, "ID: " + senderID);
    }

    // 10. 绘制分割线 - 专业级分隔
    if (index.row() < index.model()->rowCount() - 1) {
        QPen separatorPen(QColor(230, 230, 230), 1);
        painter->setPen(separatorPen);
        painter->drawLine(option.rect.left(), option.rect.bottom() - 1, 
                         option.rect.right(), option.rect.bottom() - 1);
    }

    painter->restore();
}


QSize MessageDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(0, 75); // 专业级高度：75px
}

// 新增：设置悬停行
void MessageDelegate::setHoveredRow(int row)
{
    if (m_hoveredRow != row) {
        m_hoveredRow = row;
        emit update();
    }
}

// 新增：设置选中行
void MessageDelegate::setCheckedRow(int row)
{
    if (m_checkedRow != row) {
        m_checkedRow = row;
        emit update();
    }
}

void MessageDelegate::checkedOption(QPainter *painter,const QStyleOptionViewItem &option)
{
    painter->save();
    painter->fillRect(option.rect, option.palette.highlight());
    painter->restore();
}