#include <QPainter>
#include <QApplication>
#include <QPainterPath>
#include <QStyleOptionViewItem>
#include <QPixmap>
#include <QRect>
#include <QFileInfo>
#include <QMouseEvent>
#include <QPushButton>

#include "contactmodel.h"
#include "addcontactdelegate.h"

AddContactDelegate::AddContactDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void AddContactDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    // 绘制选中背景
    if(option.state & QStyle::State_MouseOver) 
    {
        painter->fillRect(option.rect, option.palette.alternateBase());
    }
    
    // 获取数据
    QString name = index.data(static_cast<int>(ContactRoles::NameRole)).toString();
    QString id = index.data(static_cast<int>(ContactRoles::IDRole)).toString();
    QString avatarPath = index.data(static_cast<int>(ContactRoles::AvatarPathRole)).toString();

    // 定义绘制区域
    QRect contentRect = option.rect.adjusted(10, 5, -100, -5); // 右侧留出按钮空间
    QRect avatarRect = contentRect;
    avatarRect.setWidth(40);
    avatarRect.setHeight(40);
    avatarRect.moveLeft(contentRect.left());
    avatarRect.moveTop(contentRect.top() + (contentRect.height() - avatarRect.height()) / 2);

    // 从缓存中获取头像
    QPixmap avatarPixmap;
    // 尝试直接加载图片（支持物理路径和资源路径）
    avatarPixmap = QPixmap(avatarPath);
    
    // 如果加载失败（路径为空、文件不存在或资源路径无效），则加载默认头像
    if (avatarPixmap.isNull()) {
        avatarPixmap = QPixmap(":/images/avatar/default.png");
    }

    // 创建圆角矩形头像
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
    textRect.setLeft(avatarRect.right() + 10);
    textRect.setTop(contentRect.top());
    textRect.setBottom(contentRect.bottom());

    // 绘制昵称
    if (!name.isEmpty()) {
        QFont kaitiFont;
        kaitiFont.setFamilies({"KaiTi", "STKaiti", "AR PL UKai CN", "Kaiti SC"});
        kaitiFont.setPointSize(QApplication::font().pointSize()+3);
        painter->setFont(kaitiFont);
        painter->setPen(Qt::black);
        QRect nameRect = textRect;
        nameRect.setBottom(nameRect.top() + painter->fontMetrics().height() + 2);
        painter->drawText(nameRect, Qt::AlignVCenter | Qt::AlignLeft, 
                         painter->fontMetrics().elidedText(name, Qt::ElideRight, nameRect.width()));
    }

    // 绘制ID
    if (!id.isEmpty()) {
        QFont idFont = QApplication::font();
        idFont.setPointSize(idFont.pointSize() - 2);
        painter->setFont(idFont);
        painter->setPen(Qt::gray);
        QRect idRect = textRect;
        idRect.setTop(idRect.top() + painter->fontMetrics().height() + 4);
        painter->drawText(idRect, Qt::AlignVCenter | Qt::AlignLeft, 
                         painter->fontMetrics().elidedText(id, Qt::ElideRight, idRect.width()));
    }

    // 绘制添加按钮
    QRect buttonRect = addButtonRect(option);
    
    // 绘制按钮背景
    QPainterPath buttonPath;
    buttonPath.addRoundedRect(buttonRect, 4, 4);
    painter->fillPath(buttonPath, QColor(76, 175, 80)); // 绿色
    
    // 绘制按钮文字
    painter->setFont(QApplication::font());
    painter->setPen(Qt::white);
    painter->drawText(buttonRect, Qt::AlignCenter, "添加");

    // 恢复画家状态
    painter->restore();
}

QSize AddContactDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    
    // 返回固定高度，宽度自适应
    return QSize(100, 60); // 高度60，包含头像和一些边距
}

bool AddContactDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    // 处理鼠标点击事件
    if (event->type() == QEvent::MouseButtonRelease) {// 处理鼠标释放事件
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QRect buttonRect = addButtonRect(option);
        // 检查点击位置是否在添加按钮内
        if (buttonRect.contains(mouseEvent->pos())) {
            emit addButtonClicked(index);
            return true;
        }
    }
    
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QRect AddContactDelegate::addButtonRect(const QStyleOptionViewItem &option) const
{
    // 计算添加按钮的矩形区域
    QRect rect = option.rect;
    QRect buttonRect;
    buttonRect.setWidth(60);
    buttonRect.setHeight(30);
    buttonRect.moveRight(rect.right() - 20);
    buttonRect.moveTop(rect.top() + (rect.height() - buttonRect.height()) / 2);
    return buttonRect;
}