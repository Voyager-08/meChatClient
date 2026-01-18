#ifndef CONTACTDELEGATE_H
#define CONTACTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QStyleOptionViewItem>

class ContactDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ContactDelegate(QObject *parent = nullptr);

    // 实现绘制方法
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    
    // 实现尺寸提示方法
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    // 获取头像路径的字段名
    static const QString AVATAR_PATH_ROLE;
    // 获取备注的字段名
    static const QString NOTE_ROLE;
    // 头像缓存
    mutable QHash<QString, QPixmap> m_avatarCache;
};

#endif // MESSAGEDELEGATE_H