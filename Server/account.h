#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QMutex>
#include <QString>

class Account
{
    bool loggedIn;
    int id;
    QString accName;
    QString accPassword;
    QMutex accMutex;
    void dbExport();
public:
    Account(int userid);
    ~Account();
    QString getName();
    int getId();
    void changePassword(QString password);
    bool verify(QString password);
    void setLoggedIn(bool log);
    bool isLoggedIn();
};

#endif // ACCOUNT_H
