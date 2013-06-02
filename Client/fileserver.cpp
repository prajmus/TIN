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
        if(fileList[i] == "." || fileList[i]=="..")
            continue;
        addFileToList(path+fileList[i]);
    }

//    qDebug() << "FileServer::construct(path) has ended"<<endl;
}

void FileServer::addFileToList(QString path)
{
//    qDebug() << "FileServer::addFileToList:"<< path;
    files.push_back(new File(path));
}

bool FileServer::removeFileFromList(QString path)
{
//    if (files.find(path) == files.end()) {
//        return false;
//    }
//    files.erase(files.find(path));
    return true;
}

QFileInfo * FileServer::prvGetFileInfo(QString path)
{
//    return *(files.find(path)->second);
}

FileServer& FileServer::getInstance()
{
    static FileServer instance;
    return instance;
}

QFileInfo * FileServer::getFileInfo(QString path)
{
    for(int i = 0; i < files.size(); i++) {
        if (path == files.at(i) -> getPath())
            return files.at(i) -> getFileInfo();
    }
    return NULL;
}

QStringList FileServer::getFileList()
{
//    qDebug() << "FileServer::getFileList()";
    QStringList list;
    for(int i=0; i<files.size();i++) {
//        qDebug() << files.at(i)->getPath();
        list.push_back(files.at(i)->getPath());
    }
    return list;
}
