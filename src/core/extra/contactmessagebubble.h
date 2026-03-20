#ifndef CONTACTMESSAGEBUBBLE_H
#define CONTACTMESSAGEBUBBLE_H

#include <QWidget>
#include <QDateTime>

class ContactMessageBubble : public QWidget {// 联系人消息气泡
    Q_OBJECT
public:
    enum MessageType { Text, Image, File }; /** * @brief 消息类型枚举 * 定义了消息的三种类型：文本、图片和文件 */
    enum SenderType { Contact, Self, System }; /** * @brief 发送者类型枚举 * 定义了消息的三种发送者类型：联系人、自己和系统 */

    ContactMessageBubble(SenderType sender, 
                        MessageType type, 
                        const QString& content, 
                        const QDateTime& time, 
                        QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;// 重写绘图事件处理函数
    QSize sizeHint() const override;// 返回组件的建议大小

private:
    SenderType m_sender; //  发送者类型
    MessageType m_type; //  消息类型
    QString m_content; //  消息内容
    QDateTime m_time; //  消息发送时间
    int m_maxWidth; //  消息最大显示宽度
};

#endif // CONTACTMESSAGEBUBBLE_H