#ifndef USERSIGNER_H
#define USERSIGNER_H
#include "abstractuser.h"
#include <fstream>
#include "abstractuserverifier.h"
#include <iostream>
#include <QDataStream>
#include <QStack>
#include <QMap>
#include <QObject>
class MasterPCsVerifier;
using namespace std;
class UserSigner:public QObject
{
    Q_OBJECT
public:
    typedef AbstractUser::AccessLevel AccessLevel;
    typedef AbstractUserVerifier::Error Error;
    UserSigner(AbstractUserVerifier* userVerifier);
    QList<QString> signedInUserNames() const;
    Error error() const;
    QString errorString() const;
    AbstractUser *frontUser() const;
    void masterPCsLogin();
    int usersListCount() const;

private:
    QStack<AbstractUser*> _signedUsers;
    AbstractUserVerifier *_userVerifier = nullptr;
    MasterPCsVerifier *masteruser = nullptr;

public slots:
    void createMasterFile(const QString& firstName = "",const QString& lastName = "");
    void signUp(const QString&,const QString&, AccessLevel,const  QString& firstname,const QString& password);
    void signOut();
    AbstractUser* signIn(const QString& userName,const QString& passWord,QObject *parent=nullptr);

signals:
    void userSwitched();
    void UserSignedIn();
    void userSignedOut();
    void stackedWidgetSwitched(QString userName);
    void userSignedUp(QString userName);
};

#endif // USERSIGNER_H
