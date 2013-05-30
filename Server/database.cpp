#include "database.h"

#include <QMutexLocker>
#include <iostream>

Database::Database()
{
}

bool Database::dbOpen()
{
    QSqlDatabase db = QSqlDatabase::database();
    db = QSqlDatabase::addDatabase("QSQLITE", "connection");
    db.setDatabaseName("my.db.sqlite");
    //std::cout << "--- db opened ---" << std::endl;
    return db.open();
}

void Database::dbClose()
{
    //db.close();
}

std::vector< std::vector<QString> > Database::makeQuery(QString dbQuery, int mode)
{
    QMutexLocker locker(&dbMutex);
    QSqlQuery query( QSqlDatabase::database("connection"));
    std::vector< std::vector<QString> > results;
    /*if (!dbOpen()) {
        results.clear();
        std::cout << "Cannot open database" << std::endl;
        return results;
    }  */
    //std::cout << "- making query -" << std::endl;
    //std::cout << dbQuery.toStdString() << "\n" << std::endl;
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
    bool ret = false;
    if (dbOpen()) {
        QSqlQuery query( QSqlDatabase::database("connection"));
        query.exec("create table accounts "
                         "(id integer primary key, "
                         "name varchar(20), "
                         "pass varchar(30))");

    }    
    //std::cout << "--- db created ---" << std::endl;
    return ret;
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
