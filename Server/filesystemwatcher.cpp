#include "filesystemwatcher.h"

#include <QDebug>

FileSystemWatcher::FileSystemWatcher(QString path, QObject* parent) : QObject(parent)
{
    watcher = new QFileSystemWatcher();
    watcher->addPath(path);
    qDebug() << "ustawiono watcher na " << path;
    connect(watcher,SIGNAL(fileChanged(QString)),this,SLOT(fileChangedSlot(QString)));
    connect(watcher,SIGNAL(directoryChanged(QString)),this,SLOT(dirChangedSlot(QString)));
}

void FileSystemWatcher::fileChangedSlot(QString path)
{
    qDebug() << "zmieniono plik:" << path;
    // wywolanie czegos z parametrem path
}

void FileSystemWatcher::dirChangedSlot(QString path)
{
    qDebug() << "zmieniono folder: " << path;
    // wywolanie czegos z parametrem path
}
