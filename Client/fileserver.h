#ifndef FILESERVER_H
#define FILESERVER_H

#include <map>
#include <QSharedPointer>
#include <QMutex>
#include <QFileInfo>
#include "filesystemwatcher.h"

class FileServer
{
    std::map< QString, QSharedPointer<QFileInfo> > files;
    std::map< QString, QSharedPointer<FileSystemWatcher> > watchers;

    QMutex mutex;

    void addWatcher(QString path);
    void removeWatcher(QString path);

    QFileInfo &prvGetFileInfo(QString path);

protected:

public:
    FileServer(QString path = ".");
    static FileServer &getInstance();

    void addFileToList(QString path);
    bool removeFileFromList(QString path);

    QFileInfo &getFileInfo(QString path);
};

#endif // FILESERVER_H
