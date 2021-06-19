#ifndef MASTERPCSVERIFIER_H
#define MASTERPCSVERIFIER_H
#include <abstractuserverifier.h>
#include "FileUserData.h"
#include <QMap>
#include "masterfileuserdata.h"

class MasterPCsVerifier:public AbstractUserVerifier
{
    Q_OBJECT
public:
    MasterPCsVerifier(const QString &path ,QObject *parent=nullptr);
    AbstractUser * verifyUser(const QString &username, const QString &password) override;
    void unregisterUser(AbstractUser *user) override;
    bool registerUser(const QString &userName, const QString &passWord, const QString &firstName
                      , const QString &LastName, AccessLevel accesslevel, const bool &suspended = false) override;

    void createMasterFile(const QString &fileAddress,const QString& fistName,const QString& lastName);
    AbstractUser* tryCheckingIn();

private:
    QMap<QByteArray,MasterFileUserData> masterPCsList;
    AbstractUser *signedInUser = nullptr;
};

#endif // MASTERPCSVERIFIER_H
