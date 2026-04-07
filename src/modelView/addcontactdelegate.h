#ifndef ADDCONTACTDELEGATE_H
#define ADDCONTACTDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QObject>

class AddContactDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit AddContactDelegate(QObject *parent = nullptr);
    
    // 重写绘制方法
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    
    // 重写大小提示方法
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    
    // 重写编辑器事件方法，用于处理按钮点击
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    // 添加好友信号
    void addButtonClicked(const QModelIndex &index);

private:
    // 计算添加按钮的矩形区域
    QRect addButtonRect(const QStyleOptionViewItem &option) const;
};

#endif // ADDCONTACTDELEGATE_H