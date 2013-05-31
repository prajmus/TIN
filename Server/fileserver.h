#ifndef FILESERVER_H
#define FILESERVER_H

#include <QMap>
#include <QSharedPointer>
#include <QMutex>
#include "filesystemwatcher.h"
#include "fileinfo.h"

class FileServer
{
    QMap< QString, QtSharedPointer<FileSystemWatcher> > watchers;
    QMap< QString, QSharedPointer<FileInfo> > files;
    QMutex mutex;
    void addWatcher(QString path);
    void removeWatcher(QString path);
    FileInfo &prvGetFileInfo(QString path);
protected:
    FileServer();
public:
    static FileServer &getInstance();
    void addFileToList(QString path, int id);
    bool removeFileFromList(QString path);
    FileInfo &getFileInfo(QString path);
};

#endif // FILESERVER_H
