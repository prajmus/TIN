#ifndef FILEINFO_H
#define FILEINFO_H

#include <QFileInfo>
#include <QDateTime>
#include <QMutex>

class FileInfo
{
    QString filePath;
    QDateTime date;
    QMutex mutex;
public:
    FileInfo(QString path, QDateTime time);
    QString getPath();
    QDateTime getDate();
    int getId();
};

#endif // FILEINFO_H
