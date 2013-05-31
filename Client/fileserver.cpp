#include "fileserver.h"

#include <QDir>
#include <QMutexLocker>
#include <utility>

FileServer::FileServer(QString path)
{
    QDir dir = new QDir(path);
    QStringList fileList = dir.entryList();
    QFileInfoList fileInfoList = dir.entryInfoList();
    for(int i = 0; i<fileList.size();i++) {
        QString filePath = fileList[i];
        std::pair <QString,QSharedPointer<QFileInfo> > entry(filePath,new QFileInfo(filePath));
        files.insert(entry);
        addWatcher(entry.first);
    }
}

void FileServer::addWatcher(QString path)
{
    QMutexLocker locker(&mutex);
    watchers.insert(std::make_pair(path, new FileSystemWatcher(path)));
}

void FileServer::removeWatcher(QString path)
{
    QMutexLocker locker(&mutex);
    watchers.erase(watchers.find(path));
}

FileInfo &FileServer::prvGetFileInfo(QString path)
{
    return *(files.find(path)->second);
}

FileInfo &FileServer::getFileInfo(QString path)
{
    QMutexLocker locker(&mutex);
    return prvGetFileInfo(path);
}

void FileServer::addFileToList(QString path, int id)
{
    QMutexLocker locker(&mutex);
    files.insert(std::make_pair(path, new QFileInfo(path, id)));
    addWatcher(path);
}

bool FileServer::removeFileFromList(QString path)
{
    QMutexLocker locker(&mutex);
    if (files.find(path) == files.end()) {
        return false;
    }
    files.erase(files.find(path));
    removeWatcher(path);
    return true;
}


FileServer& FileServer::getInstance()
{
    FileServer instance;
    return instance;
}
