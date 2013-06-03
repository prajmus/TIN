#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QFileInfo>
#include "filesystemwatcher.h"

class File
{
    QString path;
    QFileInfo* fileInfo;
    FileSystemWatcher* watcher;
public:
    File(QString path);
    ~File();
    QString getPath();
    QFileInfo* getFileInfo();
    FileSystemWatcher* getWatcher();
};

#endif // FILE_H
