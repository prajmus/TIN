#include "fileserver.h"

#include <QDir>
#include <QFile>
#include <QMutexLocker>
#include <QDebug>
#include <iostream>
#include <vector>

FileServer::FileServer()
{
}

FileServer &FileServer::getInstance()
{
    static FileServer instance;
    return instance;
}

void FileServer::addWatcher(QString path)
{
    QMutexLocker locker(&mutex);
//    watchers.insert(std::make_pair(path, new FileSystemWatcher(path)));
}

void FileServer::removeWatcher(QString path)
{
    QMutexLocker locker(&mutex);
    watchers.erase(watchers.find(path));
}

bool FileServer::removeFileFromServer(QString path)
{
    QMutexLocker locker(&mutex);
    QFile file(path);
    if (!file.exists()) {
        std::cout << "File does not exist!" << std::endl;
        return false;
    }
    return file.remove();
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

std::vector<std::pair<QString, QDateTime> > FileServer::listFiles()
{
  std::vector<std::pair<QString, QDateTime> > t;
    std::map< QString, QSharedPointer<FileInfo> >::iterator it = files.begin();
    for (; it != files.end(); it++) {
      t.push_back(std::make_pair(it->first, it->second->getDate()));
    }
    return t;
}

bool FileServer::addFileToList(QString path, int id, QDateTime time)
{
    mutex.lock();
    QFile file(path);
    if (!file.exists()) {
        std::cout << "File does not exist!" << std::endl;
        return false;
    }
    files.insert(std::make_pair(path, QSharedPointer<FileInfo>(new FileInfo(path, id, time))));
    mutex.unlock();
    //addWatcher(path);
    return true;
}

bool FileServer::removeFile(QString path)
{
    mutex.lock();
    if (files.find(path) == files.end()) {
        return false;
    }
    files.erase(files.find(path));
    mutex.unlock();
    //removeWatcher(path);
    return removeFileFromServer(path);
}
