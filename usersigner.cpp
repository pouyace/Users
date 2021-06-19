#include "usersigner.h"
#include "adminuser.h"
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include "fileuserverifier.h"
#include "netaddverifier.h"
#include "masterpcsverifier.h"
#include "postgresqlverifier.h"
#include <QApplication>
#include "FileUserData.h"
UserSigner::UserSigner(AbstractUserVerifier* userVerifier)
    :_userVerifier(userVerifier)
{

}

QList<QString> UserSigner::signedInUserNames() const
{
    QList<QString> signedUserNames;
    signedUserNames.reserve(_signedUsers.count());
    foreach(AbstractUser* user,_signedUsers){
        signedUserNames.append(user->userName());
    }
    return  signedUserNames;
}

UserSigner::Error UserSigner::error() const
{
    return _userVerifier->error();
}

QString UserSigner::errorString() const
{
    return _userVerifier->errorString();
}

AbstractUser *UserSigner::frontUser() const
{
    return _signedUsers.count() ?_signedUsers.top() :nullptr;
}

void UserSigner::masterPCsLogin()
{
    if(!masteruser){
        masteruser = new MasterPCsVerifier(":/MasterPCsResource");
        AbstractUser* user = masteruser->tryCheckingIn();
        if(user){
            _signedUsers.push(user);
            qDebug()<<_signedUsers.top()->userName();
            emit UserSignedIn();
        }
    }
}

void UserSigner::createMasterFile(const QString &firstName, const QString &lastName)
{
    masteruser->createMasterFile(qApp->applicationDirPath()+"/masterUser.mUsr",firstName,lastName);
}

int UserSigner::usersListCount() const
{
    return _signedUsers.count();
}
void UserSigner::signUp(const QString &userName, const QString &Password, AccessLevel accessLevel, const QString &firstname, const QString &lastname){
    FileUserData newUser{userName,Password,accessLevel};
/*
    QDir userDir("userDir");
    if(userDir.exists(newUser.username + ".txt")){
        return;
    }
    QFile addUserFile("userDir/"+newUser.username+".txt");
    addUserFile.open(QIODevice::WriteOnly);
    if(!addUserFile.isOpen()){
        qDebug()<<" could not found";
        return;
    }
    QDataStream out(&addUserFile);
    out << newUser;
    addUserFile.close();
*/
    bool status = _userVerifier->registerUser(userName,Password,firstname,lastname,accessLevel);
    if(!status)
        QMessageBox::warning(nullptr,"Error",_userVerifier->errorString());
    else if(status){
        emit userSignedUp(userName);
    }

}


AbstractUser* UserSigner::signIn(const QString& userName,const QString& password,QObject *parent){
    int index = signedInUserNames().indexOf(userName);
    if(index >= 0){
        AbstractUser* user= _signedUsers.takeAt(index);
        _signedUsers.removeAll(user);
        _signedUsers.push(user);
        qDebug()<<"Usersigner::signIn -- Switching User --";
        emit userSwitched();
        emit stackedWidgetSwitched(userName);
        return user;
    }
    qDebug()<<"Usersigner::signIn -- Signing User --";
    AbstractUser* signedUser = _userVerifier->verifyUser(userName,password);
    if(signedUser != nullptr){
     _signedUsers.push(signedUser);
     emit UserSignedIn();
     emit stackedWidgetSwitched(userName);
     return signedUser;
        }
    else{
        QMessageBox::warning(nullptr,"Error",_userVerifier->errorString());return nullptr;
    }
}

void UserSigner::signOut()
{
    if(_signedUsers.count()){
        AbstractUser *temp = _signedUsers.top();
        if(temp){
            temp->deleteLater();
        }
        _signedUsers.pop();
        emit userSignedOut();
    }

}
