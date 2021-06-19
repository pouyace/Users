#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include "abstractauthentication.h"

class Authentication : public AbstractAuthentication
{
    Q_OBJECT
public:
    explicit Authentication(Database *db,GeneralUseScout *generalUse, QWidget *parent = nullptr);

    virtual void setSigninDataUserUser(QString userName , QString passWord, QString remoteName) override;
    virtual void setSignUpDataUseraUser(QString userName , QString passWord , QString rePassword) override;
    virtual bool checkedAdminData(QString userName , QString passWord) override;
    virtual bool signIn()   override;
    virtual bool signUp()   override;
    virtual void signOut()  override;

signals:

public slots:

private:
    QString inUsername;
    QString inPassword;

    QString upUsername;
    QString upPassword;
    QString upRePassword;

    QString adminUsername;
    QString adminPassword;

    Database *database;
    GeneralUseScout *generalUse;



signals:

protected:
    void wheelEvent(QWheelEvent *event);

};

#endif // AUTHENTICATION_H
