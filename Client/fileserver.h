#ifndef FILESERVER_H
#define FILESERVER_H

#include <map>
#include <QSharedPointer>
#include <QMutex>
#include <QFileInfo>
#include <QStringList>
#include "filesystemwatcher.h"

class FileServer
{
    std::map< QString, QSharedPointer<QFileInfo> > files;
    std::map< QString, QSharedPointer<FileSystemWatcher> > watchers;

    QMutex mutex;

    QString path;

    void addWatcher(QString path);
    void removeWatcher(QString path);

    QFileInfo &prvGetFileInfo(QString path);

protected:
    FileServer();

public:        
    void construct(QString path = ".");// "/home/qiubix/TIN/Client/"
    static FileServer &getInstance();

    void addFileToList(QString path);
    bool removeFileFromList(QString path);

    QFileInfo &getFileInfo(QString path);
    QStringList getFileList();
};

#endif // FILESERVER_H
