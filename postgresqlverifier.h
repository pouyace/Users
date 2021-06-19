#ifndef POSTGRESQLVERIFIER_H
#define POSTGRESQLVERIFIER_H
#include "abstractuserverifier.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include "QVariant"

class QHostAddress;
class QFile;

class PostgreSqlVerifier:public AbstractUserVerifier
{
public:

    PostgreSqlVerifier(QString filePath = "");
    virtual AbstractUser * verifyUser(const QString &username, const QString &password) override;
    bool setUpConfig(const QHostAddress&ip,const qint16 &port,const QString &userName,const QString &passWord,const QString &databaseName);
    QVariant getValue(QString field);
    void checkErrorDebug();
    QVariant getValue(int position);
    bool nextRecord();
    virtual bool registerUser(const QString &userName,const QString &passWord,const QString &firstName,const QString &LastName
                      ,AccessLevel accesslevel,const bool &suspended=false)override;
    int execute(QString query);
    virtual void unregisterUser(AbstractUser *user) override;
    virtual ~PostgreSqlVerifier();
private:
    QSqlDatabase db;
    QSqlQuery result;
    QFile *errFile = nullptr;
};

#endif // POSTGRESQLVERIFIER_H
