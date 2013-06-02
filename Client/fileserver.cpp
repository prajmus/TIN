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
//    qDebug() << "FileServer::construct(path)";

    QDir* dir = new QDir(path);
    QStringList fileList = dir->entryList();
    for(int i = 0; i<fileList.size();i++) {
//        qDebug() << path + fileList[i];
        addFileToList(path+fileList[i]);
    }

//    qDebug() << "FileServer::construct(path) has ended"<<endl;
}

void FileServer::addFileToList(QString path)
{
//    qDebug() << "FileServer::addFileToList";
    files.insert(files.end(),new File(path));
}

bool FileServer::removeFileFromList(QString path)
{
//    if (files.find(path) == files.end()) {
//        return false;
//    }
//    files.erase(files.find(path));
    return true;
}

QFileInfo &FileServer::prvGetFileInfo(QString path)
{
//    return *(files.find(path)->second);
}

FileServer& FileServer::getInstance()
{
    FileServer instance;
    return instance;
}



QFileInfo &FileServer::getFileInfo(QString path)
{
    return prvGetFileInfo(path);
}

QStringList FileServer::getFileList()
{
//    qDebug() << "FileServer::getFileList()"<<endl;
//    QStringList list;
//    std::map< QString, QSharedPointer< QFileInfo > >::iterator it;
//    for(it=files.begin(); it!=files.end(); it++)
//        list.push_back(it->first);
//    return list;
}
