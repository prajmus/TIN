#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QMutex>

#include <vector>

class Database
{
    QMutex dbMutex;
    int lastInsertId;
    QSqlDatabase db;
protected:
    Database();
public:
    ~Database();
    static Database &getInstance();
    std::vector< std::vector<QString> > makeQuery(QString dbQuery, int mode);
    bool create();
    int getLastInsertId();
    bool dbOpen();
};

#endif // DATABASE_H
