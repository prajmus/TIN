#include "fileserver.h"

#include <QDir>
#include <QDebug>
#include <QMutexLocker>
#include <utility>

FileServer::FileServer(QString path)
{
    QDir* dir = new QDir(path);
    QStringList fileList = dir->entryList();
    QFileInfoList fileInfoList = dir->entryInfoList();
    for(int i = 0; i<fileList.size();i++) {
        QString filePath = fileList[i];
        qDebug() << path + filePath;
        QFileInfo fileInfo =  fileInfoList[i];
        qDebug() << fileInfo.absolutePath() << endl;
        addFileToList(path+filePath);
    }
}

void FileServer::addWatcher(QString path)
{
//    QMutexLocker locker(&mutex);
    watchers.insert(std::make_pair(path, new FileSystemWatcher(path)));
}

void FileServer::removeWatcher(QString path)
{
    QMutexLocker locker(&mutex);
    watchers.erase(watchers.find(path));
}

QFileInfo &FileServer::prvGetFileInfo(QString path)
{
    return *(files.find(path)->second);
}

FileServer& FileServer::getInstance()
{
    FileServer instance;
    return instance;
}

void FileServer::addFileToList(QString path)
{
    QMutexLocker locker(&mutex);
    files.insert(std::make_pair(path, new QFileInfo(path)));
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

QFileInfo &FileServer::getFileInfo(QString path)
{
//    QMutexLocker locker(&mutex);
    return prvGetFileInfo(path);
}


