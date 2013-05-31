#include "fileinfo.h"

#include <QMutexLocker>

FileInfo::FileInfo(QString path, int id)
{
    filePath = path;
    lastModifierId = id;
}

QString FileInfo::getPath()
{
    QMutexLocker locker(&mutex);
    return filePath;
}

int FileInfo::getId()
{
    QMutexLocker locker(&mutex);
    return lastModifierId;
}
