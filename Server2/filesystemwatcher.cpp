#include "filesystemwatcher.h"

#include <QDebug>

FileSystemWatcher::FileSystemWatcher(QString path, QObject* parent) : QObject(parent)
{
    watcher = new QFileSystemWatcher();
    watcher->addPath(path);
    connect(watcher,SIGNAL(fileChanged(QString)),this,SLOT(fileChangedSlot(QString)));
    connect(watcher,SIGNAL(directoryChanged(QString)),this,SLOT(dirChangedSlot(QString)));
}

void FileSystemWatcher::fileChangedSlot(QString path)
{
    qDebug() << "file" << path << "has changed";
}

void FileSystemWatcher::dirChangedSlot(QString path)
{
    qDebug() << "dir " << path << " has changed";
}
