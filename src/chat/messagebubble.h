#ifndef MESSAGEBUBBLE_H
#define MESSAGEBUBBLE_H

#include <QWidget>
#include <QDateTime>
#include <QPropertyAnimation>

class MessageBubble : public QWidget {// 消息气泡
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:
    enum Role { Self, Other };//自己和别人发的消息
    // 构造函数
    MessageBubble(Role role, const QString& text, const QDateTime& time, QWidget* parent = nullptr);
    
    // 动画相关
    qreal opacity() const { return m_opacity; }// 获取透明度
    void setOpacity(qreal opacity) { m_opacity = opacity; update(); }// 设置透明度并更新显示

    void startAnimation();// 启动动画

protected:
    void paintEvent(QPaintEvent* event) override; /** *@brief 重写绘图事件处理函数 @param event 绘图事件对象，包含需要重绘的区域等信息 * 该函数在窗口需要重绘时被调用，用于实现自定义的绘制逻辑 */
    QSize sizeHint() const override; /** @brief 返回组件的建议大小 @return QSize 返回组件的推荐尺寸 * 该函数用于告诉布局管理器组件的首选大小 */

private:
    Role m_role; //  角色信息变量，用于存储消息发送者的角色
    QString m_text; //  消息内容变量，用于存储具体的消息文本
    QDateTime m_time; //  时间戳变量，用于记录消息发送的时间
    qreal m_opacity = 1.0; // 透明度属性
    QPropertyAnimation *m_animation = nullptr; // 动画对象
};

#endif // MESSAGEBUBBLE_H