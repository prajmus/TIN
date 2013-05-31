#ifndef FILESERVER_H
#define FILESERVER_H

#include <map>
#include <QSharedPointer>
#include <QMutex>
#include <QFileInfo>
#include "filesystemwatcher.h"

class FileServer
{
    std::map< QString, QSharedPointer<FileSystemWatcher> > watchers;
    std::map< QString, QSharedPointer<QFileInfo> > files;
    QMutex mutex;
    void addWatcher(QString path);
    void removeWatcher(QString path);
    QFileInfo &prvGetFileInfo(QString path);
protected:
    FileServer(QString path = ".");
public:
    static FileServer &getInstance();
    void addFileToList(QString path, int id);
    bool removeFileFromList(QString path);
    QFileInfo &getFileInfo(QString path);
};

#endif // FILESERVER_H
