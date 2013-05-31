#include "fileserver.h"

#include <QDir>
#include <QMutexLocker>

FileServer::FileServer()
{
}

void FileServer::addWatcher(QString path)
{
    QMutexLocker locker(&mutex);
    watchers.insert(path, new FileSystemWatcher(path));
}

void FileServer::removeWatcher(QString path)
{
    QMutexLocker locker(&mutex);
    watchers.remove(path);
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
    files.insert(path, new FileInfo(path, id));
    addWatcher(path);
}

bool FileServer::removeFileFromList(QString path)
{
    QMutexLocker locker(&mutex);
    if (!files.remove(path)) {
        return false;
    }
    removeWatcher(path);
    return true;
}
