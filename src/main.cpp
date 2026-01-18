#include <QApplication>
#include <QLoggingCategory>
// login.cpp 或类似文件
#include "login/LoginWindow.h"
#include "chat/ChatWindow.h"

#ifdef QT_NO_DEBUG
// 在Release版本中禁用调试消息
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 只处理critical、warning和fatal消息，忽略调试消息
    if (type == QtDebugMsg) {
        return; // 忽略调试消息
    }
    
    // 对于其他类型的消息，使用默认处理方式
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s\n", localMsg.constData());
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s\n", localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s\n", localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\n", localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", localMsg.constData());
        abort();
    }
    fflush(stderr);
}
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
#ifdef QT_NO_DEBUG
    // 在Release版本中安装自定义消息处理器
    qInstallMessageHandler(messageHandler);
#endif

    LoginWindow login;
    if(login.exec() == QDialog::Accepted)
    {
        ChatWindow w(login.getUserId());
        w.show();
        return app.exec();
    }
    return 0;
}