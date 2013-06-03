#include "accountserver.h"
#include "account.h"
#include "database.h"
#include <iostream>

AccountServer::AccountServer()
{
    Database::getInstance().dbOpen();
    std::vector< std::vector<QString> > results;
    QString str;
    QString id;
    int userid;
    bool ok;
    results = Database::getInstance().makeQuery(QString("SELECT name, id FROM accounts;"), 2);
    for (int i = 0; i < results.size(); i++) {
        str = results[i][0];
        id = results[i][1];
        userid = id.toInt(&ok, 10);
        base.insert(std::make_pair(str, QSharedPointer<Account>(new Account(userid))));
    }
}

bool AccountServer::prvExists(QString username)
{
    return base.find(username) != base.end();
}

bool AccountServer::exists(QString username)
{
    QMutexLocker locker(&baseMutex);
    return prvExists(username);
}

bool AccountServer::prvVerify(QString username, QString password)
{
    if (base.find(username) == base.end())
        return false;
    return base.find(username)->second->verify(password);
}

bool AccountServer::verify(QString username, QString password)
{
    QMutexLocker locker(&baseMutex);
    return prvVerify(username, password);
}

Account &AccountServer::prvGetAccount(QString username)
{
    return *(base.find(username)->second);
}

Account &AccountServer::getAccount(QString username)
{
    QMutexLocker locker(&baseMutex);
    return prvGetAccount(username);
}

bool AccountServer::registerUser(QString username, QString password)
{
    QMutexLocker locker(&baseMutex);
    int newId = -1;
    if(prvExists(username))
      return false;
    Database::getInstance().makeQuery(QString("INSERT INTO accounts VALUES(NULL, '%1', '%2')")
                                  .arg(username).arg(password), 0);
    newId = Database::getInstance().getLastInsertId();
    base.insert(std::make_pair(username, QSharedPointer<Account>(new Account(newId))));
    return true;
}

bool AccountServer::deleteUser(QString username)
{
    QMutexLocker locker(&baseMutex);
    if (!prvExists(username)) {
        return false;
    }
    Database::getInstance().makeQuery(QString("DELETE FROM accounts WHERE name = '%1'")
                                  .arg(username), 0);
    base.erase(base.find(username));
    return true;
}

AccountServer &AccountServer::getInstance()
{
    static AccountServer instance;
    return instance;
}

int AccountServer::listBase()
{
    std::map< QString, QSharedPointer<Account> >::iterator it = base.begin();
    for (; it != base.end(); it++) {
        qDebug() << it->first;
    }
    return base.size();
}
