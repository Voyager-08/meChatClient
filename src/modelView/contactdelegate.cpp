#include <QPainter>
#include <QApplication>
#include <QPainterPath>
#include <QStyleOptionViewItem>
#include <QPixmap>
#include <QRect>
#include <QFileInfo>

#include "contactmodel.h"
#include "contactdelegate.h"

ContactDelegate::ContactDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void ContactDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    // hover背景
    if(option.state & QStyle::State_MouseOver) 
    {
        // 确保背景填满整个 item 区域
        QRect fullRect = option.rect;
        // 使用更浅的颜色填充整个区域
        painter->fillRect(fullRect, QColor(220, 236, 249, 255));
        // 绘制右下角阴影效果
        painter->setPen(QPen(QColor(161, 161, 161), 2));
        // 绘制右边框
        painter->drawLine(fullRect.right() - 1, fullRect.top() + 1, fullRect.right() - 1, fullRect.bottom() - 1);
        // 绘制底边框
        painter->drawLine(fullRect.left() + 1, fullRect.bottom() - 1, fullRect.right() - 1, fullRect.bottom() - 1);
    }
    // 选中时背景
    if(option.state & QStyle::State_Selected) 
    {
        // 确保背景填满整个 item 区域
        QRect fullRect = option.rect;
        // 使用指定颜色填充整个区域
        painter->fillRect(fullRect, QColor(160, 190, 220, 255));
        // 绘制左上高亮效果
        painter->setPen(QPen(QColor(220, 236, 249, 255), 2));
        // 绘制左边框
        painter->drawLine(fullRect.left(), fullRect.top(), fullRect.left(), fullRect.bottom() - 1);
        // 绘制顶边框
        painter->drawLine(fullRect.left(), fullRect.top(), fullRect.right() - 1, fullRect.top());
        // 绘制右下阴影效果
        painter->setPen(QPen(QColor(120, 150, 180, 255), 2));
        // 绘制右边框
        painter->drawLine(fullRect.right() - 1, fullRect.top() + 1, fullRect.right() - 1, fullRect.bottom() - 1);
        // 绘制底边框
        painter->drawLine(fullRect.left() + 1, fullRect.bottom() - 1, fullRect.right() - 1, fullRect.bottom() - 1);
    }
    
    
    
    // 获取数据
    QString note = index.data(Qt::DisplayRole).toString();
    QString avatarPath = index.data(static_cast<int>(ContactRoles::AvatarPathRole)).toString();

    // 定义绘制区域
    QRect contentRect = option.rect.adjusted(10, 5, -10, -5);//调整矩形区域，使其左边界向右偏移10像素，上边界向下偏移5像素，右边界向左偏移10像素，下边界向上偏移5像素
    QRect avatarRect = contentRect;
    avatarRect.setWidth(40);
    avatarRect.setHeight(40);
    avatarRect.moveLeft(contentRect.left());
    avatarRect.moveTop(contentRect.top() + (contentRect.height() - avatarRect.height()) / 2);

    // 从缓存中获取头像
    QPixmap avatarPixmap;
    if (m_avatarCache.contains(avatarPath)) {
        avatarPixmap = m_avatarCache[avatarPath];
    }
    else {
        // 尝试直接加载图片（支持物理路径和资源路径）
        avatarPixmap = QPixmap(avatarPath);
        
        // 如果加载失败（路径为空、文件不存在或资源路径无效），则加载默认头像
        if (avatarPixmap.isNull()) {
            avatarPixmap = QPixmap(":/images/avatar/default.png");
        }
        qDebug() << "加载头像路径：" << avatarPath;
        // 将加载成功的图片（无论是用户头像还是默认头像）存入缓存
        m_avatarCache.insert(avatarPath, avatarPixmap);
    }


    // 创建圆角矩形头像（只创建一次）
    QPixmap roundedAvatar(40, 40);
    roundedAvatar.fill(Qt::transparent);
    QPainter avatarPainter(&roundedAvatar);
    avatarPainter.setRenderHint(QPainter::Antialiasing);
    
    QPainterPath path;
    path.addRoundedRect(0, 0, 40, 40, 8, 8);
    
    avatarPainter.setClipPath(path);
    avatarPainter.drawPixmap(0, 0, avatarPixmap.scaled(40, 40, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    // 绘制头像
    painter->drawPixmap(avatarRect, roundedAvatar);


    // 计算文字区域
    QRect textRect = contentRect;
    textRect.setLeft(avatarRect.right() + 10); //  设置文本矩形的左边界，位于头像矩形右侧10像素处，确保文本与头像之间有适当间距
    textRect.setTop(contentRect.top() + 5); //  设置文本矩形的上边界，与内容矩形的上边界对齐，并向下偏移5像素
    textRect.setBottom(contentRect.bottom() - 5); //  设置文本矩形的下边界，与内容矩形的下边界对齐，并向上偏移5像素

    // 绘制备注
    if (!note.isEmpty()) {
        QFont kaitiFont;
        kaitiFont.setFamilies({"KaiTi", "STKaiti", "AR PL UKai CN", "Kaiti SC"}); // Qt 5.14+ 支持 setFamilies
        kaitiFont.setPointSize(QApplication::font().pointSize()+3);
        painter->setFont(kaitiFont);
        painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, 
                         painter->fontMetrics().elidedText(note, Qt::ElideRight, textRect.width()));
    }

    // 恢复画家状态
    painter->restore();
}
// 设置项的大小提示
QSize ContactDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    
    // 返回固定高度，宽度自适应
    return QSize(100, 60); // 高度60，包含头像和一些边距
}