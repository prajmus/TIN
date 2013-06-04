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
    qDebug() << "FileServer::construct(path)";

    QDir* dir = new QDir(path);
    QStringList fileList = dir->entryList();
    for(int i = 0; i<fileList.size();i++) {
        qDebug() << path + fileList[i];
        addFileToList(path+fileList[i]);
    }

    qDebug() << "FileServer::construct(path) has ended"<<endl;
}

void FileServer::addFileToList(QString path)
{
//    qDebug() << "FileServer::addFileToList:"<< path;
//    files.insert(files.end(),new File(path));
    files.push_back(new QFileInfo(path));
    watcher.addPath(path);
    qDebug() << files.size();
}

void FileServer::removeFileFromDisk(QString path)
{
  QFile file(path);
  file.remove();
}

bool FileServer::removeFileFromList(QString path)
{
  for (int i=0;i<files.size();++i) {
    if(files.at(i)->path() == path) {
      files.removeAt(i);
      return true;
    }
  }
  return false;
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
    return &prvGetFileInfo(path);
}

QStringList FileServer::getFileList()
{
    qDebug() << "FileServer::getFileList()";
    QStringList list;
    qDebug() << files.size();
    for(int i=0; i<files.size();i++) {
        qDebug() << files.at(i)->path();
        list.push_back(files.at(i)->path());
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
