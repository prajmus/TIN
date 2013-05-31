#include "accountbase.h"
#include "account.h"
#include "database.h"
#include <iostream>

AccountBase::AccountBase()
{
    std::vector< std::vector<QString> > results;
    QString str;
    int userid;
    bool ok;
    results = Database::getInstance().makeQuery(QString("SELECT id FROM accounts;"), 1);
    for (int i = 0; i < results.size(); i++) {
        str = results[i][0];
        userid = str.toInt(&ok, 10);
        base.insert(std::make_pair(userid, new Account(userid)));
    }
}

bool AccountBase::prvExists(QString username)
{
    return base.find(username) != base.end();
}

bool AccountBase::exists(QString username)
{
    QMutexLocker locker(&baseMutex);
    return prvExists(username);
}

bool AccountBase::prvVerify(QString username, QString password)
{
    if (base.find(username) == base.end())
        return false;
    return base.find(username)->second->verify(password);
}

bool AccountBase::verify(QString username, QString password)
{
    QMutexLocker locker(&baseMutex);
    return prvVerify(username, password);
}

Account &AccountBase::prvGetAccount(QString username)
{
    //assert(base.find(id) != base.end());
    return *(base.find(username)->second);
}

Account &AccountBase::getAccount(QString username)
{
    QMutexLocker locker(&baseMutex);
    return prvGetAccount(username);
}

bool AccountBase::registerUser(QString username, QString password)
{
    QMutexLocker locker(&baseMutex);
    int newId = -1;
    Database::getInstance().makeQuery(QString("INSERT INTO accounts VALUES(NULL, '%1', '%2')")
                                  .arg(username).arg(password), 0);
    newId = Database::getInstance().getLastInsertId();
    base.insert(std::make_pair(newId ,new Account(newId)));
    return true;
}

bool AccountBase::deleteUser(QString username)
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

AccountBase &AccountBase::getInstance()
{
    static AccountBase instance;
    return instance;
}
