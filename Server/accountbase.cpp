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

bool AccountBase::prvExists(int id)
{
    return base.find(id) != base.end();
}

bool AccountBase::exists(int id)
{
    QMutexLocker locker(&baseMutex);
    return prvExists(id);
}

bool AccountBase::prvVerify(int id, QString password)
{
    if (base.find(id) == base.end())
        return false;
    return base.find(id)->second->verify(password);
}

bool AccountBase::verify(int id, QString password)
{
    QMutexLocker locker(&baseMutex);
    return prvVerify(id, password);
}

Account &AccountBase::prvGetAccount(int id)
{
    //assert(base.find(id) != base.end());
    return *(base.find(id)->second);
}

Account &AccountBase::getAccount(int id)
{
    QMutexLocker locker(&baseMutex);
    return prvGetAccount(id);
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

bool AccountBase::deleteUser(int id)
{
    QMutexLocker locker(&baseMutex);
    if (!prvExists(id)) {
        return false;
    }
    Database::getInstance().makeQuery(QString("DELETE FROM accounts WHERE id = '%1'")
                                  .arg(id), 0);
    base.erase(base.find(id));
    return true;
}

AccountBase &AccountBase::getInstance()
{
    static AccountBase instance;
    return instance;
}
