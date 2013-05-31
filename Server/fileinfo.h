#ifndef FILEINFO_H
#define FILEINFO_H

#include <QFileInfo>
#include <QMutex>

class FileInfo
{
    QString filePath;
    int lastModifierId;
    QMutex mutex;
public:
    FileInfo(QString path, int id);
    QString getPath();
    int getId();
};

#endif // FILEINFO_H
