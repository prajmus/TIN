#ifndef FILESERVER_H
#define FILESERVER_H

#include <map>
#include <QSharedPointer>
#include <QMutex>
#include <QFileInfo>
#include <QStringList>
#include "file.h"

class FileServer
{
    QList<File*> files;

    QMutex mutex;

    QString path;

    QFileInfo &prvGetFileInfo(QString path);

protected:
    FileServer();

public:        
    void construct(QString path = ".");
    static FileServer &getInstance();

    void addFileToList(QString path);
    bool removeFileFromList(QString path);


    QFileInfo *getFileInfo(QString path);
    QStringList getFileList();
};

#endif // FILESERVER_H
