// Worker线程类
#pragma once
#include <QObject>
#include <QThread>
#include <QFuture>
#include <QPixmap>
#include <QDebug>

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(QObject *parent = nullptr);
    ~Worker();
public slots:
    void loadAvatar(QString userID);
signals:
    void avatarLoaded(QPixmap pixmap);
};


