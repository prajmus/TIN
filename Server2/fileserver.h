#ifndef FILESERVER_H
#define FILESERVER_H

#include <map>
#include <utility>
#include <QSharedPointer>
#include <QMutex>
#include <QDateTime>
#include "filesystemwatcher.h"
#include "fileinfo.h"

class FileServer
{
    std::map< QString, QSharedPointer<FileSystemWatcher> > watchers;
    std::map< QString, QSharedPointer<FileInfo> > files;
    QMutex mutex;
    void addWatcher(QString path);
    void removeWatcher(QString path);
    bool removeFileFromServer(QString path);
    FileInfo &prvGetFileInfo(QString path);
    void loadListRecursive(QString path);
protected:
    FileServer();
public:
    static FileServer &getInstance();
    bool addFileToList(QString path, QDateTime time);
    bool removeFile(QString path);
    FileInfo &getFileInfo(QString path);
    std::vector< std::pair< QString, QDateTime> > listFiles();
};

#endif // FILESERVER_H
