#ifndef SHAREDSPACEDCONNECTION_H
#define SHAREDSPACEDCONNECTION_H

#include <intsafe.h>
#include <minwindef.h>
#include <windows.h>
#include <winnetwk.h>
#include <tchar.h>
#include <stdio.h>
#include <QString>
#include <QHostAddress>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDirIterator>

#include "abstractauthentication.h"

class SharedSpacedConnection : public AbstractAuthentication
{
    Q_OBJECT
public:
    explicit SharedSpacedConnection(QObject *parent = nullptr);
    virtual ~SharedSpacedConnection() override;
    void cpyByteToWChar(wchar_t *dst,const QString &str);
    virtual void setSigninDataUserUser(QString userName , QString passWord, QString remoteName) override;
    virtual void setSignUpDataUseraUser(QString userName , QString passWord , QString rePassword) override;
    virtual bool signIn()   override;
    virtual void signOut()  override;
    virtual bool signUp()   override;
    virtual bool checkedAdminData(QString userName , QString passWord) override;
    void setHostAddress(QString hostAddress);

private:
    struct NetResource{
        QString localName = "S:"   ;
        QString remoteName         ;
        QString userName           ;
        QString password           ;
    };

    NetResource _netResource;
    QString _localName = NULL   ;
    QString _remoteName         ;
    QString _userName           ;
    QString _password           ;
    QString _hostAddress        = "192.168.1.100";

public:
    QString retLocalName()  ;
    QString retRemoteName() ;
    QString retUserName()   ;
    QString retPassword()   ;
    QString hostAddress()   ;
    void wNetCancel();
    bool wNetAdd();


signals:

};

#endif // SHAREDSPACEDCONNECTION_H
