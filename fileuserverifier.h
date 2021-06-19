#ifndef FILEUSERVERIFIER_H
#define FILEUSERVERIFIER_H
#include "QDataStream"
#include "abstractuser.h"
#include "abstractuserverifier.h"
#include "FileUserData.h"
#include "adminuser.h"
#include "guestuser.h"

class FileUserVerifier:public AbstractUserVerifier
{
public:
    FileUserVerifier(const QString &startPath);
    virtual ~FileUserVerifier() override;
    virtual AbstractUser *verifyUser(const QString &username, const QString &password) override;

private:
    QDir usersDir;
};

#endif // FILEUSERVERIFIER_H
