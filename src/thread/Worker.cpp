#include "Worker.h"
#include <QFile>
#include <QPainter>
#include <QPainterPath>

Worker::Worker(QObject *parent) : QObject(parent)
{
}

Worker::~Worker()
{
}

void Worker::loadAvatar(QString userID)
{
    QString avatarPath = QString("./images/avatar/%1.png").arg(userID);
    QPixmap pixmap;
    
    if (QFile::exists(avatarPath)) {
        pixmap.load(avatarPath);
    } else {
        pixmap.load("./images/avatar/default.png");
    }
    
    emit avatarLoaded(pixmap);
}


