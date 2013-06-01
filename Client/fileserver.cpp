#include "fileserver.h"

#include <QDir>
#include <QDebug>
#include <QMutexLocker>
#include <utility>

FileServer::FileServer()
{
//    qDebug() << "FileServer constructor";
}

void FileServer::construct(QString path)
{
    qDebug() << "FileServer::construct(path)";

    QDir* dir = new QDir(path);
    QStringList fileList = dir->entryList();
    for(int i = 0; i<fileList.size();i++) {
        qDebug() << path + fileList[i];
        addFileToList(path+fileList[i]);
    }

    qDebug() << "FileServer::construct(path) has ended"<<endl;
}


void FileServer::addWatcher(QString path)
{
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
    qDebug() << "FileServer::addFileToList";
//    std::map< QString, QSharedPointer< QFileInfo > >::iterator it = files.end();
//    qDebug() << "Utworzono iterator";
    std::pair< QString, QSharedPointer <QFileInfo> > pair = std::make_pair(path, new QFileInfo(path));
    qDebug() << "Utworzono pare";
    files.insert(std::make_pair(path, new QFileInfo(path)));
    qDebug() << "Przeszedlem insert!!!!";
    addWatcher(path);
}

bool FileServer::removeFileFromList(QString path)
{
    if (files.find(path) == files.end()) {
        return false;
    }
    files.erase(files.find(path));
    removeWatcher(path);
    return true;
}

QFileInfo &FileServer::getFileInfo(QString path)
{
    return prvGetFileInfo(path);
}

QStringList FileServer::getFileList()
{
    qDebug() << "FileServer::getFileList()"<<endl;
    QStringList list;
    std::map< QString, QSharedPointer< QFileInfo > >::iterator it;
    for(it=files.begin(); it!=files.end(); it++)
        list.push_back(it->first);
    return list;
}
