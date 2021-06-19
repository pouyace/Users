#include "masterpcsverifier.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QDirIterator>
#include "adminuser.h"
#include <QSysInfo>
#include "guestuser.h"
#include <QApplication>

MasterPCsVerifier::MasterPCsVerifier(const QString &path, QObject *parent):
    AbstractUserVerifier(parent)
{
    //createMasterFile(qApp->applicationDirPath()+"/masterUser.mUsr");
    QDirIterator diriterator(path);
    MasterFileUserData newUser;
    while(diriterator.hasNext()){
        QFile masterPCsReader(diriterator.next());
        qDebug()<<diriterator.filePath();
        masterPCsReader.open(QIODevice::ReadOnly);
        if(!masterPCsReader.exists()){
            qDebug()<<"Not Found";
            continue;
        }
        QDataStream in(&masterPCsReader);
        in >> newUser;
        masterPCsList.insert(newUser.machinId,newUser);
        masterPCsReader.close();
    }
}

AbstractUser *MasterPCsVerifier::verifyUser(const QString &username, const QString &password)
{
    return nullptr;
}

void MasterPCsVerifier::unregisterUser(AbstractUser *user)
{
    if(user == signedInUser){
        signedInUser = nullptr;
    }
}

bool MasterPCsVerifier::registerUser(const QString &, const QString &, const QString &, const QString &, AbstractUserVerifier::AccessLevel , const bool &)
{
    return false;
}

void MasterPCsVerifier::createMasterFile(const QString &fileAddress, const QString &fistName, const QString &lastName)
{
    qDebug()<<fileAddress;
    QFile masterUserSigner(fileAddress);
    QFileInfo fInfo = masterUserSigner.fileName();
    QDir fileDir(fInfo.absolutePath());

    MasterFileUserData newUserData;
    newUserData.username = QSysInfo::machineHostName();
    newUserData.pcName = QSysInfo::prettyProductName();
    newUserData.machinId = QSysInfo::machineUniqueId();
    newUserData.accessLevel = AbstractUser::Admin;
    newUserData.lastName = lastName;
    newUserData.firstName = fistName;

    if(!fileDir.exists()){
        fileDir.mkpath(".");
    }
    if(fileDir.exists()){
        if(masterUserSigner.open(QIODevice::WriteOnly)){
            QDataStream out(&masterUserSigner);
            out << newUserData;
            masterUserSigner.close();
        }
    }
}

AbstractUser *MasterPCsVerifier::tryCheckingIn()
{
    if(signedInUser) return signedInUser;
    QByteArray machineId(QSysInfo::machineUniqueId());
    if(masterPCsList.contains(machineId)){
        MasterFileUserData newUser = masterPCsList.value(machineId);
        switch (newUser.accessLevel) {
        case AbstractUser::Admin: signedInUser = new AdminUser(newUser.username,this);break;
        case AbstractUser::Guest: signedInUser = new GuestUser(newUser.username,this);break;
        case AbstractUser::Unknown: break;
        }
        if(signedInUser){
            signedInUser->setfName(newUser.firstName);
            signedInUser->setlName(newUser.lastName);
        }
        return signedInUser;
    }
    return nullptr;
}
