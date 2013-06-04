#include "fileinfo.h"

#include <QMutexLocker>

FileInfo::FileInfo(QString path, QDateTime time)
{
    filePath = path;
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

