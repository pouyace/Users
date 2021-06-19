#ifndef ABSTRACTAUTHENTICATION_H
#define ABSTRACTAUTHENTICATION_H

#include <QObject>
#include <QWidget>
#include <QCryptographicHash>
#include <QDateTime>
#include <QString>
#include <QWheelEvent>
#include <QMouseEvent>
#include "GUI/generalusescout.h"
#include "GUI/database.h"

class AbstractAuthentication : public QObject
{
    Q_OBJECT
public:
    explicit AbstractAuthentication(QObject *parent = nullptr);

    virtual void setSigninDataUserUser(QString userName , QString passWord , QString remoteName) =0;
    virtual void setSignUpDataUseraUser(QString userName , QString passWord , QString rePassword) =0;
    virtual bool signIn()   =0;
    virtual bool signUp()   =0;
    virtual void signOut()  =0;
    virtual bool checkedAdminData(QString userName , QString passWord) =0;

signals:
    void incorrectLoginData();
    void thereIsAnotherError();
    void tnereIsErrorForSignUp();
    void backToLoginPage();
    void userDuplicateError();
    void userAddConnection(bool successfully);
    void sendLocalName(QString localName);

};

#endif // ABSTRACTAUTHENTICATION_H
