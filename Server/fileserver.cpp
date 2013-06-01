#include "fileserver.h"

#include <QDir>
#include <QMutexLocker>
#include <QDebug>

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
    mutex.lock();
    files.insert(std::make_pair(path, new FileInfo(path, id)));
    qDebug() << "dodaje plik do listy " << path << id;
    qDebug() << files.find(path)->first; // @Karol: dodaje do listy co widać
    mutex.unlock();
    addWatcher(path);
}

bool FileServer::removeFileFromList(QString path)
{
    mutex.lock();
    if (files.find(path) == files.end()) {
        return false;
    }
    qDebug() << "znaleziono plik: " << files.find(path)->first << "na liście plików";
    files.erase(files.find(path));
    if (files.find(path) == files.end()) {
        qDebug() << "wyjebalem";
    }
    mutex.unlock();
    removeWatcher(path);
    return true;
}
