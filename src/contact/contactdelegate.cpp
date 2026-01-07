#include "contactdelegate.h"
#include <QPainter>
#include <QApplication>
#include <QFile>
#include <QDir>
#include "contactmodel.h"

ContactDelegate::ContactDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    m_pixmapCache.setMaxCost(50); // 最多缓存 50 张图片
    qDebug()<<"ContactDelegate代理类创建成功";
}

QPixmap ContactDelegate::getDefaultAvatar(int size) const
{
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter p(&pixmap);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(Qt::lightGray);
    p.setPen(Qt::NoPen);
    p.drawEllipse(0, 0, size, size);
    p.end();
    return pixmap;
}

void ContactDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    painter->save(); // 保存当前画笔状态
    // 1. 绘制背景（选中/悬停效果）
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    painter->fillRect(opt.rect, opt.backgroundBrush);

    // 2. 获取数据
    QString name = index.data(static_cast<int>(ContactRoles::NameRole)).toString();
    QString avatarPath = index.data(static_cast<int>(ContactRoles::AvatarPathRole)).toString();
    QString message = index.data(static_cast<int>(ContactRoles::MessageRole)).toString();

    // 3. 设置布局参数
    int avatarSize = 48;// 头像尺寸
    int spacing = 10;// 间距
    // 计算文本区域
    QRect textRect = opt.rect.adjusted(avatarSize + spacing, 10, -spacing, 0);

    // 4. 加载头像（带缓存）
    QPixmap pixmap;
    if (!avatarPath.isEmpty()) 
    {
        if (m_pixmapCache.contains(avatarPath)) 
        {
            pixmap = *m_pixmapCache.object(avatarPath);
        }
        else if (QFile::exists(avatarPath)) 
        {
            pixmap = QPixmap(avatarPath).scaled(avatarSize, avatarSize,
                                                Qt::IgnoreAspectRatio,
                                                Qt::SmoothTransformation);
            m_pixmapCache.insert(avatarPath, new QPixmap(pixmap));
        }
    }
    if (pixmap.isNull()) pixmap = getDefaultAvatar(avatarSize);

    // 5. 绘制头像
    // 计算头像区域
    QRect avatarRect(opt.rect.left() + spacing, 
                     opt.rect.top() + (opt.rect.height() - avatarSize) / 2,
                     avatarSize, avatarSize);
    painter->drawPixmap(avatarRect, pixmap);

    // 6. 绘制姓名（加粗）
    QFont boldFont = QApplication::font();
    boldFont.setBold(true);
    painter->setFont(boldFont);
    painter->setPen(Qt::black);
    painter->drawText(textRect, Qt::AlignTop | Qt::AlignLeft, name);

    // 7. 绘制个性签名（灰色小字）
    QFont normalFont = QApplication::font();
    normalFont.setPointSize(normalFont.pointSize() - 2);
    painter->setFont(normalFont);
    painter->setPen(Qt::darkGray);
    QRect messageRect = textRect;
    // 设置签名文本位置，距离姓名下方 20 像素
    messageRect.setTop(messageRect.top() + 20);
    painter->drawText(messageRect, Qt::AlignTop | Qt::AlignLeft, message);

    painter->restore(); // 恢复画笔状态
}

QSize ContactDelegate::sizeHint(const QStyleOptionViewItem &,
                                const QModelIndex &) const
{
    return QSize(0, 70); // 每行高度 70 像素
}