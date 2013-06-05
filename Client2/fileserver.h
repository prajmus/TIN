#ifndef FILESERVER_H
#define FILESERVER_H

#include <map>
#include <QSharedPointer>
#include <QFileSystemWatcher>
#include <QMutex>
#include <QFileInfo>
#include <QStringList>
#include "file.h"

class FileServer : public QObject
{
    Q_OBJECT
    QList<QFileInfo> files;
    QList<QDateTime *> dates;

    QMutex mutex;

    QString path;

    QFileInfo &prvGetFileInfo(QString path);
    QFileSystemWatcher *watcher;

  protected:
    FileServer();
  private slots:
    void fileChanged(QString path);
    void directoryChanged(QString path);
  public slots:
    void removeFileFromDisk(QString path);
  public:
    void construct(QString path = ".");
    static FileServer &getInstance();

    void addFileToList(QFileInfo info);
    bool removeFileFromList(QString path);


    QFileInfo *getFileInfo(QString path);
    QStringList getFileList();

    void constructRecursive(QString path);
    QList<QFileInfo> getList();
    QList<QDateTime *> getDates();
    signals:
    void fileModified(QString);
    void fileDeleted(QString);
    void newFile(QString);

};

#endif // FILESERVER_H
