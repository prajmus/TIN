#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QObject>

class CommandParser : public QObject
{
    Q_OBJECT
protected:
    CommandParser(QObject *parent = 0);

public slots:
    void process();
public:
    static CommandParser& getInstance();

signals:
    void finished();


};

#endif // COMMANDPARSER_H
