#include "account.h"
#include "database.h"

#include <vector>
#include <QMutexLocker>

void Account::dbExport()
{
    Database::getInstance().makeQuery(QString("UPDATE accounts SET pass = '%1' WHERE id = '%2'")
                                  .arg(accPassword).arg(id), 0);
}

Account::Account(int userid)
{
    loggedIn = false;
    id = userid;
    std::vector< std::vector<QString> > results;
    results = Database::getInstance().makeQuery(QString("SELECT name, pass FROM accounts WHERE id = '%1'")
                                            .arg(userid), 2);
    accName = results[0][0];
    accPassword = results[0][1];
}

Account::~Account()
{
}

QString Account::getName()
{
    QMutexLocker locker(&accMutex);
    return accName;
}

void Account::changePassword(QString password)
{
    QMutexLocker locker(&accMutex);
    accPassword = password;
    dbExport();
}

bool Account::verify(QString password)
{
    QMutexLocker locker(&accMutex);
    return password == accPassword;
}

void Account::setLoggedIn(bool log)
{
    loggedIn = log;
}

bool Account::isLoggedIn()
{
    return loggedIn;
}
