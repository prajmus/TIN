#ifndef ACCOUNTBASE_H
#define ACCOUNTBASE_H

#include "account.h"
#include <map>
#include <QMutex>
#include <QSharedPointer>
#include <QMutexLocker>


class AccountBase
{
    std::map< QString, QSharedPointer<Account> > base;
    QMutex baseMutex;
    bool prvExists(QString username);
    bool prvVerify(QString username, QString password);
    Account &prvGetAccount(QString username);
protected:
    AccountBase();
public:
    bool exists(QString username);
    bool verify(QString username, QString password);
    Account &getAccount(QString username);
    bool registerUser(QString username, QString password);
    bool deleteUser(QString username);
    static AccountBase &getInstance();
};

#endif // ACCOUNTBASE_H
