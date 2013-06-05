#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QFileInfo>
#include "filesystemwatcher.h"

class File
{
    QFileInfo* fileInfo;
    FileSystemWatcher* watcher;
public:
    File(QFileInfo *);
    ~File();
    QString getPath();
    QFileInfo* getFileInfo();
    FileSystemWatcher* getWatcher();
};

#endif // FILE_H
