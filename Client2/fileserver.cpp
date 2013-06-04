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
    qDebug() << path;
    QDir* dir = new QDir(path);
    QFileInfoList fileList = dir->entryInfoList();
    for(int i = 0; i<fileList.size();i++) {
        qDebug() << fileList.at(i).fileName();
        if (fileList.at(i).fileName()=="." || fileList.at(i).fileName()=="..")
            continue;
        if (fileList.at(i).isDir())
            constructRecursive(fileList.at(i).fileName()+"/");
        else
            addFileToList(fileList.at(i).fileName());
    }
}

void FileServer::constructRecursive(QString path)
{
    QDir *dir = new QDir(Client::getInstance().getPath()+path);
    QFileInfoList fileList = dir->entryInfoList();
    for (int i=0; i<fileList.size();i++) {
        qDebug() << fileList.at(i).fileName();
        if (fileList.at(i).fileName()=="." || fileList.at(i).fileName()=="..")
            continue;
        if(fileList.at(i).isDir())
            constructRecursive(path+fileList.at(i).fileName()+"/");
        else
            addFileToList(path+fileList.at(i).fileName());
    }
    delete dir;
}

void FileServer::addFileToList(QString path)
{
    qDebug() << "addwatcher";
    files.push_back(new QFileInfo(path));
    qDebug() << path;
    watcher.addPath(Client::getInstance().getPath() + path);
}

void FileServer::removeFileFromDisk(QString path)
{
  QFile file(path);
  file.remove();
}

bool FileServer::removeFileFromList(QString path)
{ 
  for (int i=0;i<files.size();++i) {
    if(files.at(i)->filePath() == path) {
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
    for(int i = 0; i < files.size(); i++) {
        if (path == files.at(i)->filePath())
            return files.at(i);
    }
    return NULL;
}

QStringList FileServer::getFileList()
{
//    qDebug() << "FileServer::getFileList()";
    QStringList list;
    for(int i=0; i<files.size();i++) {
        qDebug() << files.at(i)->fileName();
        if (files.at(i)->fileName() == "." || files.at(i)->fileName() == "..")
          continue;
        list.push_back(files.at(i)->filePath());
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
  qDebug() << "File changed " + path;
  QFileInfo info(path);
  if (info.exists()) {
    emit fileModified(path);
  }
  else
    emit fileDeleted(path);
}
