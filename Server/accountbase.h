#ifndef ACCOUNTBASE_H
#define ACCOUNTBASE_H

#include "account.h"
#include <map>
#include <QMutex>
#include <QSharedPointer>
#include <QMutexLocker>


class AccountBase
{
    std::map< int, QSharedPointer<Account> > base;
    QMutex baseMutex;
    bool prvExists(int id);
    bool prvVerify(int id, QString password);
    Account& prvGetAccount(int id);
protected:
    AccountBase();
public:
    bool exists(int id);
    bool verify(int id, QString password);
    Account &getAccount(int id);
    bool registerUser(QString username, QString password);
    bool deleteUser(int id);
    static AccountBase &getInstance();
};

#endif // ACCOUNTBASE_H
