#ifndef MESSAGEDELEGATE_H
#define MESSAGEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPixmap>
#include <QCache>

class MessageDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MessageDelegate(QObject *parent = nullptr);//显示构造函数，必须传父对象指针
    // paint() 函数的作用是：用 QPainter 在指定区域（由 option.rect 给出）画出第 index 行的数据内容。
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;// 重写绘制函数，绘制每一项的内容
    // sizeHint() 函数的作用是：返回第 index 行的尺寸。
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    void setHoveredRow(int row);// 设置当前行高亮
    void setCheckedRow(int row);// 设置当前行选中
    void checkedOption(QPainter *painter,const QStyleOptionViewItem &option);// 设置当前行选中

signals:
    void update();

private:
    mutable QCache<QString, QPixmap> m_pixmapCache; // 缓存已加载的头像
    QPixmap getDefaultAvatar(int size) const;

    int m_hoveredRow;//鼠标悬停的行
    int m_checkedRow;//选中的行
};

#endif // MESSAGEDELEGATE_H