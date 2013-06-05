#include "fileserver.h"
#include "client.h"

#include <QDir>
#include <QDebug>
#include <QMutexLocker>
#include <utility>

FileServer::FileServer()
{
//    qDebug() << "FileServer constructor";
  watcher = new QFileSystemWatcher();
  connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(directoryChanged(QString)));
  connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
}

void FileServer::construct(QString path)
{
    watcher->addPath(path);
    files.clear();
    dates.clear();
    qDebug() << path;
    QDir* dir = new QDir(path);
    QFileInfoList fileList = dir->entryInfoList();
    for(int i = 0; i<fileList.size();i++) {
        if (fileList.at(i).fileName()=="." || fileList.at(i).fileName()=="..")
            continue;
        if (fileList.at(i).isDir())
            constructRecursive(fileList.at(i).fileName()+"/");
        else
            addFileToList(fileList.at(i));
//        qDebug() << fileList.at(i).filePath() << ' ' << fileList.at(i).lastModified().toString("hh:mm:ss");
    }
}

void FileServer::constructRecursive(QString path)
{
    QDir *dir = new QDir(Client::getInstance().getPath() + path);
    QFileInfoList fileList = dir->entryInfoList();
    for (int i=0; i<fileList.size();i++) {
        if (fileList.at(i).fileName()=="." || fileList.at(i).fileName()=="..")
            continue;
        if(fileList.at(i).isDir())
            constructRecursive(path+fileList.at(i).fileName()+"/");
        else
            addFileToList(fileList.at(i));
//        qDebug() << fileList.at(i).filePath() << ' ' << fileList.at(i).lastModified().toString("hh:mm:ss");
    }
    delete dir;
}

void FileServer::addFileToList(QFileInfo info)
{
    qDebug() << "addwatcher";
    files.push_back(info);
    qDebug() << "add: " << info.filePath() << ' ' << info.lastModified().toString("hh:mm");
    dates.push_back(new QDateTime(info.lastModified()));
    watcher->addPath(info.filePath());
}

void FileServer::removeFileFromDisk(QString path)
{
  QFile file(path);
  file.remove();
}

bool FileServer::removeFileFromList(QString path)
{ 
  for (int i=0;i<files.size();++i) {
    if(files.at(i).filePath() == path) {
      files.removeAt(i);
      dates.removeAt(i);
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
        if (path == files.at(i).filePath()) {
            QFileInfo *info = new QFileInfo(files.at(i));
            return info;
        }
    }
    return NULL;
}

QStringList FileServer::getFileList()
{
//    qDebug() << "FileServer::getFileList()";
    QStringList list;
    for(int i=0; i<files.size();i++) {
        qDebug() << files.at(i).fileName();
        if (files.at(i).fileName() == "." || files.at(i).fileName() == "..")
          continue;
        list.push_back(files.at(i).filePath());
    }
    return list;
}

void FileServer::directoryChanged(QString path)
{
  qDebug() << "Dir changed " + path;
  QDir* dir = new QDir(Client::getInstance().getPath());
  QStringList fileList = dir->entryList();
  for (int i = 0; i<fileList.size();i++) {
      if (files.count(QFileInfo(path+fileList[i])) == 0) {
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

QList<QFileInfo> FileServer::getList()
{
    return files;
}

QList<QDateTime *> FileServer::getDates()
{
    return dates;
}
