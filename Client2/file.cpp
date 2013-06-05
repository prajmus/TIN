#include "file.h"

File::File(QFileInfo *fileInfo)
{
    this->fileInfo = fileInfo;
    this->watcher = new FileSystemWatcher(fileInfo->filePath());
}

File::~File()
{
    delete watcher;
    delete fileInfo;
}

QFileInfo* File::getFileInfo()
{
    return fileInfo;
}

FileSystemWatcher* File::getWatcher()
{
    return watcher;
}
