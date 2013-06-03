#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include <QFileSystemWatcher>
#include <QObject>

class FileSystemWatcher : public QObject
{
    Q_OBJECT
  public:
    FileSystemWatcher(QString path, QObject* parent = 0);
  private slots:
    void fileChangedSlot(QString path);
    void dirChangedSlot(QString path);
  signals:
    void fileChangedSignal(QString path);
    void dirChangedSignal(QString path);
  private:
    QFileSystemWatcher *watcher;
};

#endif // FILESYSTEMWATCHER_H
