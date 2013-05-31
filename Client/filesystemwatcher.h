#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include <QObject>
#include <QString>
#include <QFileSystemWatcher>

class FileSystemWatcher : public QObject
{
    Q_OBJECT
public:
    explicit FileSystemWatcher(QString path, QObject *parent = 0);
    
private:
    QFileSystemWatcher* watcher;
private slots:
    void fileChangedSlot(QString path);
    void dirChangedSlot(QString path);
    
};

#endif // FILESYSTEMWATCHER_H
