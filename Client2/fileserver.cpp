#include "fileserver.h"
#include "client.h"

#include <QDir>
#include <QDebug>
#include <QMutexLocker>
#include <utility>

FileServer::FileServer()
{
//    qDebug() << "FileServer constructor";
  connect(&watcher, SIGNAL(directoryChanged(QString)), this, SLOT(directoryChanged(QString)));
  connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
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
//    qDebug() << "FileServer::addFileToList:"<< path;
//    files.insert(files.end(),new File(path));
<<<<<<< HEAD
    files.push_back(new QFileInfo(path));
    watcher.addPath(path);
    qDebug() << files.size();
=======
    files.push_back(new File(path));
//    qDebug() << files.size();
>>>>>>> ea35973d635b944fc3cce8b25a9df9718a6b9b02
}

void FileServer::removeFileFromDisk(QString path)
{
  QFile file(path);
  file.remove();
}

bool FileServer::removeFileFromList(QString path)
{
<<<<<<< HEAD
  for (int i=0;i<files.size();++i) {
    if(files.at(i)->path() == path) {
      files.removeAt(i);
      return true;
    }
  }
  return false;
=======
    for (int i=0; i<files.size(); i++) {
        if (files.at(i)->getPath()==path) {
            files.removeAt(i);
            return true;
        }
    }
    return false;
>>>>>>> ea35973d635b944fc3cce8b25a9df9718a6b9b02
}

QFileInfo &FileServer::prvGetFileInfo(QString path)
{
//    return *(files.find(path)->second);
}

FileServer& FileServer::getInstance()
{
    static FileServer instance;
    return instance;
}



QFileInfo *FileServer::getFileInfo(QString path)
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
<<<<<<< HEAD
        qDebug() << files.at(i)->path();
        list.push_back(files.at(i)->path());
=======
//        qDebug() << files.at(i)->getPath();
        list.push_back(files.at(i)->getPath());
>>>>>>> ea35973d635b944fc3cce8b25a9df9718a6b9b02
    }
    return list;
}

void FileServer::directoryChanged(QString path)
{
  QDir* dir = new QDir(Client::getInstance().getPath());
  QStringList fileList = dir->entryList();
  for (int i = 0; i<fileList.size();i++) {
      if (files.count(new QFileInfo(path+fileList[i])) == 0) {
        emit newFile(path+fileList[i]);
        return;
      }
  }
}

void FileServer::fileChanged(QString path)
{
  QFileInfo info(path);
  if (info.exists()) {
    emit fileModified(path);
  }
  else
    emit fileDeleted(path);
}
