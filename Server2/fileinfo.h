#ifndef FILEINFO_H
#define FILEINFO_H

#include <QFileInfo>
#include <QDateTime>
#include <QMutex>

class FileInfo
{
    QString filePath;
    QDateTime date;
    int lastModifierId;
    QMutex mutex;
public:
    FileInfo(QString path, int id, QDateTime time);
    QString getPath();
    QDateTime getDate();
    int getId();
};

#endif // FILEINFO_H
