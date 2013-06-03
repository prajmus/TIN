#include "fileinfo.h"

#include <QMutexLocker>

FileInfo::FileInfo(QString path, int id, QDateTime time)
{
    filePath = path;
    lastModifierId = id;
    date = time;
}

QString FileInfo::getPath()
{
    QMutexLocker locker(&mutex);
    return filePath;
}

QDateTime FileInfo::getDate()
{
  return date;
}

int FileInfo::getId()
{
    QMutexLocker locker(&mutex);
    return lastModifierId;
}
