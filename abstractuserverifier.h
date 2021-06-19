#ifndef ABSTRACTUSERVERIFIER_H
#define ABSTRACTUSERVERIFIER_H
#include "abstractuser.h"


class AbstractUserVerifier:public QObject
{
    Q_OBJECT
public:
    enum Error{NoError,UserNotFoundError,WrongPasswordError,UnknownError,ServerConnectionError,RemoteNameError,MultipleUsersNotAllowed
               ,BadNameOrPassword,DataBaseNotOpen,DatabaseError,InvlaidInput,UnknownAccessLevel,AlreadyAssignedError};

    typedef AbstractUser::AccessLevel AccessLevel;
    AbstractUserVerifier(QObject *parent=nullptr);
    virtual ~AbstractUserVerifier();
    virtual AbstractUser* verifyUser(const QString &username, const QString &password) = 0;
    Error error() const;
    virtual void unregisterUser(AbstractUser* user)=0;
    virtual bool registerUser(const QString &userName,const QString &passWord,const QString &firstName,const QString &LastName
                      ,AccessLevel accesslevel,const bool &suspended=false)=0;
    QString errorString() const;
private:
    Error _lastError;
    
protected:
    void setError(Error error);

signals:
    void reconnectionRequired(QString);
};

#endif // ABSTRACTUSERVERIFIER_H
