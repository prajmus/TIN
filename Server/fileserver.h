#ifndef FILESERVER_H
#define FILESERVER_H

#include <map>
#include <QSharedPointer>
#include <QMutex>
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
protected:
    FileServer();
public:
    static FileServer &getInstance();
    bool addFileToList(QString path, int id);
    bool removeFile(QString path);
    FileInfo &getFileInfo(QString path);
    int listFiles();
};

#endif // FILESERVER_H
