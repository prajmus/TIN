#include "file.h"

File::File(QString path)
{
    this->path = path;
    this->fileInfo = new QFileInfo(path);
    this->watcher = new FileSystemWatcher(path);
}

File::~File()
{
    delete watcher;
    delete fileInfo;
}

QString File::getPath()
{
    return path;
}

QFileInfo* File::getFileInfo()
{
    return fileInfo;
}

FileSystemWatcher* File::getWatcher()
{
    return watcher;
}
