#include "database.h"

#include <QMutexLocker>
#include <iostream>
#include <QDebug>

Database::Database()
{
}

Database::~Database()
{
    db.close();
}

bool Database::dbOpen()
{
    db = QSqlDatabase::database();
    db = QSqlDatabase::addDatabase("QSQLITE", "connection");
    db.setDatabaseName("my.db.sqlite");
    return db.open();
}

std::vector< std::vector<QString> > Database::makeQuery(QString dbQuery, int mode)
{
    QMutexLocker locker(&dbMutex);
    QSqlQuery query( QSqlDatabase::database("connection"));
    std::vector< std::vector<QString> > results;
    if (!db.isOpen()) {
        results.clear();
        std::cout << "Cannot open database" << std::endl;
        return results;
    }
    query.exec(dbQuery);
    if (mode != 0) {        
        while (query.next()) {
            std::vector<QString> values;
            for (int j = 0; j < mode; j++) {
                values.push_back(query.value(j).toString());
            }
            results.push_back(values);
        }
    }
    lastInsertId = query.lastInsertId().toInt();
    return results;
}

bool Database::create()
{
    if (!db.isOpen()) {
        std::cout << "Cannot open database" << std::endl;
        return false;
    }
    QSqlQuery query( QSqlDatabase::database("connection"));
    query.exec("create table accounts "
                     "(id integer primary key, "
                     "name varchar(20), "
                     "pass varchar(30))");
    return true;
}

int Database::getLastInsertId()
{
    QMutexLocker locker(&dbMutex);
    return lastInsertId;
}

Database &Database::getInstance()
{
    static Database instance;
    return instance;
}
